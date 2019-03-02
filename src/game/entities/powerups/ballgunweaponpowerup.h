#ifndef __GAME_ENTITIES_POWERUPS_BALLGUNWEAPONPOWERUP_H_INCLUDED__
#define __GAME_ENTITIES_POWERUPS_BALLGUNWEAPONPOWERUP_H_INCLUDED__

#include "../../../framework/common.h"
#include "powerup.h"

class Entity;
class EntityManager;

class BallGunWeaponPowerUp : public PowerUp
{
public:
	static POWERUP_TYPE GetType()
	{
		static POWERUP_TYPE typeName = "BallGunWeaponPowerUp";
		return typeName;
	}
	POWERUP_TYPE GetTypeOf() const                       { return GetType(); }
	
	BallGunWeaponPowerUp(EntityManager *entityManager);
	virtual ~BallGunWeaponPowerUp();
	
	void OnEntityCreation(Entity *powerUpEntity);
	void OnPickup(Entity *powerUpEntity, Entity *collector);
	BOOL ShouldCreatedCollectedEntityOnPickup() const    { return TRUE; }
};

#endif
