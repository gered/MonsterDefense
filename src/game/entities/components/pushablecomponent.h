#ifndef __GAME_ENTITIES_COMPONENTS_PUSHABLECOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_PUSHABLECOMPONENT_H_INCLUDED__

#include "../../../entities/component.h"
#include "../../physicsconstants.h"

class PushableComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "PushableComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }
	
	PushableComponent();
	void Reset();

	float resistance;
};

inline PushableComponent::PushableComponent()
{
	Reset();
}

inline void PushableComponent::Reset()
{
	resistance = STANDARD_PUSH_RESISTANCE;
}

#endif

