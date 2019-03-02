#ifndef __GAME_ENTITIES_PRESETS_PARTICLES_PARTICLEPRESET_H_INCLUDED__
#define __GAME_ENTITIES_PRESETS_PARTICLES_PARTICLEPRESET_H_INCLUDED__

#include "../../../../framework/common.h"
#include "../../../../entities/entitypreset.h"

class Entity;
class EntityManager;
struct ParticlePresetProperties;

class ParticlePreset : public EntityPreset
{
public:
	virtual ~ParticlePreset();
	
	Entity* Create(EntityPresetArgs *args = NULL);
	
protected:
	ParticlePreset(EntityManager *entityManager);

	void SetPositionFromArgs(Entity *entity, const ParticlePresetProperties *args);
};

#endif
