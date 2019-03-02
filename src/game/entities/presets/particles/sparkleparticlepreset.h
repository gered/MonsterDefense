#ifndef __GAME_ENTITIES_PRESETS_PARTICLES_SPARKLEPARTICLEPRESET_H_INCLUDED__
#define __GAME_ENTITIES_PRESETS_PARTICLES_SPARKLEPARTICLEPRESET_H_INCLUDED__

#include "../../../../framework/common.h"
#include "../../../../entities/entitypreset.h"
#include "particlepreset.h"

class Entity;
class EntityManager;

class SparkleParticlePreset : public ParticlePreset
{
public:
	static ENTITYPRESET_TYPE GetType()
	{
		static ENTITYPRESET_TYPE typeName = "SparkleParticlePreset";
		return typeName;
	}
	ENTITYPRESET_TYPE GetTypeOf() const                    { return GetType(); }

	SparkleParticlePreset(EntityManager *entityManager);
	virtual ~SparkleParticlePreset();
	
	Entity* Create(EntityPresetArgs *args = NULL);
};

#endif
