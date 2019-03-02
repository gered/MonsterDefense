#ifndef __GAME_ENTITIES_EVENTS_BULLETCOLLISIONEVENT_H_INCLUDED__
#define __GAME_ENTITIES_EVENTS_BULLETCOLLISIONEVENT_H_INCLUDED__

#include "../../../framework/common.h"
#include "entityevent.h"
#include "../../../events/event.h"

class Entity;

struct BulletCollisionEvent : public EntityEvent
{
	static EVENT_TYPE GetType()
	{
		static EVENT_TYPE typeName = "BulletCollisionEvent";
		return typeName;
	}
	EVENT_TYPE GetTypeOf() const                           { return GetType(); }

	BulletCollisionEvent(Entity *entity);
	
	Entity *collisionEntity;
};

inline BulletCollisionEvent::BulletCollisionEvent(Entity *entity)
	: EntityEvent(entity)
{
	collisionEntity = NULL;
}

#endif
