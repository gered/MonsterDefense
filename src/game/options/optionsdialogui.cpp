#include "../../framework/debug.h"

#include "optionsdialogui.h"
#include "../gameoptions.h"
#include "../events/optionsupdatedevent.h"
#include "../../gameapp.h"
#include "../../contexts/contentcache.h"
#include "../../events/eventmanager.h"
#include "../../input/gamepad.h"
#include "../../input/gamepadbuttons.h"
#include "../../states/gwengamestate.h"
#include "../../states/gwengamestateuicontroller.h"
#include <controls/gwen_canvas.h>
#include <controls/gwen_base.h>
#include <controls/gwen_button.h>
#include <controls/gwen_checkbox.h>
#include <controls/gwen_horizontalslider.h>
#include <controls/gwen_label.h>
#include <controls/gwen_panel.h>
#include <controls/gwen_windowcontrol.h>

OptionsDialogUI::OptionsDialogUI(GwenGameState *gameState)
	: GwenGameStateUIController(gameState)
{
	STACK_TRACE;
}

OptionsDialogUI::~OptionsDialogUI()
{
	STACK_TRACE;
}

void OptionsDialogUI::OnPush()
{
	STACK_TRACE;
	GameOptions *options = GetGameState()->GetGameApp()->GetOptions();
	ContentCache *contentCache = GetGameState()->GetGameApp()->GetContentCache();
	Gwen::Controls::Canvas *canvas = InitializeGwen(contentCache->GetUISkinFilename(), contentCache->GetUIFontFilename(), contentCache->GetUIFontSize());
	
	// just add the controls so they're there. we position them in a bit...
	Gwen::Controls::WindowControl *container = new Gwen::Controls::WindowControl(canvas, "winContainer");
	
	// set initial values from existing options when we create the controls...
	Gwen::Controls::CheckBoxWithLabel *chkShowMonsterHealthBars = new Gwen::Controls::CheckBoxWithLabel(container, "chkShowMonsterHealthBars");
	chkShowMonsterHealthBars->Checkbox()->SetChecked(options->showMonsterHealth);
	
	Gwen::Controls::CheckBoxWithLabel *chkShowDebugInfo = new Gwen::Controls::CheckBoxWithLabel(container, "chkShowDebugInfo");
	chkShowDebugInfo->Checkbox()->SetChecked(options->showDebugInfo);
	
	new Gwen::Controls::Label(container, "lblVirtualGamePadSize");
	Gwen::Controls::HorizontalSlider *hslVirtualGamePadSize = new Gwen::Controls::HorizontalSlider(container, "hslVirtualGamePadSize");
	hslVirtualGamePadSize->SetRange(0.5f, 1.5f);
	hslVirtualGamePadSize->SetFloatValue(options->virtualGamePadSize);
	
	new Gwen::Controls::Button(container, "btnOK");
	new Gwen::Controls::Button(container, "btnCancel");
	
	PositionControls();
}

void OptionsDialogUI::PositionControls()
{
	STACK_TRACE;
	Gwen::Controls::Canvas *canvas = GetCanvas();
	
	Gwen::Controls::WindowControl *container = canvas->FindChild<Gwen::Controls::WindowControl>("winContainer");
	Gwen::Controls::CheckBoxWithLabel *chkShowMonsterHealthBars = container->FindChild<Gwen::Controls::CheckBoxWithLabel>("chkShowMonsterHealthBars", true);
	Gwen::Controls::CheckBoxWithLabel *chkShowDebugInfo = container->FindChild<Gwen::Controls::CheckBoxWithLabel>("chkShowDebugInfo", true);
	Gwen::Controls::Label *lblVirtualGamePadSize = container->FindChild<Gwen::Controls::Label>("lblVirtualGamePadSize", true);
	Gwen::Controls::HorizontalSlider *hslVirtualGamePadSize = container->FindChild<Gwen::Controls::HorizontalSlider>("hslVirtualGamePadSize", true);
	Gwen::Controls::Button *btnOK = container->FindChild<Gwen::Controls::Button>("btnOK", true);
	Gwen::Controls::Button *btnCancel = container->FindChild<Gwen::Controls::Button>("btnCancel", true);
	
	int width = 300;
	int height = 200;
	
	container->SetSize(width, height);
	container->SetTitle("Options");
	container->SetPos((canvas->Width() - container->Width()) / 2, (canvas->Height() - container->Height()) / 2);
	container->onWindowClosed.Add(this, &OptionsDialogUI::OnCancel);
	
	chkShowMonsterHealthBars->SetPosInParentBounds(10, 10);
	chkShowMonsterHealthBars->Label()->SetText("Show Monster Health Bars");
	chkShowMonsterHealthBars->Label()->SizeToContents();
	chkShowMonsterHealthBars->SizeToChildren();
	chkShowMonsterHealthBars->Checkbox()->SetToolTip("Not implemented yet!");
	chkShowMonsterHealthBars->Label()->SetToolTip("Not implemented yet!");
	
	Gwen::Align::PlaceBelow(chkShowDebugInfo, chkShowMonsterHealthBars, 10);
	chkShowDebugInfo->MoveBy(10, 0);
	chkShowDebugInfo->Label()->SetText("Show Debugging Info");
	
	Gwen::Align::PlaceBelow(lblVirtualGamePadSize, chkShowDebugInfo, 20);
	lblVirtualGamePadSize->MoveBy(10, 0);
	lblVirtualGamePadSize->SetText("Virtual Game Pad Size");
	lblVirtualGamePadSize->SizeToContents();
	
	Gwen::Align::PlaceBelow(hslVirtualGamePadSize, lblVirtualGamePadSize, 5);
	hslVirtualGamePadSize->MoveBy(10, 0);
	hslVirtualGamePadSize->SetSize(container->Width() - 30, 20);
	hslVirtualGamePadSize->SetNotchCount(10);
	hslVirtualGamePadSize->SetClampToNotches(false);
	hslVirtualGamePadSize->UpdateBarFromValue();
	hslVirtualGamePadSize->SetToolTip("Not implemented yet!");
	
	int buttonWidth = 100;
	int buttonSpace = 10;
	int buttonX = (container->Width() - buttonWidth - buttonWidth - buttonSpace) / 2;
	
	btnOK->SetSize(100, 30);
	btnOK->SetText("OK");
	btnOK->SetPosInParentBounds(buttonX, container->Height() - 65);
	btnOK->onPress.Add(this, &OptionsDialogUI::OnOK);
	
	btnCancel->SetSize(100, 30);
	btnCancel->SetText("Cancel");
	btnCancel->SetPosInParentBounds(buttonX + buttonWidth + buttonSpace, container->Height() - 65);
	btnCancel->onPress.Add(this, &OptionsDialogUI::OnCancel);
}

void OptionsDialogUI::OnPop()
{
	STACK_TRACE;
}

void OptionsDialogUI::OnResize()
{
	STACK_TRACE;
	GwenGameStateUIController::OnResize();
	PositionControls();
}

void OptionsDialogUI::OnUpdate(float delta)
{
	STACK_TRACE;
	GwenGameStateUIController::OnUpdate(delta);
	
	if (GetGameState()->GetGameApp()->GetGamePad()->IsPressed(BUTTON_BACK))
		SetStateFinished();
}

void OptionsDialogUI::OnOK(Gwen::Controls::Base *sender)
{
	STACK_TRACE;
	GameOptions *options = GetGameState()->GetGameApp()->GetOptions();

	// read the options controls values and set the options in the game app
	Gwen::Controls::CheckBoxWithLabel *chkShowMonsterHealthBars = GetCanvas()->FindChild<Gwen::Controls::CheckBoxWithLabel>("chkShowMonsterHealthBars", true);
	options->showMonsterHealth = chkShowMonsterHealthBars->Checkbox()->IsChecked();
	
	Gwen::Controls::CheckBoxWithLabel *chkShowDebugInfo = GetCanvas()->FindChild<Gwen::Controls::CheckBoxWithLabel>("chkShowDebugInfo", true);
	options->showDebugInfo = chkShowDebugInfo->Checkbox()->IsChecked();
	
	Gwen::Controls::HorizontalSlider *hslVirtualGamePadSize = GetCanvas()->FindChild<Gwen::Controls::HorizontalSlider>("hslVirtualGamePadSize", true);
	options->virtualGamePadSize = hslVirtualGamePadSize->GetFloatValue();
	
	// so that the necessary subsystems etc can respond to the options change...
	OptionsUpdatedEvent optionsUpdatedEvent;
	GetGameState()->GetEventManager()->Trigger(&optionsUpdatedEvent);
	
	SetStateFinished();
}

void OptionsDialogUI::OnCancel(Gwen::Controls::Base *sender)
{
	STACK_TRACE;
	SetStateFinished();
}
