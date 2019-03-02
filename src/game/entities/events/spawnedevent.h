#ifndef __GAME_ENTITIES_EVENTS_SPAWNEDEVENT_H_INCLUDED__
#define __GAME_ENTITIES_EVENTS_SPAWNEDEVENT_H_INCLUDED__

#include "../../../events/event.h"
#include "entityevent.h"

class Entity;

struct SpawnedEvent : public EntityEvent
{
	static EVENT_TYPE GetType()
	{
		static EVENT_TYPE typeName = "SpawnedEvent";
		return typeName;
	}
	EVENT_TYPE GetTypeOf() const                           { return GetType(); }
	
	SpawnedEvent(Entity *entity);
};

inline SpawnedEvent::SpawnedEvent(Entity *entity)
: EntityEvent(entity)
{
}

#endif

