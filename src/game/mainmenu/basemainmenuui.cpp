#include "../../framework/debug.h"

#include "basemainmenuui.h"
#include "menuuiconstants.h"
#include "../../gameapp.h"
#include "../../contexts/contentcache.h"
#include "../../framework/math/mathhelpers.h"
#include "../../states/gwengamestate.h"
#include "../../states/gwengamestateuicontroller.h"
#include <controls/gwen_canvas.h>

BaseMainMenuUI::BaseMainMenuUI(GwenGameState *gameState)
	: GwenGameStateUIController(gameState)
{
	STACK_TRACE;
	m_fadeProgress = 0.0f;
	m_fadeAlpha = 0.0f;
	m_fadeToAlpha = 0.0f;
	m_fadeFromAlpha = 0.0f;
}

BaseMainMenuUI::~BaseMainMenuUI()
{
	STACK_TRACE;
}

void BaseMainMenuUI::OnPush()
{
	STACK_TRACE;
	ContentCache *contentCache = GetGameState()->GetGameApp()->GetContentCache();
	InitializeGwen(contentCache->GetUISkinFilename(), contentCache->GetUIFontFilename(), contentCache->GetUIFontSize());
	
	AddControls();
	PositionControls();
}

void BaseMainMenuUI::OnPop()
{
	STACK_TRACE;
}

void BaseMainMenuUI::OnResize()
{
	STACK_TRACE;
	GwenGameStateUIController::OnResize();
	PositionControls();
}

BOOL BaseMainMenuUI::OnTransition(float delta, BOOL isTransitioningOut, BOOL started)
{
	STACK_TRACE;
	if (started)
	{
		// prevent clicking of buttons, etc while fading
		EnableGwenInput(FALSE);
		
		if (isTransitioningOut)
		{
			m_fadeAlpha = 1.0f;
			m_fadeFromAlpha = 1.0f;
			m_fadeToAlpha = 0.0f;
		}
		else
		{
			m_fadeAlpha = 0.0f;
			m_fadeFromAlpha = 0.0f;
			m_fadeToAlpha = 1.0f;
		}
	}
	else
	{
		m_fadeProgress += delta * 2.5f;
		m_fadeAlpha = SmoothStep(m_fadeFromAlpha, m_fadeToAlpha, m_fadeProgress);
	}
	
	SetAlpha(m_fadeAlpha);
	
	if (IsCloseEnough(m_fadeAlpha, m_fadeToAlpha))
	{
		// if we just finished transitioning in, we should re-enable UI input
		if (!isTransitioningOut)
			EnableGwenInput(TRUE);
		
		return TRUE;
	}
	else
		return FALSE;
}
