#ifndef __GAME_ENTITIES_PRESETS_MONSTERS_REDZOMBIEPRESET_H_INCLUDED__
#define __GAME_ENTITIES_PRESETS_MONSTERS_REDZOMBIEPRESET_H_INCLUDED__

#include "../../../../framework/common.h"
#include "../../../../entities/entitypreset.h"
#include "zombiepreset.h"

class Entity;
class EntityManager;

class RedZombiePreset : public ZombiePreset
{
public:
	static ENTITYPRESET_TYPE GetType()
	{
		static ENTITYPRESET_TYPE typeName = "RedZombiePreset";
		return typeName;
	}
	ENTITYPRESET_TYPE GetTypeOf() const                    { return GetType(); }
	
	RedZombiePreset(EntityManager *entityManager);
	virtual ~RedZombiePreset();
	
	Entity* Create(EntityPresetArgs *args = NULL);
	
	void OnKilled(Entity *entity, Entity *killer);
};

#endif
