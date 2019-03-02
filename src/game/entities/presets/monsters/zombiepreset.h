#ifndef __GAME_ENTITIES_PRESETS_MONSTERS_ZOMBIEPRESET_H_INCLUDED__
#define __GAME_ENTITIES_PRESETS_MONSTERS_ZOMBIEPRESET_H_INCLUDED__

#include "../../../../framework/common.h"
#include "../../../../entities/entitypreset.h"
#include "monsterpreset.h"

class Entity;
class EntityManager;

class ZombiePreset : public MonsterPreset
{
public:
	virtual ~ZombiePreset();

	Entity* Create(EntityPresetArgs *args = NULL);
	
	void OnKilled(Entity *entity, Entity *killer);
	
protected:
	ZombiePreset(EntityManager *entityManager);
};

#endif
