#ifndef __GAME_ENTITIES_PRESETS_MONSTERS_PURPLEZOMBIEPRESET_H_INCLUDED__
#define __GAME_ENTITIES_PRESETS_MONSTERS_PURPLEZOMBIEPRESET_H_INCLUDED__

#include "../../../../framework/common.h"
#include "../../../../entities/entitypreset.h"
#include "zombiepreset.h"

class Entity;
class EntityManager;

class PurpleZombiePreset : public ZombiePreset
{
public:
	static ENTITYPRESET_TYPE GetType()
	{
		static ENTITYPRESET_TYPE typeName = "PurpleZombiePreset";
		return typeName;
	}
	ENTITYPRESET_TYPE GetTypeOf() const                    { return GetType(); }
	
	PurpleZombiePreset(EntityManager *entityManager);
	virtual ~PurpleZombiePreset();
	
	Entity* Create(EntityPresetArgs *args = NULL);
	
	void OnKilled(Entity *entity, Entity *killer);
};

#endif
