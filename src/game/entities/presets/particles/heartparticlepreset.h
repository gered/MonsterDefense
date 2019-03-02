#ifndef __GAME_ENTITIES_PRESETS_PARTICLES_HEARTPARTICLEPRESET_H_INCLUDED__
#define __GAME_ENTITIES_PRESETS_PARTICLES_HEARTPARTICLEPRESET_H_INCLUDED__

#include "../../../../framework/common.h"
#include "../../../../entities/entitypreset.h"
#include "particlepreset.h"

class Entity;
class EntityManager;

class HeartParticlePreset : public ParticlePreset
{
public:
	static ENTITYPRESET_TYPE GetType()
	{
		static ENTITYPRESET_TYPE typeName = "HeartParticlePreset";
		return typeName;
	}
	ENTITYPRESET_TYPE GetTypeOf() const                    { return GetType(); }
	
	HeartParticlePreset(EntityManager *entityManager);
	virtual ~HeartParticlePreset();
	
	Entity* Create(EntityPresetArgs *args = NULL);
};

#endif
