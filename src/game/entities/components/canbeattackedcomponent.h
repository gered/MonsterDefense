#ifndef __GAME_ENTITIES_COMPONENTS_CANBEATTACKEDCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_CANBEATTACKEDCOMPONENT_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../entities/component.h"

class CanBeAttackedComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "CanBeAttackedComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }
	
	CanBeAttackedComponent();
	void Reset();

	BOOL byPlayer;
	BOOL byNPC;
};

inline CanBeAttackedComponent::CanBeAttackedComponent()
{
	Reset();
}

inline void CanBeAttackedComponent::Reset()
{
	byPlayer = FALSE;
	byNPC = FALSE;
}

#endif

