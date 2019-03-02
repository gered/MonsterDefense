#ifndef __GAME_ENTITIES_COMPONENTS_PARTICLESONDEATHCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_PARTICLESONDEATHCOMPONENT_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../entities/component.h"
#include "../../../entities/entitypreset.h"
#include "../spawnparticleinfo.h"

class ParticlesOnDeathComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "ParticlesOnDeathComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }

	ParticlesOnDeathComponent();
	void Reset();

	SpawnParticleInfo particleInfo;
};

inline ParticlesOnDeathComponent::ParticlesOnDeathComponent()
{
	Reset();
}

inline void ParticlesOnDeathComponent::Reset()
{
	particleInfo = SpawnParticleInfo();
}

#endif

