#ifndef __GAME_ENTITIES_PRESETS_PARTICLES_PARTICLEPRESETPROPERTIES_H_INCLUDED__
#define __GAME_ENTITIES_PRESETS_PARTICLES_PARTICLEPRESETPROPERTIES_H_INCLUDED__

#include "../../../../framework/common.h"
#include "../../../../entities/entitypresetargs.h"
#include "../../../../framework/graphics/color.h"
#include "../../../../framework/math/vector3.h"

struct ParticlePresetProperties : public EntityPresetArgs
{
	static ENTITYPRESETARGS_TYPE GetType()
	{
		static ENTITYPRESETARGS_TYPE typeName = "ParticlePresetProperties";
		return typeName;
	}
	ENTITYPRESETARGS_TYPE GetTypeOf() const                 { return GetType(); }
	
	ParticlePresetProperties();

	Vector3 spawnPosition;
	BOOL canSpawnAroundPosition;
	BOOL canOnlySpawnAroundPositionOnXZ;
	float radius;
	Color lowColor;
	Color highColor;
};

inline ParticlePresetProperties::ParticlePresetProperties()
{
	spawnPosition = ZERO_VECTOR;
	canSpawnAroundPosition = FALSE;
	canOnlySpawnAroundPositionOnXZ = FALSE;
	radius = 0.0f;
	lowColor = COLOR_WHITE;
	highColor = COLOR_WHITE;
}

#endif
