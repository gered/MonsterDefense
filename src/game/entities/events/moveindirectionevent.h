#ifndef __GAME_ENTITIES_EVENTS_MOVEINDIRECTIONEVENT_H_INCLUDED__
#define __GAME_ENTITIES_EVENTS_MOVEINDIRECTIONEVENT_H_INCLUDED__

#include "../../../events/event.h"
#include "entityevent.h"
#include "../../../framework/math/vector3.h"

class Entity;

struct MoveInDirectionEvent : public EntityEvent
{
	static EVENT_TYPE GetType()
	{
		static EVENT_TYPE typeName = "MoveInDirectionEvent";
		return typeName;
	}
	EVENT_TYPE GetTypeOf() const                           { return GetType(); }

	MoveInDirectionEvent(Entity *entity);

	Vector3 direction;
};

inline MoveInDirectionEvent::MoveInDirectionEvent(Entity *entity)
	: EntityEvent(entity)
{
	direction = ZERO_VECTOR;
}

#endif

