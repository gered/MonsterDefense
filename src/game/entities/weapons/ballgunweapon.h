#ifndef __GAME_ENTITIES_WEAPONS_BALLGUNWEAPON_H_INCLUDED__
#define __GAME_ENTITIES_WEAPONS_BALLGUNWEAPON_H_INCLUDED__

#include "../../../framework/common.h"
#include "weapon.h"

class Entity;
class EntityManager;

class BallGunWeapon : public Weapon
{
public:
	static WEAPON_TYPE GetType()
	{
		static WEAPON_TYPE typeName = "BallGunWeapon";
		return typeName;
	}
	WEAPON_TYPE GetTypeOf() const                       { return GetType(); }
	
	BallGunWeapon(EntityManager *entityManager);
	virtual ~BallGunWeapon();

	void OnAttack(Entity *attacker);
	BOOL IsMelee() const                                { return FALSE; }
	float GetCooldownTime() const                       { return 0.25f; }
	uint32_t GetImageIndex() const                      { return 22; }
};

#endif
