#ifndef __GAME_ENTITIES_COMPONENTS_AITIMERSCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_AITIMERSCOMPONENT_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../entities/component.h"

class AITimersComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "AITimersComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }
	
	AITimersComponent();
	void Reset();
	
	BOOL hasAfterAttackPauseTimer;
	float afterAttackPauseTimeMin;
	float afterAttackPauseTimeMax;
	float afterAttackPauseTimeLeft;
	
	BOOL hasAfterHurtPauseTimer;
	float afterHurtPauseTimeMin;
	float afterHurtPauseTimeMax;
	float afterHurtPauseTimeLeft;
	
	BOOL hasWaitBeforeAttackingTimer;
	float waitBeforeAttackingTimeMin;
	float waitBeforeAttackingTimeMax;
	float waitBeforeAttackingTimeLeft;
};


inline AITimersComponent::AITimersComponent()
{
	Reset();
}

inline void AITimersComponent::Reset()
{
	hasAfterAttackPauseTimer = FALSE;
	afterAttackPauseTimeMin = 0.0f;
	afterAttackPauseTimeMax = 0.0f;
	afterAttackPauseTimeLeft = 0.0f;
	
	hasAfterHurtPauseTimer = FALSE;
	afterHurtPauseTimeMin = 0.0f;
	afterHurtPauseTimeMax = 0.0f;
	afterHurtPauseTimeLeft = 0.0f;
	
	hasWaitBeforeAttackingTimer = FALSE;
	waitBeforeAttackingTimeMin = 0.0f;
	waitBeforeAttackingTimeMax = 0.0f;
	waitBeforeAttackingTimeLeft = 0.0f;
}

#endif

