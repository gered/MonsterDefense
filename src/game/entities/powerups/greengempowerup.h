#ifndef __GAME_ENTITIES_POWERUPS_GREENGEMPOWERUP_H_INCLUDED__
#define __GAME_ENTITIES_POWERUPS_GREENGEMPOWERUP_H_INCLUDED__

#include "../../../framework/common.h"
#include "powerup.h"

class Entity;
class EntityManager;

class GreenGemPowerUp : public PowerUp
{
public:
	static POWERUP_TYPE GetType()
	{
		static POWERUP_TYPE typeName = "GreenGemPowerUp";
		return typeName;
	}
	POWERUP_TYPE GetTypeOf() const                       { return GetType(); }

	GreenGemPowerUp(EntityManager *entityManager);
	virtual ~GreenGemPowerUp();
	
	void OnEntityCreation(Entity *powerUpEntity);
	void OnPickup(Entity *powerUpEntity, Entity *collector);
	BOOL ShouldCreatedCollectedEntityOnPickup() const    { return FALSE; }
};

#endif
