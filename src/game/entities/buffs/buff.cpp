#include "../../../framework/debug.h"

#include "buff.h"

Buff::Buff(EntityManager *entityManager)
{
	STACK_TRACE;
	m_entityManager = entityManager;
}

Buff::~Buff()
{
	STACK_TRACE;
}

void Buff::OnApply(Entity *entity)
{
	STACK_TRACE;
}

void Buff::OnRemove(Entity *entity)
{
	STACK_TRACE;
}
