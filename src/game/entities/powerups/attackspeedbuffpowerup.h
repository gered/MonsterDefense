#ifndef __GAME_ENTITIES_POWERUPS_ATTACKSPEEDBUFFPOWERUP_H_INCLUDED__
#define __GAME_ENTITIES_POWERUPS_ATTACKSPEEDBUFFPOWERUP_H_INCLUDED__

#include "../../../framework/common.h"
#include "powerup.h"

class Entity;
class EntityManager;

class AttackSpeedBuffPowerUp : public PowerUp
{
public:
	static POWERUP_TYPE GetType()
	{
		static POWERUP_TYPE typeName = "AttackSpeedBuffPowerUp";
		return typeName;
	}
	POWERUP_TYPE GetTypeOf() const                       { return GetType(); }
	
	AttackSpeedBuffPowerUp(EntityManager *entityManager);
	virtual ~AttackSpeedBuffPowerUp();
	
	void OnEntityCreation(Entity *powerUpEntity);
	void OnPickup(Entity *powerUpEntity, Entity *collector);
	BOOL ShouldCreatedCollectedEntityOnPickup() const    { return FALSE; }
};

#endif
