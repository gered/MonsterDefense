#ifndef __GAME_ENTITIES_POWERUPS_SCATTERGUNWEAPONPOWERUP_H_INCLUDED__
#define __GAME_ENTITIES_POWERUPS_SCATTERGUNWEAPONPOWERUP_H_INCLUDED__

#include "../../../framework/common.h"
#include "powerup.h"

class Entity;
class EntityManager;

class ScatterGunWeaponPowerUp : public PowerUp
{
public:
	static POWERUP_TYPE GetType()
	{
		static POWERUP_TYPE typeName = "ScatterGunWeaponPowerUp";
		return typeName;
	}
	POWERUP_TYPE GetTypeOf() const                       { return GetType(); }
	
	ScatterGunWeaponPowerUp(EntityManager *entityManager);
	virtual ~ScatterGunWeaponPowerUp();
	
	void OnEntityCreation(Entity *powerUpEntity);
	void OnPickup(Entity *powerUpEntity, Entity *collector);
	BOOL ShouldCreatedCollectedEntityOnPickup() const    { return TRUE; }
};

#endif
