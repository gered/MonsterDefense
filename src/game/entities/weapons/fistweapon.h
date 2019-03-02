#ifndef __GAME_ENTITIES_WEAPONS_FISTWEAPON_H_INCLUDED__
#define __GAME_ENTITIES_WEAPONS_FISTWEAPON_H_INCLUDED__

#include "../../../framework/common.h"
#include "basemeleeweapon.h"

class Entity;
class EntityManager;

class FistWeapon : public BaseMeleeWeapon
{
public:
	static WEAPON_TYPE GetType()
	{
		static WEAPON_TYPE typeName = "FistWeapon";
		return typeName;
	}
	WEAPON_TYPE GetTypeOf() const                       { return GetType(); }
	
	FistWeapon(EntityManager *entityManager);
	virtual ~FistWeapon();
	
	BOOL IsMelee() const                                { return TRUE; }
	float GetCooldownTime() const                       { return 0.25f; }
	uint32_t GetImageIndex() const                      { return 20; }
};

#endif
