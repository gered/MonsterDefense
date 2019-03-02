#ifndef __GAME_ENTITIES_PRESETS_PARTICLES_TEXTPARTICLEPRESET_H_INCLUDED__
#define __GAME_ENTITIES_PRESETS_PARTICLES_TEXTPARTICLEPRESET_H_INCLUDED__

#include "../../../../framework/common.h"
#include "../../../../entities/entitypreset.h"
#include "particlepreset.h"

class Entity;
class EntityManager;

class TextParticlePreset : public ParticlePreset
{
public:
	static ENTITYPRESET_TYPE GetType()
	{
		static ENTITYPRESET_TYPE typeName = "TextParticlePreset";
		return typeName;
	}
	ENTITYPRESET_TYPE GetTypeOf() const                    { return GetType(); }
	
	TextParticlePreset(EntityManager *entityManager);
	virtual ~TextParticlePreset();
	
	Entity* Create(EntityPresetArgs *args = NULL);
};

#endif
