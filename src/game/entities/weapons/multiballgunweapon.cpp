#include "../../../framework/debug.h"

#include "multiballgunweapon.h"
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

MultiBallGunWeapon::MultiBallGunWeapon(EntityManager *entityManager)
: Weapon(entityManager)
{
	STACK_TRACE;
}

MultiBallGunWeapon::~MultiBallGunWeapon()
{
	STACK_TRACE;
}

void MultiBallGunWeapon::OnAttack(Entity *attacker)
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
	
	const float NEXT_BULLET_ANGLE_INCREMENT = DegreesToRadians(15.0f);
	float bulletAngle = attackerOrientation->angle - NEXT_BULLET_ANGLE_INCREMENT;
	
	BOOL isOwnedByPlayer = attacker->Has<PlayerComponent>();
	BOOL isOwnedByNPC = attacker->Has<NPCComponent>();

	for (int i = 0; i < 3; ++i)
	{
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
		bulletInfo->isOwnedByPlayer = isOwnedByPlayer;
		bulletInfo->isOwnedByNPC = isOwnedByNPC;
		
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
				bulletInfo->flatDamageAmount *= attackerCombatModifiers->damageModifier;
		}
		
		// set bullet initial position/orientation based on attacker's current state
		bulletOrientation->angle = bulletAngle;
		bulletPosition->position = attackerPosition->position;
		
		// start bullet out at the edge of the attacker's bounding radius instead
		// of directly at their position (which would make the bullet spawn inside 
		// the center of the attacker which is a bit odd)
		bulletPosition->position += GetDirectionFromYAxisOrientation(bulletOrientation->angle) * attackerBounds->bounds.radius;
		
		bulletAngle += NEXT_BULLET_ANGLE_INCREMENT;
	}
}
