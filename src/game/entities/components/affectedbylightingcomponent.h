#ifndef __GAME_ENTITIES_COMPONENTS_AFFECTEDBYLIGHTINGCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_AFFECTEDBYLIGHTINGCOMPONENT_H_INCLUDED__

#include "../../../entities/component.h"

class AffectedByLightingComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "AffectedByLightingComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }
};

#endif

