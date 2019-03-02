#ifndef __GAME_ENTITIES_COMPONENTS_PARTICLESONSPAWNCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_PARTICLESONSPAWNCOMPONENT_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../entities/component.h"
#include "../../../entities/entitypreset.h"
#include "../spawnparticleinfo.h"

class ParticlesOnSpawnComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "ParticlesOnSpawnComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }
	
	ParticlesOnSpawnComponent();
	void Reset();
	
	SpawnParticleInfo particleInfo;
};

inline ParticlesOnSpawnComponent::ParticlesOnSpawnComponent()
{
	Reset();
}

inline void ParticlesOnSpawnComponent::Reset()
{
	particleInfo = SpawnParticleInfo();
}

#endif

