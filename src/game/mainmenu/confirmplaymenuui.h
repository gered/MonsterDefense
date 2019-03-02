#ifndef __GAME_MAINMENU_CONFIRMPLAYMENUUI_H_INCLUDED__
#define __GAME_MAINMENU_CONFIRMPLAYMENUUI_H_INCLUDED__

#include "basemainmenuui.h"

class GwenGameState;

namespace Gwen
{
	namespace Controls
	{
		class Base;
	}
}

class ConfirmPlayMenuUI : public BaseMainMenuUI
{
public:
	ConfirmPlayMenuUI(GwenGameState *gameState);
	virtual ~ConfirmPlayMenuUI();
	
	void OnPlay(Gwen::Controls::Base *sender);
	void OnBack(Gwen::Controls::Base *sender);
	
protected:
	void AddControls();
	void PositionControls();
};

#endif
