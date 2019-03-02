#include "../../../framework/debug.h"
#include "../../../framework/common.h"

#include "basemeleeweapon.h"
#include "../components/boundingspherecomponent.h"
#include "../components/canbeattackedcomponent.h"
#include "../components/combatmodifierscomponent.h"
#include "../components/hurtcooldowncomponent.h"
#include "../components/npccomponent.h"
#include "../components/orientationxzcomponent.h"
#include "../components/physicscomponent.h"
#include "../components/playercomponent.h"
#include "../components/positioncomponent.h"
#include "../events/hurtevent.h"
#include "../forces/force.h"
#include "../../../entities/entity.h"
#include "../../../entities/entitymanager.h"
#include "../../../events/eventmanager.h"
#include "../../../framework/math/boundingsphere.h"
#include "../../../framework/math/intersectiontester.h"
#include "../../../framework/math/mathhelpers.h"
#include "../../../framework/math/vector3.h"

BaseMeleeWeapon::BaseMeleeWeapon(EntityManager *entityManager)
	: Weapon(entityManager)
{
	STACK_TRACE;
	m_attackAreaSizeModifier = 1.0f;
	m_knockbackStrength = 0.0f;
	m_knockbackFriction = 0.0f;
	m_minDamage = 0;
	m_maxDamage = 0;
}

BaseMeleeWeapon::~BaseMeleeWeapon()
{
	STACK_TRACE;
}

void BaseMeleeWeapon::OnAttack(Entity *attacker)
{
	STACK_TRACE;
	BOOL isAttackerThePlayer = attacker->Has<PlayerComponent>();
	BOOL isAttackerAnNPC = attacker->Has<NPCComponent>();
	
	CombatModifiersComponent *attackerCombatModifiers = attacker->Get<CombatModifiersComponent>();
	
	BoundingSphere areaBeingAttacked = GetAreaBeingAttacked(attacker);
	
	EntityList potentialVictims;
	GetEntityManager()->GetAllWith<CanBeAttackedComponent>(potentialVictims);
	
	for (EntityList::iterator i = potentialVictims.begin(); i != potentialVictims.end(); ++i)
	{
		Entity *potentialVictim = *i;
		
		// potentialVictim and attacker could possibly both be the same entity...
		if (attacker == potentialVictim)
			continue;
		
		// can we even hurt this entity?
		if (potentialVictim->Has<HurtCooldownComponent>())
			continue;
		
		CanBeAttackedComponent *canBeAttacked = potentialVictim->Get<CanBeAttackedComponent>();
		if (canBeAttacked->byPlayer == FALSE && isAttackerThePlayer == TRUE)
			continue;
		if (canBeAttacked->byNPC == FALSE && isAttackerAnNPC == TRUE)
			continue;
		
		BoundingSphereComponent *victimBounds = potentialVictim->Get<BoundingSphereComponent>();
		if (IntersectionTester::Test(victimBounds->bounds, areaBeingAttacked))
		{
			// entity was hit by the attack
			
			// calculate damage amount of this attack
			uint32_t damage = Rnd(GetMinDamage(), GetMaxDamage());
			damage *= GetDamageBonusForAttacker(attacker);
			if (attackerCombatModifiers != NULL)
				damage = (uint32_t)((float)damage * attackerCombatModifiers->damageModifier);
			
			// trigger hurt event
			HurtEvent *hurtEvent = new HurtEvent(potentialVictim);
			hurtEvent->usePercent = FALSE;
			hurtEvent->amount = damage;
			hurtEvent->source = attacker;
			GetEntityManager()->GetEventManager()->Queue(hurtEvent);
			
			if (GetKnockbackStrength() > 0.0f)
			{
				// if the entity has physics, add a knockback force too
				PhysicsComponent *victimPhysics = potentialVictim->Get<PhysicsComponent>();
				if (victimPhysics != NULL)
				{
					Force *knockbackForce = GetKnockbackForce(attacker, potentialVictim);
					if (knockbackForce != NULL)
						victimPhysics->forces.push_back(knockbackForce);
				}
			}
		}
	}
}

BoundingSphere BaseMeleeWeapon::GetAreaBeingAttacked(Entity *attacker)
{
	STACK_TRACE;
	BoundingSphereComponent *attackerBounds = attacker->Get<BoundingSphereComponent>();
	ASSERT(attackerBounds != NULL);
	OrientationXZComponent *attackerOrientation = attacker->Get<OrientationXZComponent>();
	ASSERT(attackerOrientation != NULL);
	
	// determine area in front of the attacker to check for potential victims
	// (entities which will be hurt by this attack).
	BoundingSphere areaBeingAttacked = attackerBounds->bounds;
	Vector3 attackDirection = GetDirectionFromYAxisOrientation(attackerOrientation->angle) * (attackerBounds->bounds.radius);
	areaBeingAttacked.center += attackDirection;
	areaBeingAttacked.radius *= GetAttackAreaSizeModifier();
	
	return areaBeingAttacked;
}

Force* BaseMeleeWeapon::GetKnockbackForce(Entity *attacker, Entity *victim)
{
	STACK_TRACE;
	PositionComponent *attackerPosition = attacker->Get<PositionComponent>();
	ASSERT(attackerPosition != NULL);
	PositionComponent *victimPosition = victim->Get<PositionComponent>();
	ASSERT(victimPosition != NULL);

	Vector3 knockbackDirection = Vector3::Normalize(victimPosition->position - attackerPosition->position);
	return new Force(knockbackDirection, GetKnockbackStrength(), GetKnockbackFriction());
}
