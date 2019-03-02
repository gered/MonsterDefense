#ifndef __GAME_ENTITIES_PRESETS_OBJECTS_CHESTPRESET_H_INCLUDED__
#define __GAME_ENTITIES_PRESETS_OBJECTS_CHESTPRESET_H_INCLUDED__

#include "../../../../framework/common.h"
#include "../../../../entities/entitypreset.h"

class Entity;
class EntityManager;

class ChestPreset : public EntityPreset
{
public:
	static ENTITYPRESET_TYPE GetType()
	{
		static ENTITYPRESET_TYPE typeName = "ChestPreset";
		return typeName;
	}
	ENTITYPRESET_TYPE GetTypeOf() const                    { return GetType(); }

	ChestPreset(EntityManager *entityManager);
	virtual ~ChestPreset();

	Entity* Create(EntityPresetArgs *args = NULL);
};

#endif
