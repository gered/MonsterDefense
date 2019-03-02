#ifndef __GAME_ENTITIES_COMPONENTS_HURTCOOLDOWNCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_HURTCOOLDOWNCOMPONENT_H_INCLUDED__

#include "../../../entities/component.h"

class HurtCooldownComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "HurtCooldownComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }

	HurtCooldownComponent();
	void Reset();

	float time;
};

inline HurtCooldownComponent::HurtCooldownComponent()
{
	Reset();
}

inline void HurtCooldownComponent::Reset()
{
	time = 0.0f;
}

#endif

