#include "../framework/debug.h"

#include "fadeeffect.h"
#include "../contexts/rendercontext.h"
#include "../framework/graphics/color.h"
#include "../framework/graphics/graphicsdevice.h"
#include "../framework/graphics/spritebatch.h"
#include "../framework/graphics/texture.h"

const float DEFAULT_FADE_SPEED = 3.0f;
const Color DEFAULT_FADE_COLOR = COLOR_BLACK;

FadeEffect::FadeEffect()
	: Effect()
{
	STACK_TRACE;
	m_doneFading = FALSE;
	m_fadeSpeed = DEFAULT_FADE_SPEED;
	m_fadingOut = TRUE;
	m_alpha = 0.0f;
	m_color = DEFAULT_FADE_COLOR;
	m_fadeToAlpha = 0.0f;
}

FadeEffect::~FadeEffect()
{
	STACK_TRACE;
}

void FadeEffect::OnRender(RenderContext *renderContext)
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

void FadeEffect::OnUpdate(float delta)
{
	STACK_TRACE;
	if (m_doneFading)
		return;

	if (m_fadingOut)
	{
		m_alpha += (delta * m_fadeSpeed);
		if (m_alpha >= m_fadeToAlpha)
		{
			m_alpha = m_fadeToAlpha;
			m_doneFading = TRUE;
		}
	}
	else
	{
		m_alpha -= (delta * m_fadeSpeed);
		if (m_alpha < m_fadeToAlpha)
		{
			m_alpha = m_fadeToAlpha;
			m_doneFading = TRUE;
		}
	}
}

