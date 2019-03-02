#ifndef __GAME_ENTITIES_EVENTS_ENTITYEXITINGTEMPSTATEEVENT_H_INCLUDED__
#define __GAME_ENTITIES_EVENTS_ENTITYEXITINGTEMPSTATEEVENT_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../events/event.h"
#include "entityevent.h"

struct EntityExitingTempStateEvent : public EntityEvent
{
	static EVENT_TYPE GetType()
	{
		static EVENT_TYPE typeName = "EntityExitingTempStateEvent";
		return typeName;
	}
	EVENT_TYPE GetTypeOf() const                           { return GetType(); }
	
	EntityExitingTempStateEvent(Entity *entity);
};

inline EntityExitingTempStateEvent::EntityExitingTempStateEvent(Entity *entity)
: EntityEvent(entity)
{
}

#endif

