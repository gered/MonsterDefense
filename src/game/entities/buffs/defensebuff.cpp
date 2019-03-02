#include "../../../framework/debug.h"

#include "defensebuff.h"
#include "../../../entities/entity.h"
#include "../../../entities/entitymanager.h"
#include "../components/combatmodifierscomponent.h"

DefenseBuff::DefenseBuff(EntityManager *entityManager)
	: Buff(entityManager)
{
	STACK_TRACE;
}

DefenseBuff::~DefenseBuff()
{
	STACK_TRACE;
}

void DefenseBuff::OnApply(Entity *entity)
{
	STACK_TRACE;
	CombatModifiersComponent *combatModifiers = entity->Get<CombatModifiersComponent>();
	ASSERT(combatModifiers != NULL);
	
	combatModifiers->defenseModifier -= 0.5f;
}

void DefenseBuff::OnRemove(Entity *entity)
{
	STACK_TRACE;
	CombatModifiersComponent *combatModifiers = entity->Get<CombatModifiersComponent>();
	ASSERT(combatModifiers != NULL);
	
	combatModifiers->defenseModifier += 0.5f;
}
