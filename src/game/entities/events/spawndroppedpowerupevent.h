#ifndef __GAME_ENTITIES_EVENTS_SPAWNDROPPEDPOWERUPEVENT_H_INCLUDED__
#define __GAME_ENTITIES_EVENTS_SPAWNDROPPEDPOWERUPEVENT_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../events/event.h"
#include "../../../framework/math/vector3.h"
#include "../powerups/powerup.h"
#include <stl/string.h>

struct SpawnDroppedPowerUpEvent : public Event
{
	static EVENT_TYPE GetType()
	{
		static EVENT_TYPE typeName = "SpawnDroppedPowerUpEvent";
		return typeName;
	}
	EVENT_TYPE GetTypeOf() const                           { return GetType(); }
	
	SpawnDroppedPowerUpEvent();
	
	POWERUP_TYPE powerUpName;
	Vector3 position;
};

inline SpawnDroppedPowerUpEvent::SpawnDroppedPowerUpEvent()
{
	powerUpName = NULL;
	position = ZERO_VECTOR;
}

#endif
