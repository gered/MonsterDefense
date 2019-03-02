#ifndef __GAME_ENTITIES_EVENTS_JUMPEVENT_H_INCLUDED__
#define __GAME_ENTITIES_EVENTS_JUMPEVENT_H_INCLUDED__

#include "../../../events/event.h"
#include "entityevent.h"

class Entity;

struct JumpEvent : public EntityEvent
{
	static EVENT_TYPE GetType()
	{
		static EVENT_TYPE typeName = "JumpEvent";
		return typeName;
	}
	EVENT_TYPE GetTypeOf() const                           { return GetType(); }

	JumpEvent(Entity *entity);
};

inline JumpEvent::JumpEvent(Entity *entity)
	: EntityEvent(entity)
{
}

#endif

