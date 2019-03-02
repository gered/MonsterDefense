#ifndef __GAME_MAINMENU_TIMEMENUUI_H_INCLUDED__
#define __GAME_MAINMENU_TIMEMENUUI_H_INCLUDED__

#include "basemainmenuui.h"

class GwenGameState;

namespace Gwen
{
	namespace Controls
	{
		class Base;
	}
}

class TimeMenuUI : public BaseMainMenuUI
{
public:
	TimeMenuUI(GwenGameState *gameState);
	virtual ~TimeMenuUI();
	
	void On3Min(Gwen::Controls::Base *sender);
	void On5Min(Gwen::Controls::Base *sender);
	void On10Min(Gwen::Controls::Base *sender);
	void OnBack(Gwen::Controls::Base *sender);
	
protected:
	void AddControls();
	void PositionControls();
};

#endif
