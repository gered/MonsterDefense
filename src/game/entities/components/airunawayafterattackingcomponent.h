#ifndef __GAME_ENTITIES_COMPONENTS_AIRUNAWAYAFTERATTACKINGCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_AIRUNAWAYAFTERATTACKINGCOMPONENT_H_INCLUDED__

#include "../../../entities/component.h"

class AIRunAwayAfterAttackingComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "AIRunAwayAfterAttackingComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }
	
	AIRunAwayAfterAttackingComponent();
	void Reset();
	
	float waitBeforeRunningTimeMin;
	float waitBeforeRunningTimeMax;
	float waitBeforeRunningTimeLeft;
	float runAwayTimeMin;
	float runAwayTimeMax;
	float runAwayTimeLeft;
	float runAwayAngle;
	float percentChanceToRunAway;
	float percentChanceToKeepRunning;
};


inline AIRunAwayAfterAttackingComponent::AIRunAwayAfterAttackingComponent()
{
	Reset();
}

inline void AIRunAwayAfterAttackingComponent::Reset()
{
	waitBeforeRunningTimeMin = 0.0f;
	waitBeforeRunningTimeMax = 0.0f;
	waitBeforeRunningTimeLeft = 0.0f;
	runAwayTimeMin = 0.0f;
	runAwayTimeMax = 0.0f;
	runAwayTimeLeft = 0.0f;
	runAwayAngle = 0.0f;
	percentChanceToRunAway = 1.0f;
	percentChanceToKeepRunning = 0.0f;
}

#endif

