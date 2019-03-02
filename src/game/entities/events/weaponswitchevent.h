#ifndef __GAME_ENTITIES_EVENTS_WEAPONSWITCHEVENT_H_INCLUDED__
#define __GAME_ENTITIES_EVENTS_WEAPONSWITCHEVENT_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../events/event.h"
#include "entityevent.h"
#include "../weapons/weapon.h"

class Entity;

struct WeaponSwitchEvent : public EntityEvent
{
	static EVENT_TYPE GetType()
	{
		static EVENT_TYPE typeName = "WeaponSwitchEvent";
		return typeName;
	}
	EVENT_TYPE GetTypeOf() const                           { return GetType(); }
	
	WeaponSwitchEvent(Entity *entity);
	
	BOOL switchUsingType;
	WEAPON_TYPE switchToType;
	uint32_t switchToIndex;
};

inline WeaponSwitchEvent::WeaponSwitchEvent(Entity *entity)
: EntityEvent(entity)
{
	switchUsingType = FALSE;
	switchToType = NULL;
	switchToIndex = 0;
}

#endif

