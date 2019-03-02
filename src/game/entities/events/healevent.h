#ifndef __GAME_ENTITIES_EVENTS_HEALEVENT_H_INCLUDED__
#define __GAME_ENTITIES_EVENTS_HEALEVENT_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../events/event.h"
#include "entityevent.h"

class Entity;

struct HealEvent : public EntityEvent
{
	static EVENT_TYPE GetType()
	{
		static EVENT_TYPE typeName = "HealEvent";
		return typeName;
	}
	EVENT_TYPE GetTypeOf() const                           { return GetType(); }

	HealEvent(Entity *entity);

	BOOL usePercent;
	float percent;
	uint32_t amount;
	Entity *source;    // NOTE: this should *always* be validated before using!
	                   //       via EntityManager::IsValid()
};

inline HealEvent::HealEvent(Entity *entity)
	: EntityEvent(entity)
{
	usePercent = FALSE;
	percent = 0.0f;
	amount = 0;
	source = NULL;
}

#endif

