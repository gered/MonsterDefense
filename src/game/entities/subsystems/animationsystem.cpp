#include "../../../framework/debug.h"

#include "../../../framework/common.h"
#include "animationsystem.h"
#include "../components/animationcomponent.h"
#include "../components/autoanimationcomponent.h"
#include "../components/billboardcomponent.h"
#include "../components/keyframemeshcomponent.h"
#include "../components/orientationxzcomponent.h"
#include "../events/animationchangeevent.h"
#include "../events/animationfinishedevent.h"
#include "../../direction.h"
#include "../../../entities/entity.h"
#include "../../../entities/entitymanager.h"
#include "../../../contexts/rendercontext.h"
#include "../../../events/event.h"
#include "../../../events/eventmanager.h"
#include "../../../framework/graphics/graphicsdevice.h"
#include "../../../framework/math/camera.h"
#include "../../../framework/math/vector3.h"

AnimationSystem::AnimationSystem(EntityManager *entityManager, EventManager *eventManager)
	: ComponentSystem(entityManager, eventManager)
{
	STACK_TRACE;
	ListenFor<AnimationChangeEvent>();
}

AnimationSystem::~AnimationSystem()
{
	STACK_TRACE;
	StopListeningFor<AnimationChangeEvent>();
}

void AnimationSystem::OnUpdate(float delta)
{
	STACK_TRACE;
	EntityList list;
	GetEntityManager()->GetAllWith<AnimationComponent>(list);
	for (EntityList::iterator i = list.begin(); i != list.end(); ++i)
	{
		Entity *entity = *i;
		AnimationComponent *animation = entity->Get<AnimationComponent>();

		if (!animation->IsAnimating())
			continue;

		// only update active animation sequences
		if (!animation->IsAnimationFinished())
		{
			animation->frameTime += delta;
			animation->interpolation += delta / animation->currentSequence.delay;

			if (animation->frameTime >= animation->currentSequence.delay)
			{
				// move to the next frame
				animation->frameTime = 0.0f;
				animation->interpolation = 0.0f;

				++animation->currentFrame;
				if (animation->currentFrame > animation->currentSequence.stop)
				{
					animation->currentFrame = animation->currentSequence.start;
					if (!animation->isLooping)
					{
						animation->currentSequence.start = animation->currentSequence.stop;
						animation->currentFrame = animation->currentSequence.stop;
						animation->nextFrame = animation->currentFrame;

						continue;
					}
				}

				++animation->nextFrame;
				if (animation->nextFrame > animation->currentSequence.stop)
				{
					if (!animation->isLooping)
						animation->nextFrame = animation->currentSequence.stop;
					else
						animation->nextFrame = animation->currentSequence.start;
				}
			}
		}
		else
		{
			// animation has finished (it is not looping and has reached the end)
			if (!animation->hasAnimationFinishEventBeenTriggered)
			{
				// and we haven't yet raised an event to signal this
				AnimationFinishedEvent finishedEvent(entity);
				GetEventManager()->Trigger(&finishedEvent);
				
				animation->hasAnimationFinishEventBeenTriggered = TRUE;
			}
		}
	}
}

void AnimationSystem::OnRender(RenderContext *context)
{
	STACK_TRACE;
	const Camera *camera = context->GetGraphicsDevice()->GetCamera();

	// update texture atlas frame indexes for tile-animation components with
	// multi-directional animations based on the current camera orientation
	EntityList list;
	GetEntityManager()->GetAllWith<AnimationComponent>(list);
	for (EntityList::iterator i = list.begin(); i != list.end(); ++i)
	{
		Entity *entity = *i;
		AnimationComponent *animation = entity->Get<AnimationComponent>();

		BillboardComponent *billboard = entity->Get<BillboardComponent>();
		if (billboard != NULL)
		{
			OrientationXZComponent *orientation = entity->Get<OrientationXZComponent>();
			UpdateBillboardFrame(billboard, animation, orientation, camera);
		}
		else
		{
			KeyframeMeshComponent *keyframeMesh = entity->Get<KeyframeMeshComponent>();
			if (keyframeMesh != NULL)
				UpdateKeyframeMeshFrame(keyframeMesh, animation);
		}
	}
}

BOOL AnimationSystem::Handle(const Event *event)
{
	STACK_TRACE;
	/*
	if (event->Is<EntityStateChangeEvent>())
	{ 
		const EntityStateChangeEvent *stateChangeEvent = event->As<EntityStateChangeEvent>();
		Entity *entity = stateChangeEvent->GetEntity();
		AutoAnimationComponent *autoAnimation = entity->Get<AutoAnimationComponent>();
		if (autoAnimation != NULL)
		{
			const EntityStateSequenceProps *sequenceProps = autoAnimation->Get(stateChangeEvent->state);
			if (sequenceProps != NULL)
			{
				AnimationComponent *animation = entity->Get<AnimationComponent>();
				ASSERT(animation != NULL);

				if (animation->currentSequenceCannotBeOverridden == FALSE || animation->IsAnimationFinished())
					animation->SetSequence(sequenceProps->first, sequenceProps->second.loop, sequenceProps->second.cannotBeOverridden);
			}
		}
	}
	*/
	
	if (event->Is<AnimationChangeEvent>())
	{
		const AnimationChangeEvent *e = event->As<AnimationChangeEvent>();
		Entity *entity = e->GetEntity();
		
		if (e->changeToSequenceForEntityState)
		{
			// event is for an animation change to a sequence based on an entity
			// state provided in the event info. requires an AutoAnimationComponent
			AutoAnimationComponent *autoAnimation = entity->Get<AutoAnimationComponent>();
			if (autoAnimation != NULL)
			{
				// if the entity's auto animation info has a sequence defined for
				// the state specified in the event, then switch to that sequence
				const EntityStateSequenceProps *sequenceProps = autoAnimation->Get(e->state);
				if (sequenceProps != NULL)
				{
					AnimationComponent *animation = entity->Get<AnimationComponent>();
					ASSERT(animation != NULL);
					
					if (animation->currentSequenceCannotBeOverridden == FALSE || animation->IsAnimationFinished() || e->overrideExisting)
					{
						animation->SetSequence(sequenceProps->first, sequenceProps->second.loop, sequenceProps->second.cannotBeOverridden);
					}
				}

			}
		}
		else
		{
			ASSERT(e->sequenceName.length() > 0);
			
			// we're just changing to a named animation sequence directly
			AnimationComponent *animation = entity->Get<AnimationComponent>();
			animation->SetSequence(e->sequenceName);
		}
	}

	return FALSE;
}

void AnimationSystem::UpdateBillboardFrame(BillboardComponent *billboard, AnimationComponent *animation, OrientationXZComponent *orientation, const Camera *camera)
{
	STACK_TRACE;
	if (billboard->textureAtlas == NULL)
		return;

	if (orientation != NULL && animation->currentSequence.tileIsMultiDirectional)
	{
		// get the direction to point the entity in based on it's own facing
		// direction and the direction the camera is pointed in
		uint32_t direction = (uint32_t)GetFacingDirectionAdjustedForCamera(orientation->angle, camera->GetOrientation());

		// +1 because sequences will be specified using start/stop frames that
		// are a part of the sequence. e.g. for a 2 frame sequence using frames
		// 10 and 11, you would specify start=10 and stop=11, so without +1, the
		// below calc would find the length of this sequence to be 1 which is
		// not correct...
		uint32_t sequenceLength = animation->currentSequence.stop - animation->currentSequence.start + 1;

		// offset between frames for different directions
		uint32_t offset = direction * animation->currentSequence.tileDirectionFrameOffset;

		// set the frame index
		billboard->textureAtlasTileIndex = animation->currentFrame + (sequenceLength * direction) + offset;
	}
	else
		billboard->textureAtlasTileIndex = animation->currentFrame;
}

void AnimationSystem::UpdateKeyframeMeshFrame(KeyframeMeshComponent *keyframeMesh, AnimationComponent *animation)
{
	STACK_TRACE;
	keyframeMesh->startFrame = animation->currentFrame;
	keyframeMesh->endFrame = animation->nextFrame;
	keyframeMesh->interpolation = animation->interpolation;
}


