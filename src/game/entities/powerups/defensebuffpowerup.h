#ifndef __GAME_ENTITIES_POWERUPS_DEFENSEBUFFPOWERUP_H_INCLUDED__
#define __GAME_ENTITIES_POWERUPS_DEFENSEBUFFPOWERUP_H_INCLUDED__

#include "../../../framework/common.h"
#include "powerup.h"

class Entity;
class EntityManager;

class DefenseBuffPowerUp : public PowerUp
{
public:
	static POWERUP_TYPE GetType()
	{
		static POWERUP_TYPE typeName = "DefenseBuffPowerUp";
		return typeName;
	}
	POWERUP_TYPE GetTypeOf() const                       { return GetType(); }
	
	DefenseBuffPowerUp(EntityManager *entityManager);
	virtual ~DefenseBuffPowerUp();
	
	void OnEntityCreation(Entity *powerUpEntity);
	void OnPickup(Entity *powerUpEntity, Entity *collector);
	BOOL ShouldCreatedCollectedEntityOnPickup() const    { return FALSE; }
};

#endif
