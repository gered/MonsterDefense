#ifndef __GAME_ENTITIES_EVENTS_SPAWNPARTICLESEVENT_H_INCLUDED__
#define __GAME_ENTITIES_EVENTS_SPAWNPARTICLESEVENT_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../events/event.h"
#include "../spawnparticleinfo.h"

struct SpawnParticlesEvent : public Event
{
	static EVENT_TYPE GetType()
	{
		static EVENT_TYPE typeName = "SpawnParticlesEvent";
		return typeName;
	}
	EVENT_TYPE GetTypeOf() const                           { return GetType(); }

	SpawnParticlesEvent();
	
	SpawnParticleInfo particleInfo;
};

inline SpawnParticlesEvent::SpawnParticlesEvent()
{
}

#endif
