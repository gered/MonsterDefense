#ifndef __GAME_ENTITIES_WEAPONS_BASEMELEEWEAPON_H_INCLUDED__
#define __GAME_ENTITIES_WEAPONS_BASEMELEEWEAPON_H_INCLUDED__

#include "../../../framework/common.h"
#include "weapon.h"
#include "../../../framework/math/boundingsphere.h"

class Entity;
class EntityManager;
class Force;

class BaseMeleeWeapon : public Weapon
{
public:
	virtual ~BaseMeleeWeapon();
	
	virtual void OnAttack(Entity *attacker);
	BOOL IsMelee() const                                { return TRUE; }
	
protected:
	BaseMeleeWeapon(EntityManager *entityManager);
	
	virtual BoundingSphere GetAreaBeingAttacked(Entity *attacker);
	virtual Force* GetKnockbackForce(Entity *attacker, Entity *victim);
	
	void SetAttackAreaSizeModifier(float sizeModifier)  { m_attackAreaSizeModifier = sizeModifier; }
	float GetAttackAreaSizeModifier() const             { return m_attackAreaSizeModifier; }
	void SetKnockbackStrength(float strength)           { m_knockbackStrength = strength; }
	float GetKnockbackStrength() const                  { return m_knockbackStrength; }
	void SetKnockbackFriction(float friction)           { m_knockbackFriction = friction; }
	float GetKnockbackFriction() const                  { return m_knockbackFriction; }
	void SetMinDamage(int damage)                       { m_minDamage = damage; }
	int GetMinDamage() const                            { return m_minDamage; }
	void SetMaxDamage(int damage)                       { m_maxDamage = damage; }
	int GetMaxDamage() const                            { return m_maxDamage; }
	
private:
	float m_attackAreaSizeModifier;
	float m_knockbackStrength;
	float m_knockbackFriction;
	int m_minDamage;
	int m_maxDamage;
};

#endif
