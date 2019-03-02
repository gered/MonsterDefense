#ifndef __GAME_ENTITIES_POWERUPS_MULTIBALLGUNWEAPONPOWERUP_H_INCLUDED__
#define __GAME_ENTITIES_POWERUPS_MULTIBALLGUNWEAPONPOWERUP_H_INCLUDED__

#include "../../../framework/common.h"
#include "powerup.h"

class Entity;
class EntityManager;

class MultiBallGunWeaponPowerUp : public PowerUp
{
public:
	static POWERUP_TYPE GetType()
	{
		static POWERUP_TYPE typeName = "MultiBallGunWeaponPowerUp";
		return typeName;
	}
	POWERUP_TYPE GetTypeOf() const                       { return GetType(); }
	
	MultiBallGunWeaponPowerUp(EntityManager *entityManager);
	virtual ~MultiBallGunWeaponPowerUp();
	
	void OnEntityCreation(Entity *powerUpEntity);
	void OnPickup(Entity *powerUpEntity, Entity *collector);
	BOOL ShouldCreatedCollectedEntityOnPickup() const    { return TRUE; }
};

#endif
