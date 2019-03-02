#ifndef __GAME_ENTITIES_COMPONENTS_DEADCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_DEADCOMPONENT_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../entities/component.h"

class DeadComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "DeadComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }

	DeadComponent();
	void Reset();

	float time;
};

inline DeadComponent::DeadComponent()
{
	Reset();
}

inline void DeadComponent::Reset()
{
	time = 0.0f;
}

#endif

