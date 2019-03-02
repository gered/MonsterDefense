#ifndef __GAME_ENTITIES_COMPONENTS_RENDEROFFSETCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_RENDEROFFSETCOMPONENT_H_INCLUDED__

#include "../../../entities/component.h"
#include "../../../framework/math/vector3.h"

class RenderOffsetComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "RenderOffsetComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }

	RenderOffsetComponent();
	void Reset();

	Vector3 offset;
};

inline RenderOffsetComponent::RenderOffsetComponent()
{
	Reset();
}

inline void RenderOffsetComponent::Reset()
{
	offset = ZERO_VECTOR;
}

#endif

