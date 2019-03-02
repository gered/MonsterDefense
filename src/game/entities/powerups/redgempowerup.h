#ifndef __GAME_ENTITIES_POWERUPS_REDGEMPOWERUP_H_INCLUDED__
#define __GAME_ENTITIES_POWERUPS_REDGEMPOWERUP_H_INCLUDED__

#include "../../../framework/common.h"
#include "powerup.h"

class Entity;
class EntityManager;

class RedGemPowerUp : public PowerUp
{
public:
	static POWERUP_TYPE GetType()
	{
		static POWERUP_TYPE typeName = "RedGemPowerUp";
		return typeName;
	}
	POWERUP_TYPE GetTypeOf() const                       { return GetType(); }

	RedGemPowerUp(EntityManager *entityManager);
	virtual ~RedGemPowerUp();
	
	void OnEntityCreation(Entity *powerUpEntity);
	void OnPickup(Entity *powerUpEntity, Entity *collector);
	BOOL ShouldCreatedCollectedEntityOnPickup() const    { return FALSE; }
};

#endif
