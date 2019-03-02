#ifndef __GAME_ENTITIES_PRESETS_MONSTERS_MONSTERPRESET_H_INCLUDED__
#define __GAME_ENTITIES_PRESETS_MONSTERS_MONSTERPRESET_H_INCLUDED__

#include "../../../../framework/common.h"
#include "../../../../entities/entitypresetargs.h"
#include "../basecharacterentitypreset.h"

class Entity;
class EntityManager;

class MonsterPreset : public BaseCharacterEntityPreset
{
public:
	virtual ~MonsterPreset();
	
	Entity* Create(EntityPresetArgs *args = NULL);

	virtual void OnSpawn(Entity *entity);
	virtual void OnDespawn(Entity *entity);
	virtual void OnHeal(Entity *entity, Entity *source);
	virtual void OnHurt(Entity *entity, Entity *source);
	virtual void OnKilled(Entity *entity, Entity *killer);
	
protected:
	MonsterPreset(EntityManager *entityManager);

	void AwardScoreToPlayer(Entity *sourceOfScore, int32_t score);
};

#endif
