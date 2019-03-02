#include "../../../framework/debug.h"

#include "ballgunweapon.h"
#include "../components/boundingspherecomponent.h"
#include "../components/bulletcomponent.h"
#include "../components/combatmodifierscomponent.h"
#include "../components/npccomponent.h"
#include "../components/playercomponent.h"
#include "../components/positioncomponent.h"
#include "../components/orientationxzcomponent.h"
#include "../components/simplephysicscomponent.h"
#include "../presets/bullets/ballbulletpreset.h"
#include "../../../entities/entity.h"
#include "../../../entities/entitymanager.h"
#include "../../../framework/math/mathhelpers.h"
#include "../../../framework/math/vector3.h"

BallGunWeapon::BallGunWeapon(EntityManager *entityManager)
: Weapon(entityManager)
{
	STACK_TRACE;
}

BallGunWeapon::~BallGunWeapon()
{
	STACK_TRACE;
}

void BallGunWeapon::OnAttack(Entity *attacker)
{
	STACK_TRACE;
	// attacker info
	PositionComponent *attackerPosition = attacker->Get<PositionComponent>();
	ASSERT(attackerPosition != NULL);
	OrientationXZComponent *attackerOrientation = attacker->Get<OrientationXZComponent>();
	ASSERT(attackerOrientation != NULL);
	BoundingSphereComponent *attackerBounds = attacker->Get<BoundingSphereComponent>();
	ASSERT(attackerBounds != NULL);
	CombatModifiersComponent *attackerCombatModifiers = attacker->Get<CombatModifiersComponent>();
	
	// create the bullet
	Entity *bullet = GetEntityManager()->AddUsingPreset<BallBulletPreset>();
	BulletComponent *bulletInfo = bullet->Get<BulletComponent>();
	ASSERT(bulletInfo != NULL);
	PositionComponent *bulletPosition = bullet->Get<PositionComponent>();
	ASSERT(bulletPosition != NULL);
	OrientationXZComponent *bulletOrientation = bullet->Get<OrientationXZComponent>();
	ASSERT(bulletOrientation != NULL);
	SimplePhysicsComponent *bulletPhysics = bullet->Get<SimplePhysicsComponent>();
	ASSERT(bulletPhysics != NULL);
	
	bulletInfo->source = attacker;
	bulletInfo->isOwnedByPlayer = attacker->Has<PlayerComponent>();
	bulletInfo->isOwnedByNPC = attacker->Has<NPCComponent>();
	
	// apply attacker's combat damage modifiers to the bullet's base damage amount
	if (attackerCombatModifiers != NULL)
	{
		if (bulletInfo->usePercentDamageAmount)
		{
			bulletInfo->percentDamageAmount *= attackerCombatModifiers->damageModifier;
			if (bulletInfo->percentDamageAmount > 1.0f)
				bulletInfo->percentDamageAmount = 1.0f;
		}
		else
		{
			bulletInfo->flatDamageAmount *= attackerCombatModifiers->damageModifier;
			bulletInfo->flatDamageAmount *= GetDamageBonusForAttacker(attacker);
		}
	}

	// set bullet initial position/orientation based on attacker's current state
	bulletOrientation->angle = attackerOrientation->angle;
	bulletPosition->position = attackerPosition->position;
	
	// start bullet out at the edge of the attacker's bounding radius instead
	// of directly at their position (which would make the bullet spawn inside 
	// the center of the attacker which is a bit odd)
	bulletPosition->position += GetDirectionFromYAxisOrientation(bulletOrientation->angle) * attackerBounds->bounds.radius;
}
