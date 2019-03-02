#ifndef __GAME_ENTITIES_COMPONENTS_VELOCITYCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_VELOCITYCOMPONENT_H_INCLUDED__

#include "../../../entities/component.h"
#include "../../../framework/math/vector3.h"

class VelocityComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "VelocityComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }

	VelocityComponent();
	void Reset();

	Vector3 velocity;
};

inline VelocityComponent::VelocityComponent()
{
	Reset();
}

inline void VelocityComponent::Reset()
{
	velocity = ZERO_VECTOR;
}

#endif

