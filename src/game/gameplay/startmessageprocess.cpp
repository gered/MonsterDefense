#include "../../framework/debug.h"
#include "../../framework/common.h"

#include "startmessageprocess.h"
#include "../../gameapp.h"
#include "../../contexts/contentcache.h"
#include "../../contexts/rendercontext.h"
#include "../../framework/graphics/color.h"
#include "../../framework/graphics/graphicsdevice.h"
#include "../../framework/graphics/spritebatch.h"
#include "../../framework/graphics/spritefont.h"
#include "../../framework/math/mathhelpers.h"
#include "../../states/gamestate.h"

const float TEXT_SCALE_START = 4.0f;
const float TEXT_SCALE_END = 0.0f;

const float TEXT_ALPHA_START = 1.0f;
const float TEXT_ALPHA_END = 0.0f;

const float TIME_TO_SHOW = 1.0f;

StartMessageProcess::StartMessageProcess(GameState *gameState, ProcessManager *processManager)
	: GameProcess(gameState, processManager)
{
	STACK_TRACE;
	m_time = 0.0f;
	m_alpha = 0.0f;
	m_scale = 0.0f;
}

StartMessageProcess::~StartMessageProcess()
{
	STACK_TRACE;
}

void StartMessageProcess::OnRender(RenderContext *renderContext)
{
	STACK_TRACE;
	const char* message = "GO!";
	GameProcess::OnRender(renderContext);
	SpriteFont *font = GetGameApp()->GetContentCache()->GetFont();
	
	float renderTextScale = m_scale * (float)renderContext->GetScreenScale();
	
	Color color(1.0f, 1.0f, 1.0f, m_alpha);
	
	uint16_t width = 0;
	uint16_t height = 0;
	font->MeasureString(&width, &height, renderTextScale, message);
	
	uint16_t x = (renderContext->GetGraphicsDevice()->GetViewportWidth() - width) / 2;
	uint16_t y = (renderContext->GetGraphicsDevice()->GetViewportHeight() - height) / 2;
	
	y -= 50 * renderContext->GetScreenScale();

	renderContext->GetSpriteBatch()->Render(font, x, y, color, renderTextScale, message);
}

void StartMessageProcess::OnUpdate(float delta)
{
	STACK_TRACE;
	GameProcess::OnUpdate(delta);
	
	if (!GetGameState()->IsTopState())
		return;
	
	m_time += delta;
	
	if (m_time >= TIME_TO_SHOW)
		SetFinished();
	else
	{
		float t = m_time / TIME_TO_SHOW;
		
		m_alpha = SmoothStep(TEXT_ALPHA_START, TEXT_SCALE_END, t);
		m_scale = SmoothStep(TEXT_SCALE_START, TEXT_SCALE_END, t);
	}
}
