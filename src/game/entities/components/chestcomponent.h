#ifndef __GAME_ENTITIES_COMPONENTS_CHESTCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_CHESTCOMPONENT_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../entities/component.h"
#include "../powerups/powerup.h"

class ChestComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "ChestComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }

	ChestComponent();
	void Reset();

	BOOL isOpen;
	PowerUp *powerUp;
};

inline ChestComponent::ChestComponent()
{
	Reset();
}

inline void ChestComponent::Reset()
{
	isOpen = FALSE;
	powerUp = NULL;
}

#endif
