#include "../../../framework/debug.h"

#include "fistweapon.h"

FistWeapon::FistWeapon(EntityManager *entityManager)
	: BaseMeleeWeapon(entityManager)
{
	STACK_TRACE;
	SetAttackAreaSizeModifier(0.8f);
	SetKnockbackStrength(15.0f);
	SetKnockbackFriction(0.8f);
	SetMinDamage(75);
	SetMaxDamage(125);
}

FistWeapon::~FistWeapon()
{
	STACK_TRACE;
}
