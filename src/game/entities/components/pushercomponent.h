#ifndef __GAME_ENTITIES_COMPONENTS_PUSHERCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_PUSHERCOMPONENT_H_INCLUDED__

#include "../../../entities/component.h"
#include "../../physicsconstants.h"

class PusherComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "PusherComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }
	
	PusherComponent();
	void Reset();
	
	float strength;
};

inline PusherComponent::PusherComponent()
{
	Reset();
}

inline void PusherComponent::Reset()
{
	strength = STANDARD_PUSH_STRENGTH;
}

#endif

