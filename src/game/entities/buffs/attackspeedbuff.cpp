#include "../../../framework/debug.h"

#include "attackspeedbuff.h"
#include "../../../entities/entity.h"
#include "../../../entities/entitymanager.h"
#include "../components/combatmodifierscomponent.h"

AttackSpeedBuff::AttackSpeedBuff(EntityManager *entityManager)
	: Buff(entityManager)
{
	STACK_TRACE;
}

AttackSpeedBuff::~AttackSpeedBuff()
{
	STACK_TRACE;
}

void AttackSpeedBuff::OnApply(Entity *entity)
{
	STACK_TRACE;
	CombatModifiersComponent *combatModifiers = entity->Get<CombatModifiersComponent>();
	ASSERT(combatModifiers != NULL);
	
	combatModifiers->attackCooldownModifier -= 0.5f;
}

void AttackSpeedBuff::OnRemove(Entity *entity)
{
	STACK_TRACE;
	CombatModifiersComponent *combatModifiers = entity->Get<CombatModifiersComponent>();
	ASSERT(combatModifiers != NULL);
	
	combatModifiers->attackCooldownModifier += 0.5f;
}
