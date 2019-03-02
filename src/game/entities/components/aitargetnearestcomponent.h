#ifndef __GAME_ENTITIES_COMPONENTS_AITARGETNEARESTCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_AITARGETNEARESTCOMPONENT_H_INCLUDED__

#include "../../../entities/component.h"

class AITargetNearestComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "AITargetNearestComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }
	
	AITargetNearestComponent();
	void Reset();
	
	float timeBeforeNextAcquire;
	float maximumDistanceToFindTarget;
	float maximumDistanceToKeepTarget;
	float percentChanceToSuccessfullyAcquireTarget;
};

inline AITargetNearestComponent::AITargetNearestComponent()
{
	Reset();
}

inline void AITargetNearestComponent::Reset()
{
	timeBeforeNextAcquire = 0.0f;
	maximumDistanceToFindTarget = 0.0f;
	maximumDistanceToKeepTarget = 0.0f;
	percentChanceToSuccessfullyAcquireTarget = 1.0f;
}

#endif

