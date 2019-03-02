#ifndef __GAME_ENTITIES_EVENTS_ENTITYSTATECHANGEEVENT_H_INCLUDED__
#define __GAME_ENTITIES_EVENTS_ENTITYSTATECHANGEEVENT_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../events/event.h"
#include "entityevent.h"
#include "../entitystates.h"

struct EntityStateChangeEvent : public EntityEvent
{
	static EVENT_TYPE GetType()
	{
		static EVENT_TYPE typeName = "EntityStateChangeEvent";
		return typeName;
	}
	EVENT_TYPE GetTypeOf() const                           { return GetType(); }

	EntityStateChangeEvent(Entity *entity);

	ENTITYSTATES state;
	BOOL isTempChange;
	BOOL clearExistingTempStateInfo;
};

inline EntityStateChangeEvent::EntityStateChangeEvent(Entity *entity)
	: EntityEvent(entity)
{
	state = ENTITYSTATE_NONE;
	isTempChange = FALSE;
	clearExistingTempStateInfo = FALSE;
}

#endif

