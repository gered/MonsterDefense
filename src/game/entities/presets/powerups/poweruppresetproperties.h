#ifndef __GAME_ENTITIES_PRESETS_POWERUPS_POWERUPPRESETPROPERTIES_H_INCLUDED__
#define __GAME_ENTITIES_PRESETS_POWERUPS_POWERUPPRESETPROPERTIES_H_INCLUDED__

#include "../../../../framework/common.h"
#include "../../../../entities/entitypresetargs.h"
#include "../../powerups/powerup.h"

struct PowerUpPresetProperties : public EntityPresetArgs
{
	static ENTITYPRESETARGS_TYPE GetType()
	{
		static ENTITYPRESETARGS_TYPE typeName = "PowerUpPresetProperties";
		return typeName;
	}
	ENTITYPRESETARGS_TYPE GetTypeOf() const                 { return GetType(); }

	PowerUp *powerUp;
	BOOL addPushForce;
	BOOL shouldRaisePowerUpOnCollect;

	PowerUpPresetProperties();
};

inline PowerUpPresetProperties::PowerUpPresetProperties()
{
	powerUp = NULL;
	addPushForce = FALSE;
	shouldRaisePowerUpOnCollect = FALSE;
}

#endif
