#ifndef __GAME_ENTITIES_COMPONENTS_ANIMATIONCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_ANIMATIONCOMPONENT_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../entities/component.h"
#include "../../../framework/support/animationsequence.h"
#include <stl/string.h>
#include <string.h>

const int ANIMATIONCOMPONENT_MAX_NAME_LENGTH = 32;

class AnimationComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "AnimationComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }

	AnimationComponent();
	void Reset();

	uint32_t currentFrame;
	uint32_t nextFrame;
	float frameTime;
	float interpolation;
	BOOL isLooping;
	BOOL currentSequenceCannotBeOverridden;
	BOOL hasAnimationFinishEventBeenTriggered;

	AnimationList sequences;
	AnimationSequence currentSequence;
	char currentSequenceName[ANIMATIONCOMPONENT_MAX_NAME_LENGTH];

	AnimationComponent* AddSequence(const stl::string &name, AnimationSequence sequence);
	AnimationComponent* AddSequence(const stl::string &name, AnimationSequence sequence, float delayOverride);
	AnimationSequence* GetSequence(const stl::string &name);
	
	AnimationComponent* SetSequence(const stl::string &name, BOOL loop = TRUE, BOOL cannotBeOverridden = FALSE);
	AnimationComponent* StopSequence();

	BOOL IsAnimating() const;
	BOOL IsAnimationFinished() const;
};

inline AnimationComponent::AnimationComponent()
{
	Reset();
}

inline void AnimationComponent::Reset()
{
	currentFrame = 0;
	nextFrame = 0;
	frameTime = 0.0f;
	interpolation = 0.0f;
	isLooping = FALSE;
	currentSequenceCannotBeOverridden = FALSE;
	hasAnimationFinishEventBeenTriggered = FALSE;
	sequences.clear();
	currentSequence = AnimationSequence();
	currentSequenceName[0] = '\0';
}

inline AnimationComponent* AnimationComponent::AddSequence(const stl::string &name, AnimationSequence sequence)
{
	sequences[name] = sequence;
	return this;
}

inline AnimationComponent* AnimationComponent::AddSequence(const stl::string &name, AnimationSequence sequence, float delayOverride)
{
	sequence.delay = delayOverride;
	AddSequence(name, sequence);
	return this;
}

inline AnimationSequence* AnimationComponent::GetSequence(const stl::string &name)
{
	AnimationList::iterator i = sequences.find(name);
	if (i == sequences.end())
		return NULL;
	else
		return (&i->second);
}

inline AnimationComponent* AnimationComponent::SetSequence(const stl::string &name, BOOL loop, BOOL cannotBeOverridden)
{
	AnimationSequence *sequence = GetSequence(name);
	if (sequence == NULL)
		return this;
	
	currentSequence = *sequence;
	strncpy(currentSequenceName, name.c_str(), ANIMATIONCOMPONENT_MAX_NAME_LENGTH);

	currentFrame = currentSequence.start;
	nextFrame = currentFrame + 1;
	if (nextFrame > currentSequence.stop)
		nextFrame = currentSequence.stop;

	frameTime = 0.0f;
	interpolation = 0.0f;
	isLooping = loop;
	currentSequenceCannotBeOverridden = cannotBeOverridden;
	hasAnimationFinishEventBeenTriggered = FALSE;

	return this;
}

inline AnimationComponent* AnimationComponent::StopSequence()
{
	currentSequence.start = 0;
	currentSequence.stop = 0;
	currentSequence.delay = 0.0f;
	currentSequence.tileDirectionFrameOffset = 0;
	currentSequence.tileIsMultiDirectional = FALSE;
	currentFrame = 0;
	nextFrame = 0;
	frameTime = 0.0f;
	interpolation = 0.0f;
	isLooping = FALSE;
	currentSequenceCannotBeOverridden = FALSE;
	hasAnimationFinishEventBeenTriggered = FALSE;

	return this;
}

inline BOOL AnimationComponent::IsAnimating() const
{
	if (currentSequence.start != currentSequence.stop)
		return TRUE;
	else
		return FALSE;
}

inline BOOL AnimationComponent::IsAnimationFinished() const
{
	if (IsAnimating() && !isLooping && currentFrame == currentSequence.stop)
		return TRUE;
	else
		return FALSE;
}

#endif

