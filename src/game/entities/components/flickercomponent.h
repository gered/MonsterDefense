#ifndef __GAME_ENTITIES_COMPONENTS_FLICKERCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_FLICKERCOMPONENT_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../entities/component.h"
#include "../../../framework/graphics/color.h"

const float DEFAULT_FLICKER_SWAP_TIME = 0.01f;

class FlickerComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "FlickerComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }

	FlickerComponent();
	void Reset();
	
	void Set(const Color &flickerColor, const Color &originalEntityColor, float timeToFlickerFor);
	void Set(const Color &flickerColor, const Color &originalEntityColor);

	Color color;
	Color originalColor;
	float flickerTime;
	float onOffSwapTime;
	float swapTime;
	BOOL flickerColorCurrentlyOn;
	BOOL isFlickerTimeInfinite;
};

inline FlickerComponent::FlickerComponent()
{
	Reset();
}

inline void FlickerComponent::Reset()
{
	color = COLOR_WHITE;
	originalColor = COLOR_WHITE;
	flickerTime = 0.0f;
	onOffSwapTime = 0.0f;
	swapTime = 0.0f;
	flickerColorCurrentlyOn = FALSE;
	isFlickerTimeInfinite = FALSE;
}

inline void FlickerComponent::Set(const Color &flickerColor, const Color &originalEntityColor, float timeToFlickerFor)
{
	this->color = flickerColor;
	this->originalColor = originalEntityColor;
	this->flickerTime = timeToFlickerFor;
	this->swapTime = DEFAULT_FLICKER_SWAP_TIME;
	this->onOffSwapTime = this->swapTime;
	this->flickerColorCurrentlyOn = TRUE;
	this->isFlickerTimeInfinite = FALSE;
}

inline void FlickerComponent::Set(const Color &flickerColor, const Color &originalEntityColor)
{
	this->color = flickerColor;
	this->originalColor = originalEntityColor;
	this->flickerTime = 0.0f;
	this->swapTime = DEFAULT_FLICKER_SWAP_TIME;
	this->onOffSwapTime = this->swapTime;
	this->flickerColorCurrentlyOn = TRUE;
	this->isFlickerTimeInfinite = TRUE;
}

#endif

