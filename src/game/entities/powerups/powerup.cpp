#include "../../../framework/debug.h"

#include "powerup.h"
#include "../components/playercomponent.h"
#include "../components/positioncomponent.h"
#include "../events/healevent.h"
#include "../events/hurtevent.h"
#include "../events/scoreevent.h"
#include "../../../entities/entity.h"
#include "../../../entities/entitymanager.h"
#include "../../../events/eventmanager.h"

PowerUp::PowerUp(EntityManager *entityManager)
{
	STACK_TRACE;
	m_entityManager = entityManager;
}

PowerUp::~PowerUp()
{
	STACK_TRACE;
}

void PowerUp::OnEntityCreation(Entity *powerUpEntity)
{
	STACK_TRACE;
}

void PowerUp::OnPickup(Entity *powerUpEntity, Entity *collector)
{
	STACK_TRACE;
}

BOOL PowerUp::ShouldCreatedCollectedEntityOnPickup() const
{
	STACK_TRACE;
	return FALSE;
}

void PowerUp::AwardScoreToEntity(Entity *entity, Entity *powerUpEntity, int32_t score)
{
	STACK_TRACE;
	ScoreEvent *event = new ScoreEvent(entity);
	event->scoreAwarded = score;
	if (powerUpEntity != NULL)
	{
		event->hasSourcePosition = TRUE;
		event->sourcePosition = powerUpEntity->Get<PositionComponent>()->position;
	}
	GetEntityManager()->GetEventManager()->Queue(event);
}

void PowerUp::HealEntity(Entity *entity, Entity *powerUpEntity, uint32_t amount)
{
	STACK_TRACE;
	HealEvent *e = new HealEvent(entity);
	e->usePercent = FALSE;
	e->amount = amount;
	e->source = powerUpEntity;
	GetEntityManager()->GetEventManager()->Queue(e);
}

void PowerUp::HealEntity(Entity *entity, Entity *powerUpEntity, float percentAmount)
{
	STACK_TRACE;
	HealEvent *e = new HealEvent(entity);
	e->usePercent = TRUE;
	e->percent = percentAmount;
	e->source = powerUpEntity;
	GetEntityManager()->GetEventManager()->Queue(e);
}

void PowerUp::HurtEntity(Entity *entity, Entity *powerUpEntity, uint32_t amount)
{
	STACK_TRACE;
	HurtEvent *e = new HurtEvent(entity);
	e->usePercent = FALSE;
	e->amount = amount;
	e->source = powerUpEntity;
	GetEntityManager()->GetEventManager()->Queue(e);
}

void PowerUp::HurtEntity(Entity *entity, Entity *powerUpEntity, float percentAmount)
{
	STACK_TRACE;
	HurtEvent *e = new HurtEvent(entity);
	e->usePercent = TRUE;
	e->percent = percentAmount;
	e->source = powerUpEntity;
	GetEntityManager()->GetEventManager()->Queue(e);
}
