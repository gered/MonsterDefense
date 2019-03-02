#ifndef __GAME_ENTITIES_PRESETS_PARTICLES_STARPARTICLEPRESET_H_INCLUDED__
#define __GAME_ENTITIES_PRESETS_PARTICLES_STARPARTICLEPRESET_H_INCLUDED__

#include "../../../../framework/common.h"
#include "../../../../entities/entitypreset.h"
#include "particlepreset.h"

class Entity;
class EntityManager;

class StarParticlePreset : public ParticlePreset
{
public:
	static ENTITYPRESET_TYPE GetType()
	{
		static ENTITYPRESET_TYPE typeName = "StarParticlePreset";
		return typeName;
	}
	ENTITYPRESET_TYPE GetTypeOf() const                    { return GetType(); }

	StarParticlePreset(EntityManager *entityManager);
	virtual ~StarParticlePreset();

	Entity* Create(EntityPresetArgs *args = NULL);
};

#endif
