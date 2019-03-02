#ifndef __GAME_ENTITIES_PRESETS_PLAYERPRESET_H_INCLUDED__
#define __GAME_ENTITIES_PRESETS_PLAYERPRESET_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../entities/entitypresetargs.h"
#include "basecharacterentitypreset.h"

class Entity;
class EntityManager;

class PlayerPreset : public BaseCharacterEntityPreset
{
public:
	static ENTITYPRESET_TYPE GetType()
	{
		static ENTITYPRESET_TYPE typeName = "player";
		return typeName;
	}
	ENTITYPRESET_TYPE GetTypeOf() const                    { return GetType(); }

	PlayerPreset(EntityManager *entityManager);
	virtual ~PlayerPreset();

	Entity* Create(EntityPresetArgs *args = NULL);

	void OnDespawn(Entity *entity);
};

#endif
