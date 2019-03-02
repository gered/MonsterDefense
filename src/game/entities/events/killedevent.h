#ifndef __GAME_ENTITIES_EVENTS_KILLEDEVENT_H_INCLUDED__
#define __GAME_ENTITIES_EVENTS_KILLEDEVENT_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../events/event.h"
#include "entityevent.h"

class Entity;

struct KilledEvent : public EntityEvent
{
	static EVENT_TYPE GetType()
	{
		static EVENT_TYPE typeName = "KilledEvent";
		return typeName;
	}
	EVENT_TYPE GetTypeOf() const                           { return GetType(); }

	KilledEvent(Entity *entity);
	
	Entity *source;    // NOTE: this must *always* be validated before using!
	                   //       via EntityManager::IsValid()
};

inline KilledEvent::KilledEvent(Entity *entity)
	: EntityEvent(entity)
{
	source = NULL;
}

#endif

