#include "../../../framework/debug.h"
#include "../../../framework/common.h"

#include "weaponsystem.h"
#include "../entitystates.h"
#include "../components/weaponcomponent.h"
#include "../components/weaponlistcomponent.h"
#include "../globalcomponents/weaponcollectioncomponent.h"
#include "../events/weaponaddedevent.h"
#include "../events/weaponswitchevent.h"
#include "../weapons/weapon.h"
#include "../weapons/weaponcollection.h"
#include "../../../entities/entity.h"
#include "../../../entities/entitymanager.h"
#include "../../../events/event.h"
#include "../../../events/eventmanager.h"
#include <stl/vector.h>

WeaponSystem::WeaponSystem(EntityManager *entityManager, EventManager *eventManager)
: ComponentSystem(entityManager, eventManager)
{
	STACK_TRACE;
	ListenFor<WeaponSwitchEvent>();
	ListenFor<WeaponAddedEvent>();
}

WeaponSystem::~WeaponSystem()
{
	STACK_TRACE;
	StopListeningFor<WeaponSwitchEvent>();
	StopListeningFor<WeaponAddedEvent>();
}

void WeaponSystem::OnUpdate(float delta)
{
	STACK_TRACE;
}

BOOL WeaponSystem::Handle(const Event *event)
{
	STACK_TRACE;
	if (event->Is<WeaponSwitchEvent>())
	{
		const WeaponSwitchEvent *e = event->As<WeaponSwitchEvent>();
		
		WeaponComponent *currentWeapon = e->GetEntity()->Get<WeaponComponent>();
		ASSERT(currentWeapon != NULL);
		WeaponListComponent *weaponList = e->GetEntity()->Get<WeaponListComponent>();
		ASSERT(weaponList != NULL);
		
		int32_t index;
		
		if (e->switchUsingType)
		{
			index = -1;
			
			// find the weapon index associated with this type from the entity's
			// current list of weapons
			for (uint32_t i = 0; i < weaponList->weapons.size(); ++i)
			{
				Weapon *weapon = weaponList->weapons[i];
				if (weapon != NULL && weapon->Is(e->switchToType))
				{
					index = i;
					break;
				}
			}
			
			// if we didn't find a match, stop
			ASSERT(index != -1);
			if (index == -1)
				return TRUE;
		}
		else
		{
			// out of bounds check and NULL checks...
			ASSERT(e->switchToIndex < weaponList->weapons.size());
			if (e->switchToIndex >= weaponList->weapons.size())
				return TRUE;
			else if (weaponList->weapons[e->switchToIndex] == NULL)
				return TRUE;
			
			// safe to switch to this index
			index = e->switchToIndex;
		}
		
		// switch the current weapon to whatever the weapon is at that index...
		currentWeapon->weapon = weaponList->weapons[index];
	}
	else if (event->Is<WeaponAddedEvent>())
	{
		const WeaponAddedEvent *e = event->As<WeaponAddedEvent>();
		
		WeaponListComponent *weaponList = e->GetEntity()->Get<WeaponListComponent>();
		ASSERT(weaponList != NULL);
		
		// is this weapon we're adding already in the list?
		for (uint32_t i = 0; i < weaponList->weapons.size(); ++i)
		{
			Weapon *weapon = weaponList->weapons[i];
			if (weapon != NULL && weapon->Is(e->type))
			{
				// this entity has the weapon already, don't add anything
				return TRUE;
			}
		}
		
		// if we get here, we can safely add this new weapon
		WeaponCollectionComponent *weapons = GetEntityManager()->GetGlobalComponent<WeaponCollectionComponent>();
		ASSERT(weapons != NULL);
		
		Weapon *weapon = weapons->weapons->Get(e->type);
		ASSERT(weapon != NULL);
		
		weaponList->weapons.push_back(weapon);
	}
	
	return FALSE;
}
