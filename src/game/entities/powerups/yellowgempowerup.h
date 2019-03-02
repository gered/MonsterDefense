#ifndef __GAME_ENTITIES_POWERUPS_YELLOWGEMPOWERUP_H_INCLUDED__
#define __GAME_ENTITIES_POWERUPS_YELLOWGEMPOWERUP_H_INCLUDED__

#include "../../../framework/common.h"
#include "powerup.h"

class Entity;
class EntityManager;

class YellowGemPowerUp : public PowerUp
{
public:
	static POWERUP_TYPE GetType()
	{
		static POWERUP_TYPE typeName = "YellowGemPowerUp";
		return typeName;
	}
	POWERUP_TYPE GetTypeOf() const                       { return GetType(); }

	YellowGemPowerUp(EntityManager *entityManager);
	virtual ~YellowGemPowerUp();
	
	void OnEntityCreation(Entity *powerUpEntity);
	void OnPickup(Entity *powerUpEntity, Entity *collector);
	BOOL ShouldCreatedCollectedEntityOnPickup() const    { return FALSE; }
};

#endif
