#ifndef __GAME_ENTITIES_COMPONENTS_ORIENTATIONXZCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_ORIENTATIONXZCOMPONENT_H_INCLUDED__

#include "../../../entities/component.h"
#include "../../direction.h"

class OrientationXZComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "OrientationXZComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }

	OrientationXZComponent();
	void Reset();

	float angle;

	FacingDirection GetFacing() const;
};

inline OrientationXZComponent::OrientationXZComponent()
{
	Reset();
}

inline void OrientationXZComponent::Reset()
{
	angle = 0.0f;
}

inline FacingDirection OrientationXZComponent::GetFacing() const
{
	return GetFacingDirection(angle);
}

#endif

