#ifndef __GAME_ENTITIES_EVENTS_ATTACKEVENT_H_INCLUDED__
#define __GAME_ENTITIES_EVENTS_ATTACKEVENT_H_INCLUDED__

#include "../../../events/event.h"
#include "entityevent.h"

class Entity;

struct AttackEvent : public EntityEvent
{
	static EVENT_TYPE GetType()
	{
		static EVENT_TYPE typeName = "AttackEvent";
		return typeName;
	}
	EVENT_TYPE GetTypeOf() const                           { return GetType(); }
	
	AttackEvent(Entity *entity);
};

inline AttackEvent::AttackEvent(Entity *entity)
: EntityEvent(entity)
{
}

#endif

