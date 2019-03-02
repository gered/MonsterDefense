#ifndef __GAME_ENTITIES_COMPONENTS_PARTICLESONPOWERUPCOLLECTIONCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_PARTICLESONPOWERUPCOLLECTIONCOMPONENT_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../entities/component.h"
#include "../../../entities/entitypreset.h"
#include "../spawnparticleinfo.h"
#include "../../../framework/graphics/color.h"

class ParticlesOnPowerUpCollectionComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "ParticlesOnPowerUpCollectionComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }
	
	ParticlesOnPowerUpCollectionComponent();
	void Reset();

	SpawnParticleInfo particleInfo;
};

inline ParticlesOnPowerUpCollectionComponent::ParticlesOnPowerUpCollectionComponent()
{
	Reset();
}

inline void ParticlesOnPowerUpCollectionComponent::Reset()
{
	particleInfo = SpawnParticleInfo();
}

#endif

