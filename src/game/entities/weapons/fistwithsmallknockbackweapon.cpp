#include "../../../framework/debug.h"

#include "fistwithsmallknockbackweapon.h"

FistWithSmallKnockbackWeapon::FistWithSmallKnockbackWeapon(EntityManager *entityManager)
: BaseMeleeWeapon(entityManager)
{
	STACK_TRACE;
	SetAttackAreaSizeModifier(0.8f);
	SetKnockbackStrength(7.0f);
	SetKnockbackFriction(0.8f);
	SetMinDamage(75);
	SetMaxDamage(125);
}

FistWithSmallKnockbackWeapon::~FistWithSmallKnockbackWeapon()
{
	STACK_TRACE;
}
