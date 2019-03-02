#ifndef __GAME_ENTITIES_EVENTS_MOVEFORWARDEVENT_H_INCLUDED__
#define __GAME_ENTITIES_EVENTS_MOVEFORWARDEVENT_H_INCLUDED__

#include "../../../events/event.h"
#include "entityevent.h"

class Entity;

struct MoveForwardEvent : public EntityEvent
{
	static EVENT_TYPE GetType()
	{
		static EVENT_TYPE typeName = "MoveForwardEvent";
		return typeName;
	}
	EVENT_TYPE GetTypeOf() const                           { return GetType(); }

	MoveForwardEvent(Entity *entity);
};

inline MoveForwardEvent::MoveForwardEvent(Entity *entity)
	: EntityEvent(entity)
{
}

#endif

