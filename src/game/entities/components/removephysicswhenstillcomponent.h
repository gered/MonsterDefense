#ifndef __GAME_ENTITIES_COMPONENTS_REMOVEPHYSICSWHENSTILLCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_REMOVEPHYSICSWHENSTILLCOMPONENT_H_INCLUDED__

#include "../../../entities/component.h"

class RemovePhysicsWhenStillComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "RemovePhysicsWhenStillComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }
};

#endif

