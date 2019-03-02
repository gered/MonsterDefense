#ifndef __GAME_ENTITIES_WEAPONS_MULTIBALLGUNWEAPON_H_INCLUDED__
#define __GAME_ENTITIES_WEAPONS_MULTIBALLGUNWEAPON_H_INCLUDED__

#include "../../../framework/common.h"
#include "weapon.h"

class Entity;
class EntityManager;

class MultiBallGunWeapon : public Weapon
{
public:
	static WEAPON_TYPE GetType()
	{
		static WEAPON_TYPE typeName = "MultiBallGunWeapon";
		return typeName;
	}
	WEAPON_TYPE GetTypeOf() const                       { return GetType(); }
	
	MultiBallGunWeapon(EntityManager *entityManager);
	virtual ~MultiBallGunWeapon();
	
	void OnAttack(Entity *attacker);
	BOOL IsMelee() const                                { return FALSE; }
	float GetCooldownTime() const                       { return 0.75f; }
	uint32_t GetImageIndex() const                      { return 24; }
};

#endif
