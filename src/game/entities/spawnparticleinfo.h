#ifndef __GAME_ENTITIES_SPAWNPARTICLEINFO_H_INCLUDED__
#define __GAME_ENTITIES_SPAWNPARTICLEINFO_H_INCLUDED__

#include "../../framework/common.h"
#include "../../entities/entitypreset.h"
#include "../../framework/graphics/color.h"
#include "../../framework/math/vector3.h"

struct SpawnParticleInfo
{
	ENTITYPRESET_TYPE particlePreset;
	uint32_t minimumAmount;
	uint32_t maximumAmount;
	Vector3 position;
	BOOL includePositionOffset;
	BOOL spawnInRadiusAroundPosition;
	BOOL canOnlySpawnOnXZ;
	float radius;
	Color lowColor;
	Color highColor;
	
	SpawnParticleInfo();
};

inline SpawnParticleInfo::SpawnParticleInfo()
{
	particlePreset = NULL;
	minimumAmount = 0;
	maximumAmount = 0;
	position = ZERO_VECTOR;
	includePositionOffset = FALSE;
	spawnInRadiusAroundPosition = FALSE;
	canOnlySpawnOnXZ = FALSE;
	radius = 0.0f;
	lowColor = COLOR_WHITE;
	highColor = COLOR_WHITE;
}

#endif
