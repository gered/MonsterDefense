#ifndef __GAME_ENTITIES_COMPONENTS_STATECOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_STATECOMPONENT_H_INCLUDED__

#include "../../../entities/component.h"
#include "../entitystates.h"

class StateComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "StateComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }
	
	StateComponent();
	void Reset();
	
	ENTITYSTATES GetCurrentState();
	
	ENTITYSTATES state;
	ENTITYSTATES tempState;
	BOOL isInTempState;
};

inline StateComponent::StateComponent()
{
	Reset();
}

inline void StateComponent::Reset()
{
	state = ENTITYSTATE_NONE;
	tempState = ENTITYSTATE_NONE;
	isInTempState = FALSE;
}

inline ENTITYSTATES StateComponent::GetCurrentState()
{
	if (isInTempState)
		return tempState;
	else
		return state;
}

#endif
