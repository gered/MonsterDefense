#ifndef __GAME_ENTITIES_PRESETS_PARTICLES_SMOKEPARTICLEPRESET_H_INCLUDED__
#define __GAME_ENTITIES_PRESETS_PARTICLES_SMOKEPARTICLEPRESET_H_INCLUDED__

#include "../../../../framework/common.h"
#include "../../../../entities/entitypreset.h"
#include "particlepreset.h"

class Entity;
class EntityManager;

class SmokeParticlePreset : public ParticlePreset
{
public:
	static ENTITYPRESET_TYPE GetType()
	{
		static ENTITYPRESET_TYPE typeName = "SmokeParticlePreset";
		return typeName;
	}
	ENTITYPRESET_TYPE GetTypeOf() const                    { return GetType(); }

	SmokeParticlePreset(EntityManager *entityManager);
	virtual ~SmokeParticlePreset();

	Entity* Create(EntityPresetArgs *args = NULL);
};

#endif
