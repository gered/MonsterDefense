#ifndef __GAME_ENTITIES_WEAPONS_FISTWITHSMALLKNOCKBACKWEAPON_H_INCLUDED__
#define __GAME_ENTITIES_WEAPONS_FISTWITHSMALLKNOCKBACKWEAPON_H_INCLUDED__

#include "../../../framework/common.h"
#include "basemeleeweapon.h"

class Entity;
class EntityManager;

class FistWithSmallKnockbackWeapon : public BaseMeleeWeapon
{
public:
	static WEAPON_TYPE GetType()
	{
		static WEAPON_TYPE typeName = "FistWithSmallKnockbackWeapon";
		return typeName;
	}
	WEAPON_TYPE GetTypeOf() const                       { return GetType(); }
	
	FistWithSmallKnockbackWeapon(EntityManager *entityManager);
	virtual ~FistWithSmallKnockbackWeapon();
	
	BOOL IsMelee() const                                { return TRUE; }
	float GetCooldownTime() const                       { return 0.25f; }
	uint32_t GetImageIndex() const                      { return 20; }
};

#endif
