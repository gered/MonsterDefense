#include "../../framework/debug.h"

#include "confirmplaymenuui.h"
#include "difficultymenuui.h"
#include "timemenuui.h"
#include "menuuiconstants.h"
#include "../gamemodesettings.h"
#include "../gameplay/gameplaystate.h"
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
#include <string.h>

ConfirmPlayMenuUI::ConfirmPlayMenuUI(GwenGameState *gameState)
	: BaseMainMenuUI(gameState)
{
	STACK_TRACE;
}

ConfirmPlayMenuUI::~ConfirmPlayMenuUI()
{
	STACK_TRACE;
}

void ConfirmPlayMenuUI::AddControls()
{
	STACK_TRACE;
	GameModeSettings *settings = GetGameState()->GetGameApp()->GetModeSettings();
	Gwen::Controls::Canvas *canvas = GetCanvas();
	
	Gwen::Controls::Label *lblTitle = new Gwen::Controls::Label(canvas, "lblTitle");
	lblTitle->SetText("Confirm Choices");
	lblTitle->SetAlignment(Gwen::Pos::CenterH);
	lblTitle->SetTextColor(UI_COLOR_TITLE);
	
	Gwen::Controls::Label *lblGameType = new Gwen::Controls::Label(canvas, "lblGameType");
	lblGameType->SetText("Game Type:");
	lblGameType->SetAlignment(Gwen::Pos::Left);
	lblGameType->SetTextColor(UI_COLOR_TEXT);
	
	Gwen::Controls::Label *lblGameTypeSelection = new Gwen::Controls::Label(canvas, "lblGameTypeSelection");
	lblGameTypeSelection->SetAlignment(Gwen::Pos::Right);
	lblGameTypeSelection->SetTextColor(UI_COLOR_SELECTION);
	if (settings->type == GAME_TYPE_TIMED)
		lblGameTypeSelection->SetText("Timed");
	else if (settings->type == GAME_TYPE_ENDLESS)
		lblGameTypeSelection->SetText("Endless");
	
	Gwen::Controls::Label *lblDifficulty = new Gwen::Controls::Label(canvas, "lblDifficulty");
	lblDifficulty->SetText("Difficulty:");
	lblDifficulty->SetAlignment(Gwen::Pos::Left);
	lblDifficulty->SetTextColor(UI_COLOR_TEXT);
	
	Gwen::Controls::Label *lblDifficultySelection = new Gwen::Controls::Label(canvas, "lblDifficultySelection");
	lblDifficultySelection->SetAlignment(Gwen::Pos::Right);
	lblDifficultySelection->SetTextColor(UI_COLOR_SELECTION);
	if (settings->difficulty == GAME_DIFFICULTY_EASY)
		lblDifficultySelection->SetText("Easy");
	else if (settings->difficulty == GAME_DIFFICULTY_NORMAL)
		lblDifficultySelection->SetText("Normal");
	else if (settings->difficulty == GAME_DIFFICULTY_HARD)
		lblDifficultySelection->SetText("Hard");
	
	if (settings->type == GAME_TYPE_TIMED)
	{
		Gwen::Controls::Label *lblTime = new Gwen::Controls::Label(canvas, "lblTime");
		lblTime->SetText("Round Length:");
		lblTime->SetAlignment(Gwen::Pos::Left);
		lblTime->SetTextColor(UI_COLOR_TEXT);
		
		Gwen::Controls::Label *lblTimeSelection = new Gwen::Controls::Label(canvas, "lblTimeSelection");
		lblTimeSelection->SetAlignment(Gwen::Pos::Right);
		lblTimeSelection->SetTextColor(UI_COLOR_SELECTION);
		
		uint32_t duration = settings->durationMs;
		int minutes = duration / 1000 / 60;
		duration -= (minutes * 60 * 1000);
		int seconds = duration / 1000;
		
		char timeString[6];
		snprintf(timeString, 6, "%2d:%02d", minutes, seconds);
		
		lblTimeSelection->SetText(timeString);
	}
	
	Gwen::Controls::Button *btnPlay = new Gwen::Controls::Button(canvas, "btnPlay");
	btnPlay->SetText("Play!");
	btnPlay->onPress.Add(this, &ConfirmPlayMenuUI::OnPlay);
	
	Gwen::Controls::Button *btnBack = new Gwen::Controls::Button(canvas, "btnBack");
	btnBack->SetText("Back");
	btnBack->onPress.Add(this, &ConfirmPlayMenuUI::OnBack);
}

void ConfirmPlayMenuUI::PositionControls()
{
	STACK_TRACE;
	GameModeSettings *settings = GetGameState()->GetGameApp()->GetModeSettings();
	Gwen::Controls::Canvas *canvas = GetCanvas();
	
	Gwen::Controls::Label *lblTitle = canvas->FindChild<Gwen::Controls::Label>("lblTitle");
	Gwen::Controls::Label *lblGameType = canvas->FindChild<Gwen::Controls::Label>("lblGameType");
	Gwen::Controls::Label *lblGameTypeSelection = canvas->FindChild<Gwen::Controls::Label>("lblGameTypeSelection");
	Gwen::Controls::Label *lblDifficulty = canvas->FindChild<Gwen::Controls::Label>("lblDifficulty");
	Gwen::Controls::Label *lblDifficultySelection = canvas->FindChild<Gwen::Controls::Label>("lblDifficultySelection");
	Gwen::Controls::Label *lblTime = canvas->FindChild<Gwen::Controls::Label>("lblTime");
	Gwen::Controls::Label *lblTimeSelection = canvas->FindChild<Gwen::Controls::Label>("lblTimeSelection");
	Gwen::Controls::Button *btnPlay = canvas->FindChild<Gwen::Controls::Button>("btnPlay");
	Gwen::Controls::Button *btnBack = canvas->FindChild<Gwen::Controls::Button>("btnBack");
	
	lblTitle->SizeToContents();   // this is really just to get the right height automatically...
	lblTitle->SetWidth(TITLE_LABEL_WIDTH);
	
	int left = canvas->Right() - MENU_SIDE_WIDTH + ((MENU_SIDE_WIDTH - BUTTON_WIDTH) / 2);
	int top = TITLE_LABEL_SPACER;
	
	lblTitle->SetPos(left, top);
	
	top += lblTitle->Height() + TITLE_LABEL_SPACER;
	
	lblGameType->SizeToContents();
	lblGameType->SetWidth(TEXT_LABEL_WIDTH);
	lblGameType->SetPos(left, top);
	top += lblGameType->Height();
	lblGameTypeSelection->SizeToContents();
	lblGameTypeSelection->SetWidth(TEXT_LABEL_WIDTH);
	lblGameTypeSelection->SetPos(left, top);
	top += lblGameTypeSelection->Height();
	
	top += TEXT_LABEL_SPACER;
	
	lblDifficulty->SizeToContents();
	lblDifficulty->SetWidth(TEXT_LABEL_WIDTH);
	lblDifficulty->SetPos(left, top);
	top += lblDifficulty->Height();
	lblDifficultySelection->SizeToContents();
	lblDifficultySelection->SetWidth(TEXT_LABEL_WIDTH);
	lblDifficultySelection->SetPos(left, top);
	top += lblDifficultySelection->Height();
	
	top += TEXT_LABEL_SPACER;
	
	if (settings->type == GAME_TYPE_TIMED)
	{
		lblTime->SizeToContents();
		lblTime->SetWidth(TEXT_LABEL_WIDTH);
		lblTime->SetPos(left, top);
		top += lblTime->Height();
		lblTimeSelection->SizeToContents();
		lblTimeSelection->SetWidth(TEXT_LABEL_WIDTH);
		lblTimeSelection->SetPos(left, top);
		top += lblTimeSelection->Height();
		
		top += TEXT_LABEL_SPACER;
	}
	
	btnPlay->SetSize(BUTTON_WIDTH, BUTTON_HEIGHT);
	btnPlay->SetPos(left, top);
	
	top = canvas->Bottom() - BUTTON_SPACING - BUTTON_HEIGHT;
	
	btnBack->SetSize(BUTTON_WIDTH, BUTTON_HEIGHT);
	btnBack->SetPos(left, top);
}

void ConfirmPlayMenuUI::OnPlay(Gwen::Controls::Base *sender)
{
	STACK_TRACE;
	GetStateManager()->SwapTopNonOverlayWith<GamePlayState>();
}

void ConfirmPlayMenuUI::OnBack(Gwen::Controls::Base *sender)
{
	STACK_TRACE;
	GameModeSettings *settings = GetGameState()->GetGameApp()->GetModeSettings();
	if (settings->type == GAME_TYPE_TIMED)
		GetStateManager()->SwapTopWith<GwenGameState>()->SetGwenHandler<TimeMenuUI>();
	else
		GetStateManager()->SwapTopWith<GwenGameState>()->SetGwenHandler<DifficultyMenuUI>();
		
}
