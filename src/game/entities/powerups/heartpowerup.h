#ifndef __GAME_ENTITIES_POWERUPS_HEARTPOWERUP_H_INCLUDED__
#define __GAME_ENTITIES_POWERUPS_HEARTPOWERUP_H_INCLUDED__

#include "../../../framework/common.h"
#include "powerup.h"

class Entity;
class EntityManager;

class HeartPowerUp : public PowerUp
{
public:
	static POWERUP_TYPE GetType()
	{
		static POWERUP_TYPE typeName = "HeartPowerUp";
		return typeName;
	}
	POWERUP_TYPE GetTypeOf() const                       { return GetType(); }

	HeartPowerUp(EntityManager *entityManager);
	virtual ~HeartPowerUp();
	
	void OnEntityCreation(Entity *powerUpEntity);
	void OnPickup(Entity *powerUpEntity, Entity *collector);
	BOOL ShouldCreatedCollectedEntityOnPickup() const    { return FALSE; }
};

#endif
