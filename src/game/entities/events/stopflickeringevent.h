#ifndef __GAME_ENTITIES_EVENTS_STOPFLICKERINGEVENT_H_INCLUDED__
#define __GAME_ENTITIES_EVENTS_STOPFLICKERINGEVENT_H_INCLUDED__

#include "../../../events/event.h"
#include "entityevent.h"

class Entity;

struct StopFlickeringEvent : public EntityEvent
{
	static EVENT_TYPE GetType()
	{
		static EVENT_TYPE typeName = "StopFlickeringEvent";
		return typeName;
	}
	EVENT_TYPE GetTypeOf() const                           { return GetType(); }
	
	StopFlickeringEvent(Entity *entity);
};

inline StopFlickeringEvent::StopFlickeringEvent(Entity *entity)
: EntityEvent(entity)
{
}

#endif

