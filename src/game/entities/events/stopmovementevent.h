#ifndef __GAME_ENTITIES_EVENTS_STOPMOVEMENTEVENT_H_INCLUDED__
#define __GAME_ENTITIES_EVENTS_STOPMOVEMENTEVENT_H_INCLUDED__

#include "../../../events/event.h"
#include "entityevent.h"

class Entity;

struct StopMovementEvent : public EntityEvent
{
	static EVENT_TYPE GetType()
	{
		static EVENT_TYPE typeName = "StopMovementEvent";
		return typeName;
	}
	EVENT_TYPE GetTypeOf() const                           { return GetType(); }

	StopMovementEvent(Entity *entity);
};

inline StopMovementEvent::StopMovementEvent(Entity *entity)
	: EntityEvent(entity)
{
}

#endif

