#ifndef __GAME_ENTITIES_EVENTS_HURTEVENT_H_INCLUDED__
#define __GAME_ENTITIES_EVENTS_HURTEVENT_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../events/event.h"
#include "entityevent.h"

class Entity;

struct HurtEvent : public EntityEvent
{
	static EVENT_TYPE GetType()
	{
		static EVENT_TYPE typeName = "HurtEvent";
		return typeName;
	}
	EVENT_TYPE GetTypeOf() const                           { return GetType(); }

	HurtEvent(Entity *entity);

	BOOL usePercent;
	float percent;
	uint32_t amount;
	BOOL skipHurtCooldown;
	BOOL skipDefenseModifiers;
	Entity *source;    // NOTE: this should *always* be validated before using!
	                   //       via EntityManager::IsValid()
};

inline HurtEvent::HurtEvent(Entity *entity)
	: EntityEvent(entity)
{
	usePercent = FALSE;
	percent = 0.0f;
	amount = 0;
	skipHurtCooldown = FALSE;
	skipDefenseModifiers = FALSE;
	source = NULL;
}

#endif

