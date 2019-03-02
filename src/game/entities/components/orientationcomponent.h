#ifndef __GAME_ENTITIES_COMPONENTS_ORIENTATIONCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_ORIENTATIONCOMPONENT_H_INCLUDED__

#include "../../../entities/component.h"
#include "../../../framework/math/vector3.h"

class OrientationComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "OrientationComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }

	OrientationComponent();
	void Reset();

	Vector3 orientation;
};

inline OrientationComponent::OrientationComponent()
{
	Reset();
}

inline void OrientationComponent::Reset()
{
	orientation = ZERO_VECTOR;
}

#endif

