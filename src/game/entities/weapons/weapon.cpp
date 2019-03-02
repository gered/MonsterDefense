#include "../../../framework/debug.h"

#include "weapon.h"
#include "../components/npccomponent.h"
#include "../globalcomponents/monsterpowercomponent.h"
#include "../../../entities/entity.h"
#include "../../../entities/entitymanager.h"

Weapon::Weapon(EntityManager *entityManager)
{
	m_entityManager = entityManager;
}

Weapon::~Weapon()
{
}

void Weapon::OnAttack(Entity *attacker)
{
}

float Weapon::GetDamageBonusForAttacker(Entity *attacker) const
{
	STACK_TRACE;
	if (attacker->Has<NPCComponent>())
	{
		MonsterPowerComponent *monsterPower = GetEntityManager()->GetGlobalComponent<MonsterPowerComponent>();
		ASSERT(monsterPower != NULL);
		return monsterPower->attackModifier;
	}
	else
		return 1.0f;
}
