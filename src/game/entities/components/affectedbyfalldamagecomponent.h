#ifndef __GAME_ENTITIES_COMPONENTS_AFFECTEDBYFALLDAMAGECOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_AFFECTEDBYFALLDAMAGECOMPONENT_H_INCLUDED__

#include "../../../entities/component.h"

class AffectedByFallDamageComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "AffectedByFallDamageComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }
};

#endif

