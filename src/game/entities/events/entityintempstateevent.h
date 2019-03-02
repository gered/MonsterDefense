#ifndef __GAME_ENTITIES_EVENTS_ENTITYINTEMPSTATEEVENT_H_INCLUDED__
#define __GAME_ENTITIES_EVENTS_ENTITYINTEMPSTATEEVENT_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../events/event.h"
#include "entityevent.h"

struct EntityInTempStateEvent : public EntityEvent
{
	static EVENT_TYPE GetType()
	{
		static EVENT_TYPE typeName = "EntityInTempStateEvent";
		return typeName;
	}
	EVENT_TYPE GetTypeOf() const                           { return GetType(); }
	
	EntityInTempStateEvent(Entity *entity);
};

inline EntityInTempStateEvent::EntityInTempStateEvent(Entity *entity)
: EntityEvent(entity)
{
}

#endif

