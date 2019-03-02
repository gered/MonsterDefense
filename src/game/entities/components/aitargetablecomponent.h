#ifndef __GAME_ENTITIES_COMPONENTS_AITARGETABLECOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_AITARGETABLECOMPONENT_H_INCLUDED__

#include "../../../entities/component.h"

class AITargetableComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "AITargetableComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }
};

#endif

