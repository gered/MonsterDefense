#ifndef __GAME_ENTITIES_COMPONENTS_AITARGETPLAYERCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_AITARGETPLAYERCOMPONENT_H_INCLUDED__

#include "../../../entities/component.h"

class AITargetPlayerComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "AITargetPlayerComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }
	
	AITargetPlayerComponent();
	void Reset();
	
	float timeBeforeNextAcquire;
	float maximumDistanceToFindTarget;
	float maximumDistanceToKeepTarget;
	float percentChanceToSuccessfullyAcquireTarget;
};

inline AITargetPlayerComponent::AITargetPlayerComponent()
{
	Reset();
}

inline void AITargetPlayerComponent::Reset()
{
	timeBeforeNextAcquire = 0.0f;
	maximumDistanceToFindTarget = 0.0f;
	maximumDistanceToKeepTarget = 0.0f;
	percentChanceToSuccessfullyAcquireTarget = 1.0f;
}

#endif

