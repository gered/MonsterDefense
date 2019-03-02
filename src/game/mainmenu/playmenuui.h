#ifndef __GAME_MAINMENU_PLAYMENUUI_H_INCLUDED__
#define __GAME_MAINMENU_PLAYMENUUI_H_INCLUDED__

#include "basemainmenuui.h"

class GwenGameState;

namespace Gwen
{
	namespace Controls
	{
		class Base;
	}
}

class PlayMenuUI : public BaseMainMenuUI
{
public:
	PlayMenuUI(GwenGameState *gameState);
	virtual ~PlayMenuUI();
	
	void OnTimed(Gwen::Controls::Base *sender);
	void OnEndless(Gwen::Controls::Base *sender);
	void OnBack(Gwen::Controls::Base *sender);
	
protected:
	void AddControls();
	void PositionControls();
};

#endif
