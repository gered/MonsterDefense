#include "../../framework/debug.h"

#include "timemenuui.h"
#include "confirmplaymenuui.h"
#include "difficultymenuui.h"
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

TimeMenuUI::TimeMenuUI(GwenGameState *gameState)
	: BaseMainMenuUI(gameState)
{
	STACK_TRACE;
}

TimeMenuUI::~TimeMenuUI()
{
	STACK_TRACE;
}

void TimeMenuUI::AddControls()
{
	STACK_TRACE;
	Gwen::Controls::Canvas *canvas = GetCanvas();
	
	Gwen::Controls::Label *lblTitle = new Gwen::Controls::Label(canvas, "lblTitle");
	lblTitle->SetText("Round Length");
	lblTitle->SetAlignment(Gwen::Pos::CenterH);
	lblTitle->SetTextColor(UI_COLOR_TITLE);
	
	Gwen::Controls::Button *btn3Min = new Gwen::Controls::Button(canvas, "btn3Min");
	btn3Min->SetText("3:00");
	btn3Min->onPress.Add(this, &TimeMenuUI::On3Min);
	
	Gwen::Controls::Button *btn5Min = new Gwen::Controls::Button(canvas, "btn5Min");
	btn5Min->SetText("5:00");
	btn5Min->onPress.Add(this, &TimeMenuUI::On5Min);
	
	Gwen::Controls::Button *btn10Min = new Gwen::Controls::Button(canvas, "btn10Min");
	btn10Min->SetText("10:00");
	btn10Min->onPress.Add(this, &TimeMenuUI::On10Min);
	
	Gwen::Controls::Button *btnBack = new Gwen::Controls::Button(canvas, "btnBack");
	btnBack->SetText("Back");
	btnBack->onPress.Add(this, &TimeMenuUI::OnBack);
}

void TimeMenuUI::PositionControls()
{
	STACK_TRACE;
	Gwen::Controls::Canvas *canvas = GetCanvas();
	
	Gwen::Controls::Label *lblTitle = canvas->FindChild<Gwen::Controls::Label>("lblTitle");
	Gwen::Controls::Button *btn3Min = canvas->FindChild<Gwen::Controls::Button>("btn3Min");
	Gwen::Controls::Button *btn5Min = canvas->FindChild<Gwen::Controls::Button>("btn5Min");
	Gwen::Controls::Button *btn10Min = canvas->FindChild<Gwen::Controls::Button>("btn10Min");
	Gwen::Controls::Button *btnBack = canvas->FindChild<Gwen::Controls::Button>("btnBack");
	
	lblTitle->SizeToContents();   // this is really just to get the right height automatically...
	lblTitle->SetWidth(TITLE_LABEL_WIDTH);
	
	int left = canvas->Right() - MENU_SIDE_WIDTH + ((MENU_SIDE_WIDTH - BUTTON_WIDTH) / 2);
	int top = TITLE_LABEL_SPACER;
	
	lblTitle->SetPos(left, top);
	
	top += lblTitle->Height() + TITLE_LABEL_SPACER;
	
	btn3Min->SetSize(BUTTON_WIDTH, BUTTON_HEIGHT);
	btn3Min->SetPos(left, top);
	
	top += BUTTON_HEIGHT + BUTTON_SPACING;
	
	btn5Min->SetSize(BUTTON_WIDTH, BUTTON_HEIGHT);
	btn5Min->SetPos(left, top);
	
	top += BUTTON_HEIGHT + BUTTON_SPACING;
	
	btn10Min->SetSize(BUTTON_WIDTH, BUTTON_HEIGHT);
	btn10Min->SetPos(left, top);
	
	top = canvas->Bottom() - BUTTON_SPACING - BUTTON_HEIGHT;
	
	btnBack->SetSize(BUTTON_WIDTH, BUTTON_HEIGHT);
	btnBack->SetPos(left, top);
}

void TimeMenuUI::On3Min(Gwen::Controls::Base *sender)
{
	STACK_TRACE;
	GameModeSettings *settings = GetGameState()->GetGameApp()->GetModeSettings();
	settings->durationMs = 1000 * 60 * 3;
	GetStateManager()->SwapTopWith<GwenGameState>()->SetGwenHandler<ConfirmPlayMenuUI>();
}

void TimeMenuUI::On5Min(Gwen::Controls::Base *sender)
{
	STACK_TRACE;
	GameModeSettings *settings = GetGameState()->GetGameApp()->GetModeSettings();
	settings->durationMs = 1000 * 60 * 5;
	GetStateManager()->SwapTopWith<GwenGameState>()->SetGwenHandler<ConfirmPlayMenuUI>();
}

void TimeMenuUI::On10Min(Gwen::Controls::Base *sender)
{
	STACK_TRACE;
	GameModeSettings *settings = GetGameState()->GetGameApp()->GetModeSettings();
	settings->durationMs = 1000 * 60 * 10;
	GetStateManager()->SwapTopWith<GwenGameState>()->SetGwenHandler<ConfirmPlayMenuUI>();
}

void TimeMenuUI::OnBack(Gwen::Controls::Base *sender)
{
	STACK_TRACE;
	GetStateManager()->SwapTopWith<GwenGameState>()->SetGwenHandler<DifficultyMenuUI>();
}
