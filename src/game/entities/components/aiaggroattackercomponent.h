#ifndef __GAME_ENTITIES_COMPONENTS_AIAGGROATTACKERCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_AIAGGROATTACKERCOMPONENT_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../entities/component.h"

class AIAggroAttackerComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "AIAggroAttackerComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }
	
	AIAggroAttackerComponent();
	void Reset();

	BOOL onlyIfNoExistingTarget;
	float maximumAggroDistance;
	BOOL currentlyAggroToTarget;
	float timeBeforeNextDistanceCheck;
	float percentChanceToSuccessfullyAcquireTarget;
};

inline AIAggroAttackerComponent::AIAggroAttackerComponent()
{
	Reset();
}

inline void AIAggroAttackerComponent::Reset()
{
	onlyIfNoExistingTarget = FALSE;
	maximumAggroDistance = 0.0f;
	currentlyAggroToTarget = FALSE;
	timeBeforeNextDistanceCheck = 0.0f;
	percentChanceToSuccessfullyAcquireTarget = 1.0f;
}

#endif

