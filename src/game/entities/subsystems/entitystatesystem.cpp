#include "../../../framework/debug.h"
#include "../../../framework/common.h"

#include "entitystatesystem.h"
#include "../components/statecomponent.h"
#include "../events/animationchangeevent.h"
#include "../events/animationfinishedevent.h"
#include "../events/entitystatechangeevent.h"
#include "../events/entityexitingtempstateevent.h"
#include "../events/entityintempstateevent.h"
#include "../events/spawnedevent.h"
#include "../events/despawnedevent.h"
#include "../../../entities/entity.h"
#include "../../../entities/entitymanager.h"
#include "../../../entities/components/inactivecomponent.h"
#include "../../../events/eventmanager.h"

EntityStateSystem::EntityStateSystem(EntityManager *entityManager, EventManager *eventManager)
: ComponentSystem(entityManager, eventManager)
{
	STACK_TRACE;
	ListenFor<EntityStateChangeEvent>();
	ListenFor<EntityExitingTempStateEvent>();
	ListenFor<AnimationFinishedEvent>();
	ListenFor<SpawnedEvent>();
	ListenFor<DespawnedEvent>();
}

EntityStateSystem::~EntityStateSystem()
{
	STACK_TRACE;
	StopListeningFor<EntityStateChangeEvent>();
	StopListeningFor<EntityExitingTempStateEvent>();
	StopListeningFor<AnimationFinishedEvent>();
	StopListeningFor<SpawnedEvent>();
	StopListeningFor<DespawnedEvent>();
}

void EntityStateSystem::OnUpdate(float delta)
{
	STACK_TRACE;
}

BOOL EntityStateSystem::Handle(const Event *event)
{
	STACK_TRACE;
	if (event->Is<EntityStateChangeEvent>())
	{
		const EntityStateChangeEvent *e = event->As<EntityStateChangeEvent>();		
		StateComponent *state = e->GetEntity()->Get<StateComponent>();
		if (state != NULL)
		{
			if (e->isTempChange)
			{
				ASSERT((e->state == ENTITYSTATE_DEAD) == FALSE);
				
				// set temp state first
				state->tempState = e->state;
				state->isInTempState = TRUE;
				
				// HACK: this was mainly added to prevent the death animation from
				//       playing twice in a row in a somewhat rare scenario. however
				//       this does seem to be a sensible check regardless so maybe
				//       it's not really a hack...?
				BOOL needToChangeAnimation = (state->state != state->tempState);

				// trigger state change event
				EntityInTempStateEvent tempStateEvent(e->GetEntity());
				GetEventManager()->Trigger(&tempStateEvent);
				
				if (needToChangeAnimation)
				{
					// trigger animation change event
					AnimationChangeEvent animationChangeEvent(e->GetEntity());
					animationChangeEvent.changeToSequenceForEntityState = TRUE;
					animationChangeEvent.state = e->state;
					GetEventManager()->Trigger(&animationChangeEvent);
				}
			}
			else
			{
				//ASSERT((e->state == ENTITYSTATE_DEAD && state->isInTempState == TRUE) == FALSE);
				
				// HACK: this will (hopefully?) stop entity's from doing their
				//       death animation and then popping up to idle before
				//       despawning. need to verify (very hard to reproduce!)
				if (state->state == ENTITYSTATE_DEAD && e->state == ENTITYSTATE_IDLE)
					return FALSE;
				
				// HACK: this was mainly added to prevent the death animation from
				//       playing twice in a row in a somewhat rare scenario. however
				//       this does seem to be a sensible check regardless so maybe
				//       it's not really a hack...?
				BOOL needToChangeAnimation = (state->state != e->state);
				
				// set non-temp state
				state->state = e->state;
				
				if (e->clearExistingTempStateInfo)
				{
					state->tempState = ENTITYSTATE_NONE;
					state->isInTempState = FALSE;
				}
				
				if (needToChangeAnimation)
				{
					// trigger animation change event
					AnimationChangeEvent animationChangeEvent(e->GetEntity());
					animationChangeEvent.changeToSequenceForEntityState = TRUE;
					animationChangeEvent.state = e->state;
					
					// HACK: this ensures that the death animation will play
					animationChangeEvent.overrideExisting = (e->state == ENTITYSTATE_DEAD);
					
					GetEventManager()->Trigger(&animationChangeEvent);
				}
			}
		}
	}
	else if (event->Is<EntityExitingTempStateEvent>())
	{
		const EntityExitingTempStateEvent *e = event->As<EntityExitingTempStateEvent>();
		StateComponent *state = e->GetEntity()->Get<StateComponent>();
		ASSERT(state != NULL);
		if (state != NULL)
		{
			// HACK: this will (hopefully?) stop entity's from doing their
			//       death animation and then popping up to idle before
			//       despawning. need to verify (very hard to reproduce!)
			if (state->state == ENTITYSTATE_DEAD)
				return FALSE;

			// HACK: this was mainly added to prevent the death animation from
			//       playing twice in a row in a somewhat rare scenario. however
			//       this does seem to be a sensible check regardless so maybe
			//       it's not really a hack...?
			BOOL needToChangeAnimation = (state->state != state->tempState);
			
			// clear temp state info
			ASSERT(state->isInTempState == TRUE);
			state->isInTempState = FALSE;
			state->tempState = ENTITYSTATE_NONE;
			
			if (needToChangeAnimation)
			{
				// trigger animation change event for whatever the non-temp state is
				AnimationChangeEvent animationChangeEvent(e->GetEntity());
				animationChangeEvent.changeToSequenceForEntityState = TRUE;
				animationChangeEvent.state = state->state;
				GetEventManager()->Trigger(&animationChangeEvent);
			}
		}
	}
	else if (event->Is<AnimationFinishedEvent>())
	{
		const AnimationFinishedEvent *e = event->As<AnimationFinishedEvent>();
		Entity *entity = e->GetEntity();
		StateComponent *state = entity->Get<StateComponent>();
		
		// TODO: all this assumes that an AnimationFinishedEvent triggered for 
		//       an entity with a StateComponent marked as "in temp state"
		//       should *always* trigger a EntityExitingTempStateEvent. Maybe
		//       this should be set up some other way with less assumptions?
		if (state != NULL && state->isInTempState)
		{
			EntityExitingTempStateEvent exitTempStateEvent(entity);
			GetEventManager()->Trigger(&exitTempStateEvent);
		}
	}
	else if (event->Is<SpawnedEvent>())
	{

	}
	else if (event->Is<DespawnedEvent>())
	{
		const DespawnedEvent *e = event->As<DespawnedEvent>();
		e->GetEntity()->Add<InactiveComponent>();
	}
	
	return FALSE;
}
