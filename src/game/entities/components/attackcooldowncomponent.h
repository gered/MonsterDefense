#ifndef __GAME_ENTITIES_COMPONENTS_ATTACKCOOLDOWNCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_ATTACKCOOLDOWNCOMPONENT_H_INCLUDED__

#include "../../../entities/component.h"

class AttackCooldownComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "AttackCooldownComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }

	AttackCooldownComponent();
	void Reset();

	float time;
};

inline AttackCooldownComponent::AttackCooldownComponent()
{
	Reset();
}

inline void AttackCooldownComponent::Reset()
{
	time = 0.0f;
}

#endif

