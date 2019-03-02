#ifndef __GAME_ENTITIES_COMPONENTS_SCALECOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_SCALECOMPONENT_H_INCLUDED__

#include "../../../entities/component.h"
#include "../../../framework/math/vector3.h"

class ScaleComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "ScaleComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }

	ScaleComponent();
	void Reset();

	Vector3 scale;
};

inline ScaleComponent::ScaleComponent()
{
	Reset();
}

inline void ScaleComponent::Reset()
{
	scale = Vector3(1.0f, 1.0f, 1.0f);
}

#endif

