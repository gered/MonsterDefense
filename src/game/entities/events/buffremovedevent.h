#ifndef __GAME_ENTITIES_EVENTS_BUFFREMOVEDEVENT_H_INCLUDED__
#define __GAME_ENTITIES_EVENTS_BUFFREMOVEDEVENT_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../events/event.h"
#include "entityevent.h"
#include "../buffs/buff.h"

class Entity;

struct BuffRemovedEvent : public EntityEvent
{
	static EVENT_TYPE GetType()
	{
		static EVENT_TYPE typeName = "BuffRemovedEvent";
		return typeName;
	}
	EVENT_TYPE GetTypeOf() const                           { return GetType(); }
	
	BuffRemovedEvent(Entity *entity);
	
	BUFF_TYPE type;
};

inline BuffRemovedEvent::BuffRemovedEvent(Entity *entity)
	: EntityEvent(entity)
{
	type = NULL;
}

#endif

