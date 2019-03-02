#ifndef __GAME_ENTITIES_COMPONENTS_FLOATBOUNCECOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_FLOATBOUNCECOMPONENT_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../entities/component.h"

class FloatBounceComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "FloatBounceComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }

	FloatBounceComponent();
	void Reset();

	float minY;
	float maxY;
	float position;
	BOOL isMovingUp;
};

inline FloatBounceComponent::FloatBounceComponent()
{
	Reset();
}

inline void FloatBounceComponent::Reset()
{
	minY = 0.0f;
	maxY = 0.0f;
	position = 0.0f;
	isMovingUp = FALSE;
}

#endif

