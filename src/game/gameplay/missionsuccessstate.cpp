#include "../../framework/debug.h"

#include "missionsuccessstate.h"
#include "gameplaystate.h"
#include "../events/quitgameevent.h"
#include "../../gameapp.h"
#include "../../contexts/contentcache.h"
#include "../../contexts/rendercontext.h"
#include "../../events/eventmanager.h"
#include "../../framework/graphics/color.h"
#include "../../framework/graphics/graphicsdevice.h"
#include "../../framework/graphics/spritebatch.h"
#include "../../framework/graphics/spritefont.h"
#include "../../framework/math/mathhelpers.h"
#include "../../input/gamepad.h"
#include "../../input/gamepadbuttons.h"
#include "../../states/gwengamestate.h"
#include "../../states/gwengamestateuicontroller.h"
#include "../../states/statemanager.h"
#include <controls/gwen_canvas.h>
#include <controls/gwen_base.h>
#include <controls/gwen_button.h>
#include <controls/gwen_label.h>
#include <string.h>

const float TEXT_ANIMATE_TIME = 0.5f;
const float TEXT_START_SCALE = 1.0f;
const float TEXT_MAX_SCALE = 4.0f;

const float GWEN_ANIMATE_TIME = 0.5f;

MissionSuccessState::MissionSuccessState(GwenGameState *gameState)
	: GwenGameStateUIController(gameState)
{
	STACK_TRACE;
	m_elapsedTime = 0.0f;
	m_textAlpha = 0.0f;
	m_textScale = TEXT_START_SCALE;
	m_gwenAlpha = 0.0f;
	m_uiAreaHeight = 0;
}

MissionSuccessState::~MissionSuccessState()
{
	STACK_TRACE;
}

void MissionSuccessState::OnPush()
{
	STACK_TRACE;
	ContentCache *contentCache = GetGameState()->GetGameApp()->GetContentCache();
	Gwen::Controls::Canvas *canvas = InitializeGwen(contentCache->GetUISkinFilename(), contentCache->GetUIFontFilename(), contentCache->GetUIFontSize());
	
	new Gwen::Controls::Button(canvas, "btnRetry");
	new Gwen::Controls::Button(canvas, "btnQuit");
	new Gwen::Controls::Label(canvas, "lblScoreText");
	Gwen::Controls::Label *lblScoreAmount = new Gwen::Controls::Label(canvas, "lblScoreAmount");
	
	// place the player's score into the label
	GamePlayState *gamePlayState = GetStateManager()->GetTopNonOverlayState()->As<GamePlayState>();
	char tempScoreString[32];
	snprintf(tempScoreString, 32, "%d", gamePlayState->GetPlayerScore());
	lblScoreAmount->SetText(tempScoreString);
	
	SetupControls();
}

void MissionSuccessState::SetupControls()
{
	STACK_TRACE;
	Gwen::Controls::Canvas *canvas = GetCanvas();
	Gwen::Controls::Button *btnRetry = canvas->FindChild<Gwen::Controls::Button>("btnRetry", true);
	Gwen::Controls::Button *btnQuit = canvas->FindChild<Gwen::Controls::Button>("btnQuit", true);
	Gwen::Controls::Label *lblScoreText = canvas->FindChild<Gwen::Controls::Label>("lblScoreText", true);
	Gwen::Controls::Label *lblScoreAmount = canvas->FindChild<Gwen::Controls::Label>("lblScoreAmount", true);
	
	const int BUTTON_WIDTH = 150;
	const int BUTTON_HEIGHT = 30;
	const int BUTTON_SPACER = 50;
	const int LABEL_SPACER = 15;
	const int VERTICAL_SPACER = 30;
	
	lblScoreText->SetText("Your Score:");
	lblScoreText->SizeToContents();
	lblScoreText->SetTextColor(Gwen::Color(255, 255, 255));
	
	lblScoreAmount->SizeToContents();
	lblScoreAmount->SetTextColor(Gwen::Color(255, 255, 0));
	
	btnRetry->SetSize(BUTTON_WIDTH, BUTTON_HEIGHT);
	btnRetry->SetText("Retry");
	btnRetry->onPress.Add(this, &MissionSuccessState::OnRetry);
	
	btnQuit->SetSize(BUTTON_WIDTH, BUTTON_HEIGHT);
	btnQuit->SetText("Quit");
	btnQuit->onPress.Add(this, &MissionSuccessState::OnQuit);
	
	int left;
	int top;
	
	// position labels
	left = (canvas->Width() - lblScoreText->Width() - LABEL_SPACER - lblScoreAmount->Width()) / 2;
	top = canvas->Bottom() - (VERTICAL_SPACER * 2) - BUTTON_HEIGHT - lblScoreText->Height();
	
	lblScoreText->SetPos(left, top);
	lblScoreAmount->SetPos(left + lblScoreText->Width() + LABEL_SPACER, top);
	
	// position buttons
	
	left = (canvas->Width() - (BUTTON_WIDTH * 2) - BUTTON_SPACER) / 2;
	top = canvas->Bottom() - VERTICAL_SPACER - BUTTON_HEIGHT;
	
	btnRetry->SetPos(left, top);
	btnQuit->SetPos(left + BUTTON_WIDTH + BUTTON_SPACER, top);
	
	m_uiAreaHeight = BUTTON_HEIGHT + lblScoreText->Height() + (VERTICAL_SPACER * 3);
}

void MissionSuccessState::OnPop()
{
	STACK_TRACE;
}

void MissionSuccessState::OnRender(RenderContext *renderContext)
{
	STACK_TRACE;
	const char *message = "VICTORY!";

	SetAlpha(m_gwenAlpha);
	GwenGameStateUIController::OnRender(renderContext);
	
	Color textColor(1.0f, 1.0f, 1.0f, m_textAlpha);
	SpriteFont *font = GetGameState()->GetGameApp()->GetContentCache()->GetFont();
	
	float renderTextScale = m_textScale + (float)renderContext->GetScreenScale();
	
	uint16_t width = 0;
	uint16_t height = 0;
	font->MeasureString(&width, &height, renderTextScale, message);
	
	// scale to screen space (that is, real pixel sizes)
	uint16_t bottomUIAreaHeight = m_uiAreaHeight * renderContext->GetScreenScale();
	
	uint16_t remainingHeight = renderContext->GetGraphicsDevice()->GetViewportHeight() - bottomUIAreaHeight;
	
	uint16_t x = (renderContext->GetGraphicsDevice()->GetViewportWidth() - width) / 2;
	uint16_t y = (remainingHeight - height) / 2;
	uint32_t shadowOffset = (uint16_t)(1.3f * renderTextScale);
	
	renderContext->GetSpriteBatch()->Render(font, x + shadowOffset, y + shadowOffset, COLOR_BLACK, renderTextScale, message);
	renderContext->GetSpriteBatch()->Render(font, x, y, textColor, renderTextScale, message);
}

void MissionSuccessState::OnResize()
{
	STACK_TRACE;
	GwenGameStateUIController::OnResize();
	SetupControls();
}

void MissionSuccessState::OnUpdate(float delta)
{
	STACK_TRACE;
	GwenGameStateUIController::OnUpdate(delta);
	
	if (GetGameState()->IsTransitioning())
		m_elapsedTime += delta;}

BOOL MissionSuccessState::OnTransition(float delta, BOOL isTransitioningOut, BOOL started)
{
	STACK_TRACE;
	if (isTransitioningOut)
		return TRUE;
	
	if (started)
		EnableGwenInput(FALSE);
	
	if (m_elapsedTime < TEXT_ANIMATE_TIME)
	{
		// fade & scale text in first
		float t = m_elapsedTime / TEXT_ANIMATE_TIME;
		m_textAlpha = SmoothStep(0.0f, 1.0f, t);
		m_textScale = SmoothStep(TEXT_START_SCALE, TEXT_MAX_SCALE, t);
	}
	
	else if ((m_elapsedTime - TEXT_ANIMATE_TIME) < GWEN_ANIMATE_TIME)
	{
		// fade in gwen's UI stuff next
		float t = (m_elapsedTime - TEXT_ANIMATE_TIME) / GWEN_ANIMATE_TIME;
		m_gwenAlpha = SmoothStep(0.0f, 1.0f, t);
	}
	else
	{
		// finished both fade-ins
		EnableGwenInput(TRUE);
		return TRUE;
	}
	
	return FALSE;
}

void MissionSuccessState::OnRetry(Gwen::Controls::Base *sender)
{
	STACK_TRACE;
	if (!GetStateManager()->IsTransitioning())
		GetStateManager()->SwapTopNonOverlayWith<GamePlayState>();
}

void MissionSuccessState::OnQuit(Gwen::Controls::Base *sender)
{
	STACK_TRACE;
	QuitGameEvent event;
	GetGameState()->GetEventManager()->Trigger(&event);
}
