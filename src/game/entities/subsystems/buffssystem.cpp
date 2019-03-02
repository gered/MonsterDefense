#include "../../../framework/debug.h"

#include "buffssystem.h"
#include "../buffs/appliedbuff.h"
#include "../buffs/buff.h"
#include "../buffs/buffcollection.h"
#include "../components/buffscomponent.h"
#include "../events/buffaddedevent.h"
#include "../events/buffremovedevent.h"
#include "../globalcomponents/buffcollectioncomponent.h"
#include "../../../entities/entity.h"
#include "../../../entities/entitymanager.h"
#include "../../../events/event.h"
#include "../../../events/eventmanager.h"

BuffsSystem::BuffsSystem(EntityManager *entityManager, EventManager *eventManager)
	: ComponentSystem(entityManager, eventManager)
{
	STACK_TRACE;
	ListenFor<BuffAddedEvent>();
}

BuffsSystem::~BuffsSystem()
{
	STACK_TRACE;
	StopListeningFor<BuffAddedEvent>();
}

void BuffsSystem::OnUpdate(float delta)
{
	STACK_TRACE;
	EntityList list;
	GetEntityManager()->GetAllWith<BuffsComponent>(list);
	for (EntityList::iterator i = list.begin(); i != list.end(); ++i)
	{
		Entity *entity = *i;
		BuffsComponent *buffs = entity->Get<BuffsComponent>();
		
		// go through each of the buffs on this entity and update the
		// remaining duration time, removing buffs which have expired
		AppliedBuffsList::iterator j = buffs->buffs.begin();
		while (j != buffs->buffs.end())
		{
			AppliedBuff *appliedBuff = &(*j);
			appliedBuff->remainingTime -= delta;
			if (appliedBuff->remainingTime <= 0.0f)
			{
				// this buff has worn off, so remove it
				appliedBuff->buff->OnRemove(entity);
				j = buffs->buffs.erase(j);
			}
			else
				++j;
		}
	}
}

BOOL BuffsSystem::Handle(const Event *event)
{
	STACK_TRACE;
	if (event->Is<BuffAddedEvent>())
	{
		const BuffAddedEvent *e = event->As<BuffAddedEvent>();
		ASSERT(e->type != NULL);

		BuffsComponent *entityBuffs = e->GetEntity()->Get<BuffsComponent>();
		ASSERT(entityBuffs != NULL);
		
		// check if the entity has this buff already, and if so, refresh the
		// remaining duration time
		for (AppliedBuffsList::iterator i = entityBuffs->buffs.begin(); i != entityBuffs->buffs.end(); ++i)
		{
			AppliedBuff *appliedBuff = &(*i);
			if (appliedBuff->buff->Is(e->type))
			{
				// same type of buff, refresh duration and stop
				appliedBuff->remainingTime = appliedBuff->buff->GetDuration();
				return FALSE;
			}
		}
		
		// if we get here, then we need to add a new buff to this entity
		
		BuffCollectionComponent *buffs = GetEntityManager()->GetGlobalComponent<BuffCollectionComponent>();
		ASSERT(buffs != NULL);
		
		AppliedBuff newAppliedBuff;
		newAppliedBuff.buff = buffs->buffs->Get(e->type);
		ASSERT(newAppliedBuff.buff != NULL);
		newAppliedBuff.remainingTime = newAppliedBuff.buff->GetDuration();
		
		entityBuffs->buffs.push_back(newAppliedBuff);
		
		newAppliedBuff.buff->OnApply(e->GetEntity());
	}
	
	return FALSE;
}
