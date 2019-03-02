#include "../framework/debug.h"

#include "dimeffect.h"
#include "../contexts/rendercontext.h"
#include "../framework/graphics/color.h"
#include "../framework/graphics/graphicsdevice.h"
#include "../framework/graphics/spritebatch.h"

const float DEFAULT_DIM_ALPHA = 0.5f;
const Color DEFAULT_DIM_COLOR = COLOR_BLACK;

DimEffect::DimEffect()
	: Effect()
{
	STACK_TRACE;
	m_alpha = DEFAULT_DIM_ALPHA;
	m_color = DEFAULT_DIM_COLOR;
}

DimEffect::~DimEffect()
{
	STACK_TRACE;
}

void DimEffect::OnRender(RenderContext *renderContext)
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

