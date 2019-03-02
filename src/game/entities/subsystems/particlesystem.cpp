#include "../../../framework/debug.h"
#include "../../../framework/common.h"

#include "particlesystem.h"
#include "../spawnparticleinfo.h"
#include "../components/animationcomponent.h"
#include "../components/boundingspherecomponent.h"
#include "../components/particlecomponent.h"
#include "../components/particletrailcomponent.h"
#include "../components/positioncomponent.h"
#include "../components/positionoffsetcomponent.h"
#include "../components/flatvelocityparticlecomponent.h"
#include "../events/spawnparticlesevent.h"
#include "../presets/particles/particlepresetproperties.h"
#include "../../../entities/entity.h"
#include "../../../entities/entitymanager.h"
#include "../../../entities/components/inactivecomponent.h"
#include "../../../events/eventmanager.h"
#include "../../../framework/graphics/color.h"
#include "../../../framework/math/common.h"

ParticleSystem::ParticleSystem(EntityManager *entityManager, EventManager *eventManager)
	: ComponentSystem(entityManager, eventManager)
{
	STACK_TRACE;
	ListenFor<SpawnParticlesEvent>();
}

ParticleSystem::~ParticleSystem()
{
	STACK_TRACE;
	StopListeningFor<SpawnParticlesEvent>();
}

void ParticleSystem::OnUpdate(float delta)
{
	STACK_TRACE;
	EntityList list;
	
	GetEntityManager()->GetAllWith<ParticleTrailComponent>(list);
	for (EntityList::iterator i = list.begin(); i != list.end(); ++i)
	{
		Entity *entity = *i;
		ParticleTrailComponent *particleTrail = entity->Get<ParticleTrailComponent>();
		
		particleTrail->timeSinceLast += delta;
		
		float spawnIfPastThisTime = Rnd(particleTrail->minTime, particleTrail->maxTime);
		if (particleTrail->timeSinceLast > spawnIfPastThisTime)
		{
			particleTrail->timeSinceLast = 0.0f;
						
			SpawnParticlesEvent particleEvent;
			particleEvent.particleInfo = particleTrail->particleInfo;

			// automatically set a few things from this entity's own components
			PositionComponent *position = entity->Get<PositionComponent>();
			PositionOffsetComponent *positionOffset = entity->Get<PositionOffsetComponent>();
			BoundingSphereComponent *bounds = entity->Get<BoundingSphereComponent>();
			particleEvent.particleInfo.position = position->position;
			if (particleEvent.particleInfo.spawnInRadiusAroundPosition && bounds != NULL)
				particleEvent.particleInfo.radius = bounds->bounds.radius;
			if (particleTrail->particleInfo.includePositionOffset && positionOffset != NULL)
				particleEvent.particleInfo.position += positionOffset->offset;
			
			GetEventManager()->Trigger(&particleEvent);
		}
	}
	
	list.clear();
	GetEntityManager()->GetAllWith<ParticleComponent>(list);
	for (EntityList::iterator i = list.begin(); i != list.end(); ++i)
	{
		Entity *particle = *i;
		ParticleComponent *particleInfo = particle->Get<ParticleComponent>();

		// update particle life
		if (particleInfo->lastsUntilAnimationFinishes)
		{
			// check if the animation is done, if so, switch off this flag
			// so that the particle will last until it's timer runs out
			// (which if it's not set, will be on the next tick)
			// this way, the last frame of animation can be shown for a little
			// bit instead of the particle disappearing immediately when the
			// last frame of animation is reached
			AnimationComponent *animation = particle->Get<AnimationComponent>();
			ASSERT(animation != NULL);
			if (animation->IsAnimationFinished())
				particleInfo->lastsUntilAnimationFinishes = FALSE;
		}
		else
		{
			// simple time left update
			particleInfo->lifeTimeLeft -= delta;
			if (particleInfo->lifeTimeLeft < 0.0f)
			{
				particle->Add<InactiveComponent>();
				continue;
			}
		}

		PositionComponent *position = particle->Get<PositionComponent>();
		ASSERT(position != NULL);

		// update position/speed depending on specific particle type
		FlatVelocityParticleComponent *velocity = particle->Get<FlatVelocityParticleComponent>();
		if (velocity != NULL)
		{
			Vector3 movement = velocity->velocity * delta;
			position->position += movement;
		}
	}
}

BOOL ParticleSystem::Handle(const Event *event)
{
	STACK_TRACE;
	if (event->Is<SpawnParticlesEvent>())
	{
		const SpawnParticlesEvent *e = event->As<SpawnParticlesEvent>();
		
		ParticlePresetProperties spawnArgs;
		spawnArgs.spawnPosition = e->particleInfo.position;
		spawnArgs.canSpawnAroundPosition = e->particleInfo.spawnInRadiusAroundPosition;
		spawnArgs.canOnlySpawnAroundPositionOnXZ = e->particleInfo.canOnlySpawnOnXZ;
		spawnArgs.radius = e->particleInfo.radius;
		spawnArgs.lowColor = e->particleInfo.lowColor;
		spawnArgs.highColor = e->particleInfo.highColor;

		uint32_t numParticles;
		if (e->particleInfo.minimumAmount == e->particleInfo.maximumAmount)
			numParticles = e->particleInfo.minimumAmount;
		else
			numParticles = (uint32_t)Rnd((int32_t)e->particleInfo.minimumAmount, (int32_t)e->particleInfo.maximumAmount);
		
		for (uint32_t i = 0; i < numParticles; ++i)
			GetEntityManager()->AddUsingPreset(e->particleInfo.particlePreset, &spawnArgs);
	}

	return FALSE;
}
