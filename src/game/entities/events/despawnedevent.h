#ifndef __GAME_ENTITIES_EVENTS_DESPAWNEDEVENT_H_INCLUDED__
#define __GAME_ENTITIES_EVENTS_DESPAWNEDEVENT_H_INCLUDED__

#include "../../../events/event.h"
#include "entityevent.h"

class Entity;

struct DespawnedEvent : public EntityEvent
{
	static EVENT_TYPE GetType()
	{
		static EVENT_TYPE typeName = "DespawnedEvent";
		return typeName;
	}
	EVENT_TYPE GetTypeOf() const                           { return GetType(); }
	
	DespawnedEvent(Entity *entity);
};

inline DespawnedEvent::DespawnedEvent(Entity *entity)
: EntityEvent(entity)
{
}

#endif

