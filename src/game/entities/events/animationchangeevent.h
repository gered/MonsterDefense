#ifndef __GAME_ENTITIES_EVENTS_ANIMATIONCHANGEEVENT_H_INCLUDED__
#define __GAME_ENTITIES_EVENTS_ANIMATIONCHANGEEVENT_H_INCLUDED__

#include "../../../framework/common.h"
#include "entityevent.h"
#include "../../../events/event.h"
#include "../entitystates.h"
#include <stl/string.h>

class Entity;

struct AnimationChangeEvent : public EntityEvent
{
	static EVENT_TYPE GetType()
	{
		static EVENT_TYPE typeName = "AnimationChangeEvent";
		return typeName;
	}
	EVENT_TYPE GetTypeOf() const                           { return GetType(); }
	
	AnimationChangeEvent(Entity *entity);

	stl::string sequenceName;
	BOOL changeToSequenceForEntityState;
	ENTITYSTATES state;
	BOOL loop;
	BOOL overrideExisting;
};

inline AnimationChangeEvent::AnimationChangeEvent(Entity *entity)
: EntityEvent(entity)
{
	changeToSequenceForEntityState = FALSE;
	state = ENTITYSTATE_NONE;
	loop = FALSE;
	overrideExisting = FALSE;
}

#endif

