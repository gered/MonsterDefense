#ifndef __GAME_ENTITIES_COMPONENTS_PLAYERCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_PLAYERCOMPONENT_H_INCLUDED__

#include "../../../entities/component.h"

class PlayerComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "PlayerComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }
};

#endif

