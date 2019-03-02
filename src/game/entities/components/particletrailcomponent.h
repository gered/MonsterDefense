#ifndef __GAME_ENTITIES_COMPONENTS_PARTICLETRAILCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_PARTICLETRAILCOMPONENT_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../entities/component.h"
#include "../../../entities/entitypreset.h"
#include "../spawnparticleinfo.h"

class ParticleTrailComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "ParticleTrailComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }
	
	ParticleTrailComponent();
	void Reset();
	
	SpawnParticleInfo particleInfo;
	float minTime;
	float maxTime;
	float timeSinceLast;
};

inline ParticleTrailComponent::ParticleTrailComponent()
{
	Reset();
}

inline void ParticleTrailComponent::Reset()
{
	particleInfo = SpawnParticleInfo();
	minTime = 0.0f;
	maxTime = 0.0f;
	timeSinceLast = 0.0f;
}

#endif

