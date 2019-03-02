#ifndef __GAME_OPTIONS_OPTIONSDIALOGUI_H_INCLUDED__
#define __GAME_OPTIONS_OPTIONSDIALOGUI_H_INCLUDED__

#include "../../states/gwengamestateuicontroller.h"

class GwenGameState;

namespace Gwen
{
	namespace Controls
	{
		class Base;
	}
}

class OptionsDialogUI : public GwenGameStateUIController
{
public:
	OptionsDialogUI(GwenGameState *gameState);
	virtual ~OptionsDialogUI();
	
	virtual void OnPush();
	virtual void OnPop();
	virtual void OnResize();
	virtual void OnUpdate(float delta);
	
	void OnOK(Gwen::Controls::Base *sender);
	void OnCancel(Gwen::Controls::Base *sender);
	
private:
	void PositionControls();
};

#endif
