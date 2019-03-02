#ifndef __GAME_ENTITIES_EVENTS_BUFFADDEDEVENT_H_INCLUDED__
#define __GAME_ENTITIES_EVENTS_BUFFADDEDEVENT_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../events/event.h"
#include "entityevent.h"
#include "../buffs/buff.h"

class Entity;

struct BuffAddedEvent : public EntityEvent
{
	static EVENT_TYPE GetType()
	{
		static EVENT_TYPE typeName = "BuffAddedEvent";
		return typeName;
	}
	EVENT_TYPE GetTypeOf() const                           { return GetType(); }
	
	BuffAddedEvent(Entity *entity);
	
	BUFF_TYPE type;
};

inline BuffAddedEvent::BuffAddedEvent(Entity *entity)
	: EntityEvent(entity)
{
	type = NULL;
}

#endif

