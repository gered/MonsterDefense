#ifndef __GAME_ENTITIES_COMPONENTS_NOTVISIBLECOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_NOTVISIBLECOMPONENT_H_INCLUDED__

#include "../../../entities/component.h"

class NotVisibleComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "NotVisibleComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }
};

#endif

