#ifndef __GAME_ENTITIES_PRESETS_MONSTERS_BLUEZOMBIEPRESET_H_INCLUDED__
#define __GAME_ENTITIES_PRESETS_MONSTERS_BLUEZOMBIEPRESET_H_INCLUDED__

#include "../../../../framework/common.h"
#include "../../../../entities/entitypreset.h"
#include "zombiepreset.h"

class Entity;
class EntityManager;

class BlueZombiePreset : public ZombiePreset
{
public:
	static ENTITYPRESET_TYPE GetType()
	{
		static ENTITYPRESET_TYPE typeName = "BlueZombiePreset";
		return typeName;
	}
	ENTITYPRESET_TYPE GetTypeOf() const                    { return GetType(); }
	
	BlueZombiePreset(EntityManager *entityManager);
	virtual ~BlueZombiePreset();
	
	Entity* Create(EntityPresetArgs *args = NULL);
	
	void OnKilled(Entity *entity, Entity *killer);
};

#endif
