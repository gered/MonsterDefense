#include "../../framework/debug.h"
#include "../../framework/common.h"

#include "introcountdownstate.h"
#include "pausemenuui.h"
#include "startmessageprocess.h"
#include "../events/introfinishedevent.h"
#include "../../gameapp.h"
#include "../../contexts/contentcache.h"
#include "../../contexts/rendercontext.h"
#include "../../events/eventmanager.h"
#include "../../framework/graphics/color.h"
#include "../../framework/graphics/graphicsdevice.h"
#include "../../framework/graphics/spritebatch.h"
#include "../../framework/graphics/spritefont.h"
#include "../../framework/math/mathhelpers.h"
#include "../../states/gwengamestate.h"
#include "../../states/statemanager.h"
#include "../../processes/processmanager.h"
#include <string.h>

const float TEXT_SCALE_START = 4.0f;
const float TEXT_SCALE_END = 0.0f;

const float TEXT_ALPHA_START = 1.0f;
const float TEXT_ALPHA_END = 0.0f;

const float INITIAL_WAIT_TIME = 0.5f;
const float STEP_TIME = 1.0f;

IntroCountdownState::IntroCountdownState(GameApp *gameApp, StateManager *stateManager)
	: GameState(gameApp, stateManager)
{
	STACK_TRACE;
	m_initialWaitTime = 0.0f;
	m_time = 0.0f;
	m_alpha = 0.0f;
	m_scale = 0.0f;
	m_number = 0;
}

IntroCountdownState::~IntroCountdownState()
{
	STACK_TRACE;
}

void IntroCountdownState::OnPush()
{
	STACK_TRACE;
	m_time = 0.0f;
	m_initialWaitTime = INITIAL_WAIT_TIME;
	m_alpha = TEXT_ALPHA_START;
	m_scale = TEXT_SCALE_START;
	m_number = 3;
	
	snprintf(m_message, 5, "%d..", m_number);
	
	if (!GetGameApp()->IsAppFocused())
		GetStateManager()->Overlay<GwenGameState>()->SetGwenHandler<PauseMenuUI>();
}

void IntroCountdownState::OnPop()
{
	STACK_TRACE;
}

void IntroCountdownState::OnAppLostFocus()
{
	STACK_TRACE;
	GameState::OnAppLostFocus();
	if (IsTopState())
		GetStateManager()->Overlay<GwenGameState>()->SetGwenHandler<PauseMenuUI>();
}

void IntroCountdownState::OnRender(RenderContext *renderContext)
{
	STACK_TRACE;
	GameState::OnRender(renderContext);
	if (m_initialWaitTime > 0.0f)
		return;
	
	SpriteFont *font = GetGameApp()->GetContentCache()->GetFont();
	
	float renderTextScale = m_scale * (float)renderContext->GetScreenScale();
	
	Color color(1.0f, 1.0f, 1.0f, m_alpha);
	
	uint16_t width = 0;
	uint16_t height = 0;
	font->MeasureString(&width, &height, renderTextScale, m_message);
	
	uint16_t x = (renderContext->GetGraphicsDevice()->GetViewportWidth() - width) / 2;
	uint16_t y = (renderContext->GetGraphicsDevice()->GetViewportHeight() - height) / 2;
	
	y -= 50 * renderContext->GetScreenScale();
	
	renderContext->GetSpriteBatch()->Render(font, x, y, color, renderTextScale, m_message);
}

void IntroCountdownState::OnUpdate(float delta)
{
	STACK_TRACE;
	GameState::OnUpdate(delta);
	
	if (!IsTopState())
		return;
	
	if (m_initialWaitTime > 0.0f)
	{
		m_initialWaitTime -= delta;
		return;
	}

	m_time += delta;
	
	if (m_time >= STEP_TIME)
	{
		// go to the next step # ..
		--m_number;
		m_time = 0.0f;
		m_scale = TEXT_SCALE_END;

		if (m_number > 0)
			snprintf(m_message, 5, "%d..", m_number);
		else
		{
			GameState *gameState = GetStateManager()->GetTopNonOverlayState();
			gameState->GetProcessManager()->Add<StartMessageProcess>();
			
			// TODO: push "go" message process and queue event that starts
			//       the round (create player, start spawning mobs, etc)
			IntroFinishedEvent *e = new IntroFinishedEvent();
			GetEventManager()->Queue(e);
			
			SetFinished();
		}
	}
	else
	{
		float t = m_time / STEP_TIME;
		
		m_alpha = SmoothStep(TEXT_ALPHA_START, TEXT_SCALE_END, t);
		m_scale = SmoothStep(TEXT_SCALE_START, TEXT_SCALE_END, t);
	}
}
