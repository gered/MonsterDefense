#ifndef __GAME_ENTITIES_EVENTS_RESETDESPAWNTIMEREVENT_H_INCLUDED__
#define __GAME_ENTITIES_EVENTS_RESETDESPAWNTIMEREVENT_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../events/event.h"
#include "entityevent.h"

class Entity;

struct ResetDespawnTimerEvent : public EntityEvent
{
	static EVENT_TYPE GetType()
	{
		static EVENT_TYPE typeName = "ResetDespawnTimerEvent";
		return typeName;
	}
	EVENT_TYPE GetTypeOf() const                           { return GetType(); }
	
	ResetDespawnTimerEvent(Entity *entity);
	
	float newTimeTillDespawn;
	BOOL alsoResetFlickerTime;
	float newTimeToFlickerAt;
};

inline ResetDespawnTimerEvent::ResetDespawnTimerEvent(Entity *entity)
: EntityEvent(entity)
{
	newTimeTillDespawn = 0.0f;
	alsoResetFlickerTime = FALSE;
	newTimeToFlickerAt = 0.0f;
}

#endif

