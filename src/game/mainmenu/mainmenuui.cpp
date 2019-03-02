#include "../../framework/debug.h"

#include "mainmenuui.h"
#include "playmenuui.h"
#include "menuuiconstants.h"
#include "../events/quitappevent.h"
#include "../options/optionsdialogui.h"
#include "../../gameapp.h"
#include "../../contexts/contentcache.h"
#include "../../events/eventmanager.h"
#include "../../states/gwengamestate.h"
#include "../../states/gwengamestateuicontroller.h"
#include "../../states/statemanager.h"
#include <controls/gwen_canvas.h>
#include <controls/gwen_base.h>
#include <controls/gwen_button.h>

MainMenuUI::MainMenuUI(GwenGameState *gameState)
	: BaseMainMenuUI(gameState)
{
	STACK_TRACE;
}

MainMenuUI::~MainMenuUI()
{
	STACK_TRACE;
}

void MainMenuUI::AddControls()
{
	STACK_TRACE;
	Gwen::Controls::Canvas *canvas = GetCanvas();

	Gwen::Controls::Button *btnPlay = new Gwen::Controls::Button(canvas, "btnPlay");
	btnPlay->SetText("Play");
	btnPlay->onPress.Add(this, &MainMenuUI::OnPlay);
	
	Gwen::Controls::Button *btnOptions = new Gwen::Controls::Button(canvas, "btnOptions");
	btnOptions->SetText("Options");
	btnOptions->onPress.Add(this, &MainMenuUI::OnOptions);
	
	Gwen::Controls::Button *btnQuit = new Gwen::Controls::Button(canvas, "btnQuit");
	btnQuit->SetText("Quit");
	btnQuit->onPress.Add(this, &MainMenuUI::OnQuit);
}

void MainMenuUI::PositionControls()
{
	STACK_TRACE;
	Gwen::Controls::Canvas *canvas = GetCanvas();
	
	Gwen::Controls::Button *btnPlay = canvas->FindChild<Gwen::Controls::Button>("btnPlay");
	Gwen::Controls::Button *btnOptions = canvas->FindChild<Gwen::Controls::Button>("btnOptions");
	Gwen::Controls::Button *btnQuit = canvas->FindChild<Gwen::Controls::Button>("btnQuit");

	int left = canvas->Right() - MENU_SIDE_WIDTH + ((MENU_SIDE_WIDTH - BUTTON_WIDTH) / 2);
	int top = BUTTON_SPACING;
	
	btnPlay->SetSize(BUTTON_WIDTH, BUTTON_HEIGHT);
	btnPlay->SetPos(left, top);
	
	top += BUTTON_HEIGHT + BUTTON_SPACING;
	
	btnOptions->SetSize(BUTTON_WIDTH, BUTTON_HEIGHT);
	btnOptions->SetPos(left, top);
	
	top = canvas->Bottom() - BUTTON_SPACING - BUTTON_HEIGHT;
	
	btnQuit->SetSize(BUTTON_WIDTH, BUTTON_HEIGHT);
	btnQuit->SetPos(left, top);
}

void MainMenuUI::OnPlay(Gwen::Controls::Base *sender)
{
	STACK_TRACE;
	GetStateManager()->SwapTopWith<GwenGameState>()->SetGwenHandler<PlayMenuUI>();
}

void MainMenuUI::OnOptions(Gwen::Controls::Base *sender)
{
	STACK_TRACE;
	GetStateManager()->Overlay<GwenGameState>()->SetGwenHandler<OptionsDialogUI>();
}

void MainMenuUI::OnQuit(Gwen::Controls::Base *sender)
{
	STACK_TRACE;
	QuitAppEvent event;
	GetGameState()->GetEventManager()->Trigger(&event);
}
