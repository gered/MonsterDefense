#ifndef __GAME_ENTITIES_COMPONENTS_FLATVELOCITYPARTICLECOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_FLATVELOCITYPARTICLECOMPONENT_H_INCLUDED__

#include "../../../entities/component.h"
#include "../../../framework/math/vector3.h"

class FlatVelocityParticleComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "FlatVelocityParticleComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }

	FlatVelocityParticleComponent();
	void Reset();

	Vector3 velocity;
};

inline FlatVelocityParticleComponent::FlatVelocityParticleComponent()
{
	Reset();
}

inline void FlatVelocityParticleComponent::Reset()
{
	velocity = ZERO_VECTOR;
}

#endif

