#include "../../framework/debug.h"

#include "difficultymenuui.h"
#include "confirmplaymenuui.h"
#include "playmenuui.h"
#include "timemenuui.h"
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

DifficultyMenuUI::DifficultyMenuUI(GwenGameState *gameState)
	: BaseMainMenuUI(gameState)
{
	STACK_TRACE;
}

DifficultyMenuUI::~DifficultyMenuUI()
{
	STACK_TRACE;
}

void DifficultyMenuUI::AddControls()
{
	STACK_TRACE;
	Gwen::Controls::Canvas *canvas = GetCanvas();
	
	Gwen::Controls::Label *lblTitle = new Gwen::Controls::Label(canvas, "lblTitle");
	lblTitle->SetText("Difficulty Level");
	lblTitle->SetAlignment(Gwen::Pos::CenterH);
	lblTitle->SetTextColor(UI_COLOR_TITLE);
	
	Gwen::Controls::Button *btnEasy = new Gwen::Controls::Button(canvas, "btnEasy");
	btnEasy->SetText("Easy");
	btnEasy->onPress.Add(this, &DifficultyMenuUI::OnEasy);
	
	Gwen::Controls::Button *btnNormal = new Gwen::Controls::Button(canvas, "btnNormal");
	btnNormal->SetText("Normal");
	btnNormal->onPress.Add(this, &DifficultyMenuUI::OnNormal);

	Gwen::Controls::Button *btnHard = new Gwen::Controls::Button(canvas, "btnHard");
	btnHard->SetText("Hard");
	btnHard->onPress.Add(this, &DifficultyMenuUI::OnHard);
	
	Gwen::Controls::Button *btnBack = new Gwen::Controls::Button(canvas, "btnBack");
	btnBack->SetText("Back");
	btnBack->onPress.Add(this, &DifficultyMenuUI::OnBack);
}

void DifficultyMenuUI::PositionControls()
{
	STACK_TRACE;
	Gwen::Controls::Canvas *canvas = GetCanvas();
	
	Gwen::Controls::Label *lblTitle = canvas->FindChild<Gwen::Controls::Label>("lblTitle");
	Gwen::Controls::Button *btnEasy = canvas->FindChild<Gwen::Controls::Button>("btnEasy");
	Gwen::Controls::Button *btnNormal = canvas->FindChild<Gwen::Controls::Button>("btnNormal");
	Gwen::Controls::Button *btnHard = canvas->FindChild<Gwen::Controls::Button>("btnHard");
	Gwen::Controls::Button *btnBack = canvas->FindChild<Gwen::Controls::Button>("btnBack");
	
	lblTitle->SizeToContents();   // this is really just to get the right height automatically...
	lblTitle->SetWidth(TITLE_LABEL_WIDTH);
	
	int left = canvas->Right() - MENU_SIDE_WIDTH + ((MENU_SIDE_WIDTH - BUTTON_WIDTH) / 2);
	int top = TITLE_LABEL_SPACER;
	
	lblTitle->SetPos(left, top);
	
	top += lblTitle->Height() + TITLE_LABEL_SPACER;
	
	btnEasy->SetSize(BUTTON_WIDTH, BUTTON_HEIGHT);
	btnEasy->SetPos(left, top);
	
	top += BUTTON_HEIGHT + BUTTON_SPACING;
	
	btnNormal->SetSize(BUTTON_WIDTH, BUTTON_HEIGHT);
	btnNormal->SetPos(left, top);
	
	top += BUTTON_HEIGHT + BUTTON_SPACING;
	
	btnHard->SetSize(BUTTON_WIDTH, BUTTON_HEIGHT);
	btnHard->SetPos(left, top);
	
	top = canvas->Bottom() - BUTTON_SPACING - BUTTON_HEIGHT;
	
	btnBack->SetSize(BUTTON_WIDTH, BUTTON_HEIGHT);
	btnBack->SetPos(left, top);
}

void DifficultyMenuUI::OnEasy(Gwen::Controls::Base *sender)
{
	STACK_TRACE;
	GameModeSettings *settings = GetGameState()->GetGameApp()->GetModeSettings();
	settings->difficulty = GAME_DIFFICULTY_EASY;
	if (settings->type == GAME_TYPE_TIMED)
		GetStateManager()->SwapTopWith<GwenGameState>()->SetGwenHandler<TimeMenuUI>();
	else
		GetStateManager()->SwapTopWith<GwenGameState>()->SetGwenHandler<ConfirmPlayMenuUI>();		
}

void DifficultyMenuUI::OnNormal(Gwen::Controls::Base *sender)
{
	STACK_TRACE;
	GameModeSettings *settings = GetGameState()->GetGameApp()->GetModeSettings();
	settings->difficulty = GAME_DIFFICULTY_NORMAL;
	if (settings->type == GAME_TYPE_TIMED)
		GetStateManager()->SwapTopWith<GwenGameState>()->SetGwenHandler<TimeMenuUI>();
	else
		GetStateManager()->SwapTopWith<GwenGameState>()->SetGwenHandler<ConfirmPlayMenuUI>();		
}

void DifficultyMenuUI::OnHard(Gwen::Controls::Base *sender)
{
	STACK_TRACE;
	GameModeSettings *settings = GetGameState()->GetGameApp()->GetModeSettings();
	settings->difficulty = GAME_DIFFICULTY_HARD;
	if (settings->type == GAME_TYPE_TIMED)
		GetStateManager()->SwapTopWith<GwenGameState>()->SetGwenHandler<TimeMenuUI>();
	else
		GetStateManager()->SwapTopWith<GwenGameState>()->SetGwenHandler<ConfirmPlayMenuUI>();		
}

void DifficultyMenuUI::OnBack(Gwen::Controls::Base *sender)
{
	STACK_TRACE;
	GetStateManager()->SwapTopWith<GwenGameState>()->SetGwenHandler<PlayMenuUI>();
}
