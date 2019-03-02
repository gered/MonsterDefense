#ifndef __GAME_ENTITIES_WEAPONS_SCATTERGUNWEAPON_H_INCLUDED__
#define __GAME_ENTITIES_WEAPONS_SCATTERGUNWEAPON_H_INCLUDED__

#include "../../../framework/common.h"
#include "weapon.h"

class Entity;
class EntityManager;

class ScatterGunWeapon : public Weapon
{
public:
	static WEAPON_TYPE GetType()
	{
		static WEAPON_TYPE typeName = "ScatterGunWeapon";
		return typeName;
	}
	WEAPON_TYPE GetTypeOf() const                       { return GetType(); }
	
	ScatterGunWeapon(EntityManager *entityManager);
	virtual ~ScatterGunWeapon();
	
	void OnAttack(Entity *attacker);
	BOOL IsMelee() const                                { return FALSE; }
	float GetCooldownTime() const                       { return 0.10f; }
	uint32_t GetImageIndex() const                      { return 23; }
};

#endif
