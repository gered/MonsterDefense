#ifndef __GAME_ENTITIES_POWERUPS_BLUEGEMPOWERUP_H_INCLUDED__
#define __GAME_ENTITIES_POWERUPS_BLUEGEMPOWERUP_H_INCLUDED__

#include "../../../framework/common.h"
#include "powerup.h"

class Entity;
class EntityManager;

class BlueGemPowerUp : public PowerUp
{
public:
	static POWERUP_TYPE GetType()
	{
		static POWERUP_TYPE typeName = "BlueGemPowerUp";
		return typeName;
	}
	POWERUP_TYPE GetTypeOf() const                       { return GetType(); }

	BlueGemPowerUp(EntityManager *entityManager);
	virtual ~BlueGemPowerUp();
	
	void OnEntityCreation(Entity *powerUpEntity);
	void OnPickup(Entity *powerUpEntity, Entity *collector);
	BOOL ShouldCreatedCollectedEntityOnPickup() const    { return FALSE; }
};

#endif
