#include "../../../framework/debug.h"

#include "scattergunweapon.h"
#include "../components/boundingspherecomponent.h"
#include "../components/bulletcomponent.h"
#include "../components/combatmodifierscomponent.h"
#include "../components/npccomponent.h"
#include "../components/playercomponent.h"
#include "../components/positioncomponent.h"
#include "../components/orientationxzcomponent.h"
#include "../components/simplephysicscomponent.h"
#include "../presets/bullets/scatterbulletpreset.h"
#include "../../../entities/entity.h"
#include "../../../entities/entitymanager.h"
#include "../../../framework/math/mathhelpers.h"
#include "../../../framework/math/vector3.h"

ScatterGunWeapon::ScatterGunWeapon(EntityManager *entityManager)
: Weapon(entityManager)
{
	STACK_TRACE;
}

ScatterGunWeapon::~ScatterGunWeapon()
{
	STACK_TRACE;
}

void ScatterGunWeapon::OnAttack(Entity *attacker)
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
	
	BOOL isOwnedByPlayer = attacker->Has<PlayerComponent>();
	BOOL isOwnedByNPC = attacker->Has<NPCComponent>();
	
	int numBullets = 1;
	
	float chanceForAnotherBulletRoll = Rnd(0.0f, 1.0f);
	if (chanceForAnotherBulletRoll < 0.6f)
		++numBullets;

	chanceForAnotherBulletRoll = Rnd(0.0f, 1.0f);
	if (chanceForAnotherBulletRoll < 0.3f)
		++numBullets;
	
	for (int i = 0; i < numBullets; ++i)
	{
		// create the bullet
		Entity *bullet = GetEntityManager()->AddUsingPreset<ScatterBulletPreset>();
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
		bulletOrientation->angle = attackerOrientation->angle;
		bulletPosition->position = attackerPosition->position;
		
		// offset bullet angle in a random amount so they fire out in a type of
		// cone-shape area in the direction the attacker is facing
		float offset = Rnd(DegreesToRadians(-15.0f), DegreesToRadians(15.0f));
		bulletOrientation->angle += offset;
		
		// start bullet out at the edge of the attacker's bounding radius instead
		// of directly at their position (which would make the bullet spawn inside 
		// the center of the attacker which is a bit odd)
		bulletPosition->position += GetDirectionFromYAxisOrientation(bulletOrientation->angle) * attackerBounds->bounds.radius;
	}
}
