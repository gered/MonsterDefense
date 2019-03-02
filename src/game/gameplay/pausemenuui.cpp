#include "../../framework/debug.h"

#include "pausemenuui.h"
#include "gameplaystate.h"
#include "../events/quitgameevent.h"
#include "../options/optionsdialogui.h"
#include "../../gameapp.h"
#include "../../contexts/contentcache.h"
#include "../../events/eventmanager.h"
#include "../../input/gamepad.h"
#include "../../input/gamepadbuttons.h"
#include "../../states/gwengamestate.h"
#include "../../states/gwengamestateuicontroller.h"
#include "../../states/statemanager.h"
#include <controls/gwen_canvas.h>
#include <controls/gwen_base.h>
#include <controls/gwen_button.h>

PauseMenuUI::PauseMenuUI(GwenGameState *gameState)
	: GwenGameStateUIController(gameState)
{
	STACK_TRACE;
}

PauseMenuUI::~PauseMenuUI()
{
	STACK_TRACE;
}

void PauseMenuUI::OnPush()
{
	STACK_TRACE;
	ContentCache *contentCache = GetGameState()->GetGameApp()->GetContentCache();
	Gwen::Controls::Canvas *canvas = InitializeGwen(contentCache->GetUISkinFilename(), contentCache->GetUIFontFilename(), contentCache->GetUIFontSize());
	
	// just add the controls so they're there. we position them in a bit...
	new Gwen::Controls::Button(canvas, "btnResume");
	new Gwen::Controls::Button(canvas, "btnRestart");
	new Gwen::Controls::Button(canvas, "btnOptions");
	new Gwen::Controls::Button(canvas, "btnQuit");
	
	PositionControls();
}

void PauseMenuUI::PositionControls()
{
	STACK_TRACE;
	Gwen::Controls::Canvas *canvas = GetCanvas();

	Gwen::Controls::Button *btnResume = canvas->FindChild<Gwen::Controls::Button>("btnResume");
	Gwen::Controls::Button *btnRestart = canvas->FindChild<Gwen::Controls::Button>("btnRestart");
	Gwen::Controls::Button *btnOptions = canvas->FindChild<Gwen::Controls::Button>("btnOptions");
	Gwen::Controls::Button *btnQuit = canvas->FindChild<Gwen::Controls::Button>("btnQuit");

	int buttonWidth = 200;
	int buttonHeight = 30;
	int buttonVerticalSpacing = 20;
	
	int left = canvas->GetSize().x / 2 - (buttonWidth / 2);
	int y = canvas->GetSize().y / 2 - ((buttonHeight * 4) + (buttonVerticalSpacing * 3)) / 2;
	
	btnResume->SetText("Resume Game");
	btnResume->SetSize(buttonWidth, buttonHeight);
	btnResume->SetPos(left, y);
	btnResume->onPress.Add(this, &PauseMenuUI::OnResumeGame);
	
	y += buttonHeight + buttonVerticalSpacing;
	
	btnRestart->SetText("Restart");
	btnRestart->SetSize(buttonWidth, buttonHeight);
	btnRestart->SetPos(left, y);
	btnRestart->onPress.Add(this, &PauseMenuUI::OnRestart);
	
	y += buttonHeight + buttonVerticalSpacing;
	
	btnOptions->SetText("Options");
	btnOptions->SetSize(buttonWidth, buttonHeight);
	btnOptions->SetPos(left, y);
	btnOptions->onPress.Add(this, &PauseMenuUI::OnOptions);
	
	y += buttonHeight + buttonVerticalSpacing;

	btnQuit->SetText("Quit");
	btnQuit->SetSize(buttonWidth, buttonHeight);
	btnQuit->SetPos(left, y);
	btnQuit->onPress.Add(this, &PauseMenuUI::OnQuit);
}

void PauseMenuUI::OnPop()
{
	STACK_TRACE;
}

void PauseMenuUI::OnResize()
{
	STACK_TRACE;
	GwenGameStateUIController::OnResize();
	PositionControls();
}

void PauseMenuUI::OnUpdate(float delta)
{
	STACK_TRACE;
	GwenGameStateUIController::OnUpdate(delta);
	
	if (GetGameState()->IsTopState())
	{
		if (GetGameState()->GetGameApp()->GetGamePad()->IsPressed(BUTTON_BACK))
			SetStateFinished();
	}
}

void PauseMenuUI::OnResumeGame(Gwen::Controls::Base *sender)
{
	STACK_TRACE;
	SetStateFinished();
}

void PauseMenuUI::OnRestart(Gwen::Controls::Base *sender)
{
	STACK_TRACE;
	if (!GetStateManager()->IsTransitioning())
		GetStateManager()->SwapTopNonOverlayWith<GamePlayState>();
}

void PauseMenuUI::OnOptions(Gwen::Controls::Base *sender)
{
	STACK_TRACE;
	GetStateManager()->Overlay<GwenGameState>()->SetGwenHandler<OptionsDialogUI>();
}

void PauseMenuUI::OnQuit(Gwen::Controls::Base *sender)
{
	STACK_TRACE;
	QuitGameEvent event;
	GetGameState()->GetEventManager()->Trigger(&event);
}
