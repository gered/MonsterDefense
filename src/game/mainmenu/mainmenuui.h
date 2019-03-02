#ifndef __GAME_MAINMENU_MAINMENUUI_H_INCLUDED__
#define __GAME_MAINMENU_MAINMENUUI_H_INCLUDED__

#include "basemainmenuui.h"

class GwenGameState;

namespace Gwen
{
	namespace Controls
	{
		class Base;
	}
}

class MainMenuUI : public BaseMainMenuUI
{
public:
	MainMenuUI(GwenGameState *gameState);
	virtual ~MainMenuUI();
	
	void OnPlay(Gwen::Controls::Base *sender);
	void OnOptions(Gwen::Controls::Base *sender);
	void OnQuit(Gwen::Controls::Base *sender);
	
protected:
	void AddControls();
	void PositionControls();
};

#endif
