#include "../../../framework/debug.h"

#include "../../../framework/common.h"
#include "lifeanddeathsystem.h"
#include "../components/boundingspherecomponent.h"
#include "../components/chestopenercomponent.h"
#include "../components/colorcomponent.h"
#include "../components/combatmodifierscomponent.h"
#include "../components/deadcomponent.h"
#include "../components/despawntimercomponent.h"
#include "../components/flickercomponent.h"
#include "../components/hurtcooldowncomponent.h"
#include "../components/lifecomponent.h"
#include "../components/npccomponent.h"
#include "../components/positioncomponent.h"
#include "../components/powerupcollectorcomponent.h"
#include "../globalcomponents/monsterpowercomponent.h"
#include "../events/despawnedevent.h"
#include "../events/entitystatechangeevent.h"
#include "../events/healevent.h"
#include "../events/hurtevent.h"
#include "../events/killedevent.h"
#include "../events/resetdespawntimerevent.h"
#include "../events/spawnparticlesevent.h"
#include "../events/stopflickeringevent.h"
#include "../../../entities/entity.h"
#include "../../../entities/entitymanager.h"
#include "../../../events/event.h"
#include "../../../events/eventmanager.h"

LifeAndDeathSystem::LifeAndDeathSystem(EntityManager *entityManager, EventManager *eventManager)
	: ComponentSystem(entityManager, eventManager)
{
	STACK_TRACE;
	ListenFor<HealEvent>();
	ListenFor<HurtEvent>();
	ListenFor<KilledEvent>();
	ListenFor<ResetDespawnTimerEvent>();
}

LifeAndDeathSystem::~LifeAndDeathSystem()
{
	STACK_TRACE;
	StopListeningFor<HealEvent>();
	StopListeningFor<HurtEvent>();
	StopListeningFor<KilledEvent>();
	StopListeningFor<ResetDespawnTimerEvent>();
}

void LifeAndDeathSystem::OnUpdate(float delta)
{
	STACK_TRACE;
	EntityList list;
	
	// separate check for DeadComponents allows us to have entity's with
	// a DeadComponent but without a LifeComponent (might be handy ... ?)
	GetEntityManager()->GetAllWith<DeadComponent>(list);
	for (EntityList::iterator i = list.begin(); i != list.end(); ++i)
	{
		Entity *entity = *i;
		DeadComponent *dead = entity->Get<DeadComponent>();
		dead->time -= delta;
		if (dead->time <= 0.0f)
		{
			// ?? don't think we use this anywhere ... ?
			EntityStateChangeEvent stateChangeEvent(entity);
			stateChangeEvent.state = ENTITYSTATE_INACTIVE;
			GetEventManager()->Trigger(&stateChangeEvent);
			
			DespawnedEvent despawnedEvent(entity);
			GetEventManager()->Trigger(&despawnedEvent);
		}
	}

	list.clear();
	GetEntityManager()->GetAllWith<LifeComponent>(list);
	for (EntityList::iterator i = list.begin(); i != list.end(); ++i)
	{
		Entity *entity = *i;
		LifeComponent *life = entity->Get<LifeComponent>();
		if (life->health == 0 && !entity->Has<DeadComponent>())
		{
			DeadComponent *dead = entity->Add<DeadComponent>();
			dead->time = 2.0f;

			KilledEvent killedEvent(entity);
			killedEvent.source = life->lastSourceOfDamage;
			EntityStateChangeEvent stateChangeEvent(entity);
			stateChangeEvent.state = ENTITYSTATE_DEAD;
			stateChangeEvent.clearExistingTempStateInfo = TRUE;

			GetEventManager()->Trigger(&killedEvent);
			GetEventManager()->Trigger(&stateChangeEvent);
		}
	}

	list.clear();
	GetEntityManager()->GetAllWith<HurtCooldownComponent>(list);
	for (EntityList::iterator i = list.begin(); i != list.end(); ++i)
	{
		Entity *entity = *i;
		HurtCooldownComponent *hurtCooldown = entity->Get<HurtCooldownComponent>();
		hurtCooldown->time -= delta;

		if (hurtCooldown->time <= 0.0f)
			entity->Remove<HurtCooldownComponent>();
	}
	
	list.clear();
	GetEntityManager()->GetAllWith<DespawnTimerComponent>(list);
	for (EntityList::iterator i = list.begin(); i != list.end(); ++i)
	{
		Entity *entity = *i;
		DespawnTimerComponent *despawnTimer = entity->Get<DespawnTimerComponent>();
		
		despawnTimer->timeLeft -= delta;
		if (despawnTimer->timeLeft <= 0.0f)
		{
			if (despawnTimer->dontTriggerDespawnEvent == FALSE)
			{
				DespawnedEvent despawnedEvent(entity);
				GetEventManager()->Trigger(&despawnedEvent);
			}
		}
		else if (despawnTimer->timeLeft <= despawnTimer->startFlickeringAtTimeLeft)
		{
			// only start flickering if they aren't already flickering
			FlickerComponent *flicker = entity->Get<FlickerComponent>();
			if (flicker == NULL)
			{
				ColorComponent *color = entity->Get<ColorComponent>();
				ASSERT(color != NULL);
				
				Color flickerColor(color->color.r, color->color.g, color->color.b, 0.3f);
				flicker = entity->Add<FlickerComponent>();
				flicker->Set(flickerColor, color->color);
			}
		}
	}
}

BOOL LifeAndDeathSystem::Handle(const Event *event)
{
	STACK_TRACE;
	if (event->Is<HealEvent>())
	{
		const HealEvent *e = event->As<HealEvent>();
		
		// heal entity
		LifeComponent *life = e->GetEntity()->Get<LifeComponent>();
		ASSERT(life != NULL);
		if (e->usePercent)
			life->Heal(e->percent);
		else
			life->Heal(e->amount);
		
		life->lastSourceOfHealing = e->source;
	}
	else if (event->Is<HurtEvent>())
	{
		const HurtEvent *e = event->As<HurtEvent>();
		
		// don't hurt again if the event isn't skipping this entity's hurt
		// cooldown timer and the entity still has a hurt cooldown timer
		if (!e->skipHurtCooldown && e->GetEntity()->Has<HurtCooldownComponent>())
			return FALSE;
		
		// apply defensive modifier (if present) on the entity being hurt to 
		// it's incoming damage
		CombatModifiersComponent *combatModifiers = e->GetEntity()->Get<CombatModifiersComponent>();
		float modifier = 1.0f;
		if (combatModifiers != NULL)
			modifier = combatModifiers->defenseModifier;

		if (e->GetEntity()->Has<NPCComponent>())
		{
			MonsterPowerComponent *monsterPower = GetEntityManager()->GetGlobalComponent<MonsterPowerComponent>();
			ASSERT(monsterPower != NULL);
			modifier += monsterPower->defenseModifier - 1.0f;
		}
		
		// hurt entity
		LifeComponent *life = e->GetEntity()->Get<LifeComponent>();
		ASSERT(life != NULL);
		if (e->usePercent)
			life->Hurt(e->percent, modifier);
		else
			life->Hurt(e->amount, modifier);
		
		life->lastSourceOfDamage = e->source;

		// flicker entity for an added visual cue that it was hurt
		ColorComponent *color = e->GetEntity()->Get<ColorComponent>();
		if (color != NULL)
		{
			FlickerComponent *flicker = e->GetEntity()->Get<FlickerComponent>();
			if (flicker == NULL)
				flicker = e->GetEntity()->Add<FlickerComponent>();
			else
			{
				// reset colorcomponent's color first so we don't lose the
				// original color below
				color->color = flicker->originalColor;
			}
			Color flickerColor(color->color.r, color->color.g, color->color.b, 0.3f);
			flicker->Set(flickerColor, color->color, 0.5f);
		}

		if (!e->skipHurtCooldown && !e->GetEntity()->Has<HurtCooldownComponent>())
		{
			HurtCooldownComponent *hurtCooldown = e->GetEntity()->Add<HurtCooldownComponent>();
			hurtCooldown->time = 0.5f;
		}
	}
	else if (event->Is<KilledEvent>())
	{
		const KilledEvent *e = event->As<KilledEvent>();
		
		// don't allow this dead entity to collect stuff while it's going
		// through it's death animation
		e->GetEntity()->Remove<PowerUpCollectorComponent>();
		e->GetEntity()->Remove<ChestOpenerComponent>();
	}
	else if (event->Is<ResetDespawnTimerEvent>())
	{
		const ResetDespawnTimerEvent *e = event->As<ResetDespawnTimerEvent>();
		DespawnTimerComponent *despawnTimer = e->GetEntity()->Get<DespawnTimerComponent>();
		ASSERT(despawnTimer != NULL);
		
		// first, check if the existing despawn timer is within the flicker
		// time, and if so, we should begin by removing the flicker component
		// before fiddling with the times
		if (despawnTimer->startFlickeringAtTimeLeft > despawnTimer->timeLeft)
		{
			StopFlickeringEvent stopFlicker(e->GetEntity());
			GetEventManager()->Trigger(&stopFlicker);
		}
		
		// now continue resetting the despawn timer...
		despawnTimer->timeLeft = e->newTimeTillDespawn;
		if (e->alsoResetFlickerTime)
			despawnTimer->startFlickeringAtTimeLeft = e->newTimeToFlickerAt;
		}

	return FALSE;
}

