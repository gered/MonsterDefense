#include "../../framework/debug.h"

#include "playmenuui.h"
#include "difficultymenuui.h"
#include "mainmenuui.h"
#include "menuuiconstants.h"
#include "../gamemodesettings.h"
#include "../../gameapp.h"
#include "../../contexts/contentcache.h"
#include "../../events/eventmanager.h"
#include "../../states/gwengamestate.h"
#include "../../states/gwengamestateuicontroller.h"
#include "../../states/statemanager.h"
#include <controls/gwen_canvas.h>
#include <controls/gwen_base.h>
#include <controls/gwen_button.h>
#include <controls/gwen_label.h>

PlayMenuUI::PlayMenuUI(GwenGameState *gameState)
	: BaseMainMenuUI(gameState)
{
	STACK_TRACE;
}

PlayMenuUI::~PlayMenuUI()
{
	STACK_TRACE;
}

void PlayMenuUI::AddControls()
{
	STACK_TRACE;
	Gwen::Controls::Canvas *canvas = GetCanvas();
	
	Gwen::Controls::Label *lblTitle = new Gwen::Controls::Label(canvas, "lblTitle");
	lblTitle->SetText("Game Type");
	lblTitle->SetAlignment(Gwen::Pos::CenterH);
	lblTitle->SetTextColor(UI_COLOR_TITLE);
	
	Gwen::Controls::Button *btnTimed = new Gwen::Controls::Button(canvas, "btnTimed");
	btnTimed->SetText("Timed");
	btnTimed->onPress.Add(this, &PlayMenuUI::OnTimed);
	
	Gwen::Controls::Button *btnEndless = new Gwen::Controls::Button(canvas, "btnEndless");
	btnEndless->SetText("Endless");
	btnEndless->onPress.Add(this, &PlayMenuUI::OnEndless);
	
	Gwen::Controls::Button *btnBack = new Gwen::Controls::Button(canvas, "btnBack");
	btnBack->SetText("Back");
	btnBack->onPress.Add(this, &PlayMenuUI::OnBack);
}

void PlayMenuUI::PositionControls()
{
	STACK_TRACE;
	Gwen::Controls::Canvas *canvas = GetCanvas();
	
	Gwen::Controls::Label *lblTitle = canvas->FindChild<Gwen::Controls::Label>("lblTitle");
	Gwen::Controls::Button *btnTimed = canvas->FindChild<Gwen::Controls::Button>("btnTimed");
	Gwen::Controls::Button *btnEndless = canvas->FindChild<Gwen::Controls::Button>("btnEndless");
	Gwen::Controls::Button *btnBack = canvas->FindChild<Gwen::Controls::Button>("btnBack");
	
	lblTitle->SizeToContents();   // this is really just to get the right height automatically...
	lblTitle->SetWidth(TITLE_LABEL_WIDTH);
	
	int left = canvas->Right() - MENU_SIDE_WIDTH + ((MENU_SIDE_WIDTH - BUTTON_WIDTH) / 2);
	int top = TITLE_LABEL_SPACER;
	
	lblTitle->SetPos(left, top);
	
	top += lblTitle->Height() + TITLE_LABEL_SPACER;
	
	btnTimed->SetSize(BUTTON_WIDTH, BUTTON_HEIGHT);
	btnTimed->SetPos(left, top);
	
	top += BUTTON_HEIGHT + BUTTON_SPACING;
	
	btnEndless->SetSize(BUTTON_WIDTH, BUTTON_HEIGHT);
	btnEndless->SetPos(left, top);
	
	top = canvas->Bottom() - BUTTON_SPACING - BUTTON_HEIGHT;
	
	btnBack->SetSize(BUTTON_WIDTH, BUTTON_HEIGHT);
	btnBack->SetPos(left, top);
}

void PlayMenuUI::OnTimed(Gwen::Controls::Base *sender)
{
	STACK_TRACE;
	GameModeSettings *settings = GetGameState()->GetGameApp()->GetModeSettings();
	settings->type = GAME_TYPE_TIMED;
	GetStateManager()->SwapTopWith<GwenGameState>()->SetGwenHandler<DifficultyMenuUI>();
}

void PlayMenuUI::OnEndless(Gwen::Controls::Base *sender)
{
	STACK_TRACE;
	GameModeSettings *settings = GetGameState()->GetGameApp()->GetModeSettings();
	settings->type = GAME_TYPE_ENDLESS;
	GetStateManager()->SwapTopWith<GwenGameState>()->SetGwenHandler<DifficultyMenuUI>();
}

void PlayMenuUI::OnBack(Gwen::Controls::Base *sender)
{
	STACK_TRACE;
	GetStateManager()->SwapTopWith<GwenGameState>()->SetGwenHandler<MainMenuUI>();
}
