#ifndef __GAME_ENTITIES_COMPONENTS_AIFOLLOWANDATTACKTARGETCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_AIFOLLOWANDATTACKTARGETCOMPONENT_H_INCLUDED__

#include "../../../entities/component.h"

class AIFollowAndAttackTargetComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "AIFollowAndAttackTargetComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }
};

#endif

