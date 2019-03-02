#include "../../../framework/debug.h"

#include "basecharacterentitypreset.h"
#include "particles/smokeparticlepreset.h"
#include "../components/boundingspherecomponent.h"
#include "../components/positioncomponent.h"
#include "../events/despawnedevent.h"
#include "../events/healevent.h"
#include "../events/hurtevent.h"
#include "../events/killedevent.h"
#include "../events/spawnedevent.h"
#include "../events/spawnparticlesevent.h"
#include "../../../entities/entity.h"
#include "../../../entities/entitymanager.h"
#include "../../../events/event.h"
#include "../../../events/eventlistenerex.h"
#include "../../../events/eventmanager.h"
#include "../../../framework/graphics/color.h"

BaseCharacterEntityPreset::BaseCharacterEntityPreset(EntityManager *entityManager)
	: EntityPreset(entityManager),
	  EventListenerEx(entityManager->GetEventManager())
{
	STACK_TRACE;
	ListenFor<DespawnedEvent>();
	ListenFor<HealEvent>();
	ListenFor<HurtEvent>();
	ListenFor<KilledEvent>();
	ListenFor<SpawnedEvent>();
}

BaseCharacterEntityPreset::~BaseCharacterEntityPreset()
{
	STACK_TRACE;
	StopListeningFor<DespawnedEvent>();
	StopListeningFor<HealEvent>();
	StopListeningFor<HurtEvent>();
	StopListeningFor<KilledEvent>();
	StopListeningFor<SpawnedEvent>();
}

BOOL BaseCharacterEntityPreset::Handle(const Event *event)
{
	STACK_TRACE;
	if (event->Is<SpawnedEvent>())
	{
		const SpawnedEvent *e = event->As<SpawnedEvent>();
		if (e->GetEntity()->WasCreatedUsingPreset(GetTypeOf()))
			OnSpawn(e->GetEntity());
	}
	else if (event->Is<DespawnedEvent>())
	{
		const DespawnedEvent *e = event->As<DespawnedEvent>();
		if (e->GetEntity()->WasCreatedUsingPreset(GetTypeOf()))
			OnDespawn(e->GetEntity());
	}
	else if (event->Is<HealEvent>())
	{
		const HealEvent *e = event->As<HealEvent>();
		if (e->GetEntity()->WasCreatedUsingPreset(GetTypeOf()))
			OnHeal(e->GetEntity(), e->source);
	}
	else if (event->Is<HurtEvent>())
	{
		const HurtEvent *e = event->As<HurtEvent>();
		if (e->GetEntity()->WasCreatedUsingPreset(GetTypeOf()))
			OnHurt(e->GetEntity(), e->source);
	}
	else if (event->Is<KilledEvent>())
	{
		const KilledEvent *e = event->As<KilledEvent>();
		if (e->GetEntity()->WasCreatedUsingPreset(GetTypeOf()))
			OnKilled(e->GetEntity(), e->source);
	}
	
	return FALSE;
}

void BaseCharacterEntityPreset::SpawnGreenSmokeAt(Entity *entity)
{
	STACK_TRACE;
	PositionComponent *position = entity->Get<PositionComponent>();
	BoundingSphereComponent *bounds = entity->Get<BoundingSphereComponent>();
	
	SpawnParticlesEvent particlesEvent;
	particlesEvent.particleInfo.particlePreset = SmokeParticlePreset::GetType();
	particlesEvent.particleInfo.position = position->position;
	particlesEvent.particleInfo.spawnInRadiusAroundPosition = TRUE;
	particlesEvent.particleInfo.radius = bounds->bounds.radius;
	particlesEvent.particleInfo.minimumAmount = 30;
	particlesEvent.particleInfo.maximumAmount = 30;
	particlesEvent.particleInfo.lowColor = Color(0.0f, 0.4f, 0.0f);
	particlesEvent.particleInfo.highColor = Color(0.0f, 0.7f, 0.0f);
	GetEventManager()->Trigger(&particlesEvent);
}

void BaseCharacterEntityPreset::SpawnPuffOfSmokeAt(Entity *entity)
{
	STACK_TRACE;
	PositionComponent *position = entity->Get<PositionComponent>();
	BoundingSphereComponent *bounds = entity->Get<BoundingSphereComponent>();
	
	SpawnParticlesEvent particlesEvent;
	particlesEvent.particleInfo.particlePreset = SmokeParticlePreset::GetType();
	particlesEvent.particleInfo.position = position->position;
	particlesEvent.particleInfo.spawnInRadiusAroundPosition = TRUE;
	particlesEvent.particleInfo.radius = bounds->bounds.radius;
	particlesEvent.particleInfo.minimumAmount = 30;
	particlesEvent.particleInfo.maximumAmount = 30;
	GetEventManager()->Trigger(&particlesEvent);
}
