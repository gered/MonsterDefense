#ifndef __GAME_ENTITIES_COMPONENTS_AFFECTEDBYGRAVITYCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_AFFECTEDBYGRAVITYCOMPONENT_H_INCLUDED__

#include "../../../entities/component.h"

class AffectedByGravityComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "AffectedByGravityComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }
};

#endif

