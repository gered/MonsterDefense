#ifndef __GAME_ENTITIES_EVENTS_SCOREEVENT_H_INCLUDED__
#define __GAME_ENTITIES_EVENTS_SCOREEVENT_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../events/event.h"
#include "entityevent.h"
#include "../../../framework/math/vector3.h"

class Entity;

struct ScoreEvent : public EntityEvent
{
	static EVENT_TYPE GetType()
	{
		static EVENT_TYPE typeName = "ScoreEvent";
		return typeName;
	}
	EVENT_TYPE GetTypeOf() const                           { return GetType(); }
	
	ScoreEvent(Entity *entity);
	
	int32_t scoreAwarded;
	BOOL hasSourcePosition;
	Vector3 sourcePosition;
};

inline ScoreEvent::ScoreEvent(Entity *entity)
: EntityEvent(entity)
{
	scoreAwarded = 0;
	hasSourcePosition = FALSE;
	sourcePosition = ZERO_VECTOR;
}

#endif

