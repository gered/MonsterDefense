#ifndef __GAME_ENTITIES_EVENTS_POWERUPCOLLECTEDEVENT_H_INCLUDED__
#define __GAME_ENTITIES_EVENTS_POWERUPCOLLECTEDEVENT_H_INCLUDED__

#include "../../../framework/common.h"
#include "entityevent.h"
#include "../../../events/event.h"

class Entity;

struct PowerUpCollectedEvent : public EntityEvent
{
	static EVENT_TYPE GetType()
	{
		static EVENT_TYPE typeName = "PowerUpCollectedEvent";
		return typeName;
	}
	EVENT_TYPE GetTypeOf() const                           { return GetType(); }

	PowerUpCollectedEvent(Entity *entity, Entity *powerUp);

	BOOL powerUpEntityIsChestEntity;
	Entity *powerUpEntity;
};

inline PowerUpCollectedEvent::PowerUpCollectedEvent(Entity *entity, Entity *powerUp)
	: EntityEvent(entity)
{
	powerUpEntityIsChestEntity = FALSE;
	powerUpEntity = powerUp;
}

#endif

