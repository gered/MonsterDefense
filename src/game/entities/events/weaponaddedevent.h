#ifndef __GAME_ENTITIES_EVENTS_WEAPONADDEDEVENT_H_INCLUDED__
#define __GAME_ENTITIES_EVENTS_WEAPONADDEDEVENT_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../events/event.h"
#include "entityevent.h"
#include "../weapons/weapon.h"

class Entity;

struct WeaponAddedEvent : public EntityEvent
{
	static EVENT_TYPE GetType()
	{
		static EVENT_TYPE typeName = "WeaponAddedEvent";
		return typeName;
	}
	EVENT_TYPE GetTypeOf() const                           { return GetType(); }
	
	WeaponAddedEvent(Entity *entity);
	
	WEAPON_TYPE type;
};

inline WeaponAddedEvent::WeaponAddedEvent(Entity *entity)
	: EntityEvent(entity)
{
	type = NULL;
}

#endif

