#include "../framework/debug.h"

#include "flasheffect.h"
#include "../contexts/rendercontext.h"
#include "../framework/graphics/color.h"
#include "../framework/graphics/graphicsdevice.h"
#include "../framework/graphics/spritebatch.h"
#include "../framework/graphics/texture.h"

const float DEFAULT_FLASH_SPEED = 16.0f;
const float DEFAULT_MAX_INTENSITY = 1.0f;

FlashEffect::FlashEffect()
	: Effect()
{
	STACK_TRACE;
	m_flashingIn = TRUE;
	m_flashInSpeed = DEFAULT_FLASH_SPEED;
	m_flashOutSpeed = DEFAULT_FLASH_SPEED;
	m_maximumIntensity = DEFAULT_MAX_INTENSITY;
	m_alpha = 0.0f;
	m_color = COLOR_BLACK;
}

FlashEffect::~FlashEffect()
{
	STACK_TRACE;
}

void FlashEffect::OnRender(RenderContext *renderContext)
{
	STACK_TRACE;
	m_color.a = m_alpha;
	renderContext->GetSpriteBatch()->Render(
		renderContext->GetGraphicsDevice()->GetSolidColorTexture(COLOR_WHITE), 
		0, 0, 
		renderContext->GetGraphicsDevice()->GetViewportWidth(), renderContext->GetGraphicsDevice()->GetViewportHeight(), 
		m_color
		);
}

void FlashEffect::OnUpdate(float delta)
{
	STACK_TRACE;
	if (m_flashingIn)
	{
		m_alpha += (delta * m_flashInSpeed);
		if (m_alpha >= m_maximumIntensity)
		{
			m_alpha = m_maximumIntensity;
			m_flashingIn = FALSE;
		}
	}
	else
	{
		m_alpha -= (delta * m_flashOutSpeed);
		if (m_alpha < 0.0f)
			m_alpha = 0.0f;
	}

	if (m_alpha == 0.0f && m_flashingIn == FALSE)
		MarkInactive();
}

