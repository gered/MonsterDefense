#ifndef __GAME_ENTITIES_COMPONENTS_CHESTOPENERCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_CHESTOPENERCOMPONENT_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../entities/component.h"

class ChestOpenerComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "ChestOpenerComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }

	ChestOpenerComponent();
	void Reset();
	
	BOOL hidePowerUpsOnOpen;
};

inline ChestOpenerComponent::ChestOpenerComponent()
{
	Reset();
}

inline void ChestOpenerComponent::Reset()
{
	hidePowerUpsOnOpen = FALSE;
}

#endif

