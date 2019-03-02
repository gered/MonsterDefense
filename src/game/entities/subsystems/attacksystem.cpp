#include "../../../framework/debug.h"
#include "../../../framework/common.h"

#include "attacksystem.h"
#include "../entitystates.h"
#include "../components/attackcooldowncomponent.h"
#include "../components/canbeattackedcomponent.h"
#include "../components/combatmodifierscomponent.h"
#include "../components/weaponcomponent.h"
#include "../events/attackevent.h"
#include "../events/entitystatechangeevent.h"
#include "../events/killedevent.h"
#include "../weapons/weapon.h"
#include "../../../entities/entity.h"
#include "../../../entities/entitymanager.h"
#include "../../../events/event.h"
#include "../../../events/eventmanager.h"

AttackSystem::AttackSystem(EntityManager *entityManager, EventManager *eventManager)
: ComponentSystem(entityManager, eventManager)
{
	STACK_TRACE;
	ListenFor<AttackEvent>();
	ListenFor<KilledEvent>();
}

AttackSystem::~AttackSystem()
{
	STACK_TRACE;
	StopListeningFor<AttackEvent>();
	StopListeningFor<KilledEvent>();
}

void AttackSystem::OnUpdate(float delta)
{
	STACK_TRACE;
	EntityList list;
	GetEntityManager()->GetAllWith<AttackCooldownComponent>(list);
	for (EntityList::iterator i = list.begin(); i != list.end(); ++i)
	{
		Entity *entity = *i;
		AttackCooldownComponent *cooldown = entity->Get<AttackCooldownComponent>();
		cooldown->time -= delta;
		if (cooldown->time <= 0.0f)
			entity->Remove<AttackCooldownComponent>();
		
	}
}

BOOL AttackSystem::Handle(const Event *event)
{
	STACK_TRACE;
	if (event->Is<AttackEvent>())
	{
		const AttackEvent *e = event->As<AttackEvent>();
		
		// can't attack if an attack cooldown is currently active
		if (!e->GetEntity()->Has<AttackCooldownComponent>())
		{
			// also can't attack if the entity has no weapon
			WeaponComponent *weapon = e->GetEntity()->Get<WeaponComponent>();
			if (weapon != NULL && weapon->weapon != NULL)
			{
				// trigger state change to make the entity do an appropriate
				// attacking animation
				EntityStateChangeEvent *stateChangeEvent = new EntityStateChangeEvent(e->GetEntity());
				stateChangeEvent->state = (weapon->weapon->IsMelee() ? ENTITYSTATE_PUNCHING : ENTITYSTATE_SHOOTING);
				stateChangeEvent->isTempChange = TRUE;
				GetEventManager()->Queue(stateChangeEvent);
				
				// process weapon-specific attack handling
				// (this will spawn bullets, or whatever is needed)
				weapon->weapon->OnAttack(e->GetEntity());
				
				// add cooldown time
				AttackCooldownComponent *attackCooldown = e->GetEntity()->Add<AttackCooldownComponent>();
				attackCooldown->time = weapon->weapon->GetCooldownTime();
				
				CombatModifiersComponent *combatModifiers = e->GetEntity()->Get<CombatModifiersComponent>();
				if (combatModifiers != NULL)
					attackCooldown->time *= combatModifiers->attackCooldownModifier;
			}
		}
	}
	else if (event->Is<KilledEvent>())
	{
		const KilledEvent *e = event->As<KilledEvent>();
		e->GetEntity()->Remove<CanBeAttackedComponent>();
	}
	return FALSE;
}
