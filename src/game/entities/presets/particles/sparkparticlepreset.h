#ifndef __GAME_ENTITIES_PRESETS_PARTICLES_SPARKPARTICLEPRESET_H_INCLUDED__
#define __GAME_ENTITIES_PRESETS_PARTICLES_SPARKPARTICLEPRESET_H_INCLUDED__

#include "../../../../framework/common.h"
#include "../../../../entities/entitypreset.h"
#include "particlepreset.h"

class Entity;
class EntityManager;

class SparkParticlePreset : public ParticlePreset
{
public:
	static ENTITYPRESET_TYPE GetType()
	{
		static ENTITYPRESET_TYPE typeName = "SparkParticlePreset";
		return typeName;
	}
	ENTITYPRESET_TYPE GetTypeOf() const                    { return GetType(); }

	SparkParticlePreset(EntityManager *entityManager);
	virtual ~SparkParticlePreset();

	Entity* Create(EntityPresetArgs *args = NULL);
};

#endif
