#ifndef __GAME_ENTITIES_COMPONENTS_POSITIONOFFSETCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_POSITIONOFFSETCOMPONENT_H_INCLUDED__

#include "../../../entities/component.h"
#include "../../../framework/math/vector3.h"

class PositionOffsetComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "PositionOffsetComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }

	PositionOffsetComponent();
	void Reset();

	Vector3 offset;
};

inline PositionOffsetComponent::PositionOffsetComponent()
{
	Reset();
}

inline void PositionOffsetComponent::Reset()
{
	offset = ZERO_VECTOR;
}

#endif

