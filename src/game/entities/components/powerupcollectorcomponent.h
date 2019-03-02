#ifndef __GAME_ENTITIES_COMPONENTS_POWERUPCOLLECTORCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_POWERUPCOLLECTORCOMPONENT_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../entities/component.h"

class PowerUpCollectorComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "PowerUpCollectorComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }
};

#endif

