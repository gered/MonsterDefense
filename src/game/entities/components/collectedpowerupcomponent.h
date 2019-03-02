#ifndef __GAME_ENTITIES_COMPONENTS_COLLECTEDPOWERUPCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_COLLECTEDPOWERUPCOMPONENT_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../entities/component.h"
#include "../powerups/powerup.h"

class CollectedPowerUpComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "CollectedPowerUpComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }

	CollectedPowerUpComponent();
	void Reset();

	PowerUp *powerUp;
	float riseTime;
	float postRiseFloatTime;
	float minFloatY;
	float maxFloatY;
	float alpha;
};

inline CollectedPowerUpComponent::CollectedPowerUpComponent()
{
	Reset();
}

inline void CollectedPowerUpComponent::Reset()
{
	powerUp = NULL;
	riseTime = 0.0f;
	postRiseFloatTime = 0.0f;
	minFloatY = 0.0f;
	maxFloatY = 0.0f;
	alpha = 0.0f;
}

#endif

