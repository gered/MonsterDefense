#ifndef __GAME_ENTITIES_COMPONENTS_POWERUPCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_POWERUPCOMPONENT_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../entities/component.h"
#include "../powerups/powerup.h"
#include <stl/string.h>

class PowerUpComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "PowerUpComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }

	PowerUpComponent();
	void Reset();

	PowerUp *powerUp;
	BOOL shouldRaisePowerUpOnCollect;
};

inline PowerUpComponent::PowerUpComponent()
{
	Reset();
}

inline void PowerUpComponent::Reset()
{
	powerUp = NULL;
	shouldRaisePowerUpOnCollect = FALSE;
}

#endif

