#ifndef __GAME_ENTITIES_EVENTS_ANIMATIONFINISHEDEVENT_H_INCLUDED__
#define __GAME_ENTITIES_EVENTS_ANIMATIONFINISHEDEVENT_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../events/event.h"
#include "entityevent.h"

struct AnimationFinishedEvent : public EntityEvent
{
	static EVENT_TYPE GetType()
	{
		static EVENT_TYPE typeName = "AnimationFinishedEvent";
		return typeName;
	}
	EVENT_TYPE GetTypeOf() const                           { return GetType(); }
	
	AnimationFinishedEvent(Entity *entity);
};

inline AnimationFinishedEvent::AnimationFinishedEvent(Entity *entity)
: EntityEvent(entity)
{
}

#endif

