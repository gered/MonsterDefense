#include "../../../../framework/debug.h"

#include "poweruppreset.h"
#include "poweruppresetproperties.h"
#include "../../components/affectedbygravitycomponent.h"
#include "../../components/blobshadowcomponent.h"
#include "../../components/boundingspherecomponent.h"
#include "../../components/colorcomponent.h"
#include "../../components/despawntimercomponent.h"
#include "../../components/floatbouncecomponent.h"
#include "../../components/physicscomponent.h"
#include "../../components/positioncomponent.h"
#include "../../components/positionoffsetcomponent.h"
#include "../../components/powerupcomponent.h"
#include "../../components/removephysicswhenstillcomponent.h"
#include "../../events/powerupcollectedevent.h"
#include "../../powerups/powerup.h"
#include "../../../physicsconstants.h"
#include "../../../../entities/entity.h"
#include "../../../../entities/entitymanager.h"
#include "../../../../events/event.h"
#include "../../../../events/eventlistenerex.h"
#include "../../../../events/eventmanager.h"

PowerUpPreset::PowerUpPreset(EntityManager *entityManager)
	: EntityPreset(entityManager),
	  EventListenerEx(entityManager->GetEventManager())
{
	STACK_TRACE;
	ListenFor<PowerUpCollectedEvent>();
}

PowerUpPreset::~PowerUpPreset()
{
	STACK_TRACE;
	StopListeningFor<PowerUpCollectedEvent>();
}

Entity* PowerUpPreset::Create(EntityPresetArgs *args)
{
	STACK_TRACE;
	ASSERT(args != NULL);
	PowerUpPresetProperties *spawnArgs = args->As<PowerUpPresetProperties>();
	ASSERT(spawnArgs->powerUp != NULL);
	
	Entity *entity = GetEntityManager()->Add();
	
	entity->Add<AffectedByGravityComponent>();
	
	entity->Add<BoundingSphereComponent>()
	->bounds.radius = 0.3f;
	
	FloatBounceComponent *floatBounce = entity->Add<FloatBounceComponent>();
	floatBounce->isMovingUp = TRUE;
	floatBounce->position = 0.0f;
	floatBounce->minY = 0.0f;
	floatBounce->maxY = 0.25;
	
	PhysicsComponent *physics = entity->Add<PhysicsComponent>();
	physics->friction = FRICTION_NORMAL;
	physics->maxWalkSpeed = 0.0f;
	physics->walkingAcceleration = 0.0f;
	physics->SetBoundsRadius(0.3f);
	
	if (spawnArgs->addPushForce)
	{
		Vector3 pushDirection = GetDirectionFromYAxisOrientation(Rnd(RADIANS_0, RADIANS_360));
		float pushStrength = Rnd(4.0f, 10.0f);
		Force *pushForce = new Force(pushDirection, pushStrength, 0.8f);
		physics->forces.push_back(pushForce);
	}
	
	entity->Add<RemovePhysicsWhenStillComponent>();
	
	entity->Add<PositionComponent>();
	
	entity->Add<PositionOffsetComponent>();
	
	PowerUpComponent *powerUp = entity->Add<PowerUpComponent>();
	powerUp->powerUp = spawnArgs->powerUp;
	powerUp->shouldRaisePowerUpOnCollect = spawnArgs->shouldRaisePowerUpOnCollect;
	
	entity->Add<BlobShadowComponent>();
	
	DespawnTimerComponent *despawnTimer = entity->Add<DespawnTimerComponent>();
	despawnTimer->timeLeft = 20.0f;
	despawnTimer->startFlickeringAtTimeLeft = 5.0f;
	
	entity->Add<ColorComponent>();
		
	// let the power up class do it's own specific creation stuff
	spawnArgs->powerUp->OnEntityCreation(entity);
	
	return entity;
}

BOOL PowerUpPreset::Handle(const Event *event)
{
	STACK_TRACE;
	if (event->Is<PowerUpCollectedEvent>())
	{
		const PowerUpCollectedEvent *e = event->As<PowerUpCollectedEvent>();
		if (e->GetEntity()->WasCreatedUsingPreset(GetType()))
			OnPowerUpCollected(e->GetEntity(), e->powerUpEntity);
	}
	
	return FALSE;
}
