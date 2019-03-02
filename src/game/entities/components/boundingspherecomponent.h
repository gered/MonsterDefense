#ifndef __GAME_ENTITIES_COMPONENTS_BOUNDINGSPHERECOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_BOUNDINGSPHERECOMPONENT_H_INCLUDED__

#include "../../../entities/component.h"
#include "../../../framework/math/boundingsphere.h"

class BoundingSphereComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "BoundingSphereComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }

	BoundingSphereComponent();
	void Reset();

	BoundingSphere bounds;
};

inline BoundingSphereComponent::BoundingSphereComponent()
{
	Reset();
}

inline void BoundingSphereComponent::Reset()
{
	bounds = BoundingSphere();
}

#endif

