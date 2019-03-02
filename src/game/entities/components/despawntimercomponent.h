#ifndef __GAME_ENTITIES_COMPONENTS_DESPAWNTIMERCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_DESPAWNTIMERCOMPONENT_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../entities/component.h"

class DespawnTimerComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "DespawnTimerComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }
	
	DespawnTimerComponent();
	void Reset();
	
	float timeLeft;
	float startFlickeringAtTimeLeft;
	BOOL dontTriggerDespawnEvent;
};

inline DespawnTimerComponent::DespawnTimerComponent()
{
	Reset();
}

inline void DespawnTimerComponent::Reset()
{
	timeLeft = 0.0f;
	startFlickeringAtTimeLeft = 0.0f;
	dontTriggerDespawnEvent = FALSE;
}

#endif

