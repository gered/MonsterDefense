#ifndef __GAME_ENTITIES_COMPONENTS_POSITIONCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_POSITIONCOMPONENT_H_INCLUDED__

#include "../../../entities/component.h"
#include "../../../framework/math/vector3.h"

class PositionComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "PositionComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }

	PositionComponent();
	void Reset();

	Vector3 position;
};

inline PositionComponent::PositionComponent()
{
	Reset();
}

inline void PositionComponent::Reset()
{
	position = ZERO_VECTOR;
}

#endif

