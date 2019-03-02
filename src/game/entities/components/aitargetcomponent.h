#ifndef __GAME_ENTITIES_COMPONENTS_AITARGETCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_AITARGETCOMPONENT_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../entities/component.h"

class Entity;

class AITargetComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "AITargetComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }
	
	AITargetComponent();
	void Reset();
	
	Entity *target;
};

inline AITargetComponent::AITargetComponent()
{
	Reset();
}

inline void AITargetComponent::Reset()
{
	target = NULL;
}

#endif

