#ifndef __GAME_ENTITIES_PRESETS_POWERUPS_POWERUPPRESET_H_INCLUDED__
#define __GAME_ENTITIES_PRESETS_POWERUPS_POWERUPPRESET_H_INCLUDED__

#include "../../../../framework/common.h"
#include "../../../../entities/entitypreset.h"
#include "../../../../events/eventlistenerex.h"

class Entity;
class EntityManager;
struct Event;

class PowerUpPreset : public EntityPreset, public EventListenerEx
{
public:
	static ENTITYPRESET_TYPE GetType()
	{
		static ENTITYPRESET_TYPE typeName = "PowerUpPreset";
		return typeName;
	}
	ENTITYPRESET_TYPE GetTypeOf() const                    { return GetType(); }

	PowerUpPreset(EntityManager *entityManager);
	virtual ~PowerUpPreset();

	Entity* Create(EntityPresetArgs *args = NULL);
	
	BOOL Handle(const Event *event);
	
	virtual void OnPowerUpCollected(Entity *collector, Entity *powerUp) {}
};

#endif
