#ifndef __GAME_ENTITIES_POWERUPS_ATTACKBUFFPOWERUP_H_INCLUDED__
#define __GAME_ENTITIES_POWERUPS_ATTACKBUFFPOWERUP_H_INCLUDED__

#include "../../../framework/common.h"
#include "powerup.h"

class Entity;
class EntityManager;

class AttackBuffPowerUp : public PowerUp
{
public:
	static POWERUP_TYPE GetType()
	{
		static POWERUP_TYPE typeName = "AttackBuffPowerUp";
		return typeName;
	}
	POWERUP_TYPE GetTypeOf() const                       { return GetType(); }
	
	AttackBuffPowerUp(EntityManager *entityManager);
	virtual ~AttackBuffPowerUp();
	
	void OnEntityCreation(Entity *powerUpEntity);
	void OnPickup(Entity *powerUpEntity, Entity *collector);
	BOOL ShouldCreatedCollectedEntityOnPickup() const    { return FALSE; }
};

#endif
