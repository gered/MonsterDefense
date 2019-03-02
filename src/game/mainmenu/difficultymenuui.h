#ifndef __GAME_MAINMENU_DIFFICULTYMENUUI_H_INCLUDED__
#define __GAME_MAINMENU_DIFFICULTYMENUUI_H_INCLUDED__

#include "basemainmenuui.h"

class GwenGameState;

namespace Gwen
{
	namespace Controls
	{
		class Base;
	}
}

class DifficultyMenuUI : public BaseMainMenuUI
{
public:
	DifficultyMenuUI(GwenGameState *gameState);
	virtual ~DifficultyMenuUI();
	
	void OnEasy(Gwen::Controls::Base *sender);
	void OnNormal(Gwen::Controls::Base *sender);
	void OnHard(Gwen::Controls::Base *sender);
	void OnBack(Gwen::Controls::Base *sender);
	
protected:
	void AddControls();
	void PositionControls();
};

#endif
