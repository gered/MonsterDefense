#ifndef __GAME_ENTITIES_COMPONENTS_PARTICLECOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_PARTICLECOMPONENT_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../entities/component.h"

class ParticleComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "ParticleComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }

	ParticleComponent();
	void Reset();

	BOOL lastsUntilAnimationFinishes;
	float lifeTimeLeft;
};

inline ParticleComponent::ParticleComponent()
{
	Reset();
}

inline void ParticleComponent::Reset()
{
	lifeTimeLeft = 0.0f;
	lastsUntilAnimationFinishes = FALSE;
}

#endif

