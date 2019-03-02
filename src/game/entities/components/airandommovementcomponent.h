#ifndef __GAME_ENTITIES_COMPONENTS_AIRANDOMMOVEMENTCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_AIRANDOMMOVEMENTCOMPONENT_H_INCLUDED__

#include "../../../entities/component.h"

class AIRandomMovementComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "AIRandomMovementComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }

	AIRandomMovementComponent();
	void Reset();

	float percentChanceToStartMoving;
	float timeToMoveMin;
	float timeToMoveMax;
	float timeToMoveLeft;
	float timeBeforeNextMovementAttempt;
};


inline AIRandomMovementComponent::AIRandomMovementComponent()
{
	Reset();
}

inline void AIRandomMovementComponent::Reset()
{
	percentChanceToStartMoving = 0.0f;
	timeToMoveMin = 0.0f;
	timeToMoveMax = 0.0f;
	timeToMoveLeft = 0.0f;
	timeBeforeNextMovementAttempt = 0.0f;
}

#endif

