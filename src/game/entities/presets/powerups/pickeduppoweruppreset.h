#ifndef __GAME_ENTITIES_PRESETS_POWERUPS_PICKEDUPPOWERUPPRESET_H_INCLUDED__
#define __GAME_ENTITIES_PRESETS_POWERUPS_PICKEDUPPOWERUPPRESET_H_INCLUDED__

#include "../../../../framework/common.h"
#include "../../../../entities/entitypreset.h"

class Entity;
class EntityManager;

class PickedUpPowerUpPreset : public EntityPreset
{
public:
	static ENTITYPRESET_TYPE GetType()
	{
		static ENTITYPRESET_TYPE typeName = "PickedUpPowerUpPreset";
		return typeName;
	}
	ENTITYPRESET_TYPE GetTypeOf() const                    { return GetType(); }

	PickedUpPowerUpPreset(EntityManager *entityManager);
	virtual ~PickedUpPowerUpPreset();
	
	Entity* Create(EntityPresetArgs *args = NULL);
};

#endif
