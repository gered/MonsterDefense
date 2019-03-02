#ifndef __GAME_ENTITIES_COMPONENTS_DROPSITEMSONDEATHCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_DROPSITEMSONDEATHCOMPONENT_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../entities/component.h"
#include <stl/string.h>

class DropsItemsOnDeathComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "DropsItemsOnDeathComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }

	DropsItemsOnDeathComponent();
	void Reset();
};

inline DropsItemsOnDeathComponent::DropsItemsOnDeathComponent()
{
	Reset();
}

inline void DropsItemsOnDeathComponent::Reset()
{
}

#endif

