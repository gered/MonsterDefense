#ifndef __EFFECTS_FLASHEFFECT_H_INCLUDED__
#define __EFFECTS_FLASHEFFECT_H_INCLUDED__

#include "../framework/common.h"
#include "effect.h"
#include "../framework/graphics/color.h"

class RenderContext;

class FlashEffect : public Effect
{
public:
	static EFFECT_TYPE GetType()
	{
		static EFFECT_TYPE typeName = "FlashEffect";
		return typeName;
	}
	EFFECT_TYPE GetTypeOf() const                           { return GetType(); }

	FlashEffect();
	virtual ~FlashEffect();

	void OnRender(RenderContext *renderContext);
	void OnUpdate(float delta);

	FlashEffect* SetColor(const Color &color);
	FlashEffect* SetFlashInSpeed(float speed);
	FlashEffect* SetFlashOutSpeed(float speed);
	FlashEffect* SetMaximumIntensity(float maxIntensity);

private:
	BOOL m_flashingIn;
	float m_flashInSpeed;
	float m_flashOutSpeed;
	float m_maximumIntensity;
	float m_alpha;
	Color m_color;
};

inline FlashEffect* FlashEffect::SetColor(const Color &color)
{
	m_color = color;
	return this;
}

inline FlashEffect* FlashEffect::SetFlashInSpeed(float speed)
{
	m_flashInSpeed = speed;
	return this;
}

inline FlashEffect* FlashEffect::SetFlashOutSpeed(float speed)
{
	m_flashOutSpeed = speed;
	return this;
}

inline FlashEffect* FlashEffect::SetMaximumIntensity(float maxIntensity)
{
	m_maximumIntensity = maxIntensity;
	return this;
}

#endif

