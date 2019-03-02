#include "../../../framework/debug.h"

#include "attackbuff.h"
#include "../../../entities/entity.h"
#include "../../../entities/entitymanager.h"
#include "../components/combatmodifierscomponent.h"

AttackBuff::AttackBuff(EntityManager *entityManager)
	: Buff(entityManager)
{
	STACK_TRACE;
}

AttackBuff::~AttackBuff()
{
	STACK_TRACE;
}

void AttackBuff::OnApply(Entity *entity)
{
	STACK_TRACE;
	CombatModifiersComponent *combatModifiers = entity->Get<CombatModifiersComponent>();
	ASSERT(combatModifiers != NULL);
	
	combatModifiers->damageModifier += 1.0f;
}

void AttackBuff::OnRemove(Entity *entity)
{
	STACK_TRACE;
	CombatModifiersComponent *combatModifiers = entity->Get<CombatModifiersComponent>();
	ASSERT(combatModifiers != NULL);
	
	combatModifiers->damageModifier -= 1.0f;
}
