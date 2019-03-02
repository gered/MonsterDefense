#ifndef __GAME_MAINMENU_BASEMAINMENUUI_H_INCLUDED__
#define __GAME_MAINMENU_BASEMAINMENUUI_H_INCLUDED__

#include "../../states/gwengamestateuicontroller.h"

class GwenGameState;

namespace Gwen
{
	namespace Controls
	{
		class Base;
	}
}

class BaseMainMenuUI : public GwenGameStateUIController
{
public:
	BaseMainMenuUI(GwenGameState *gameState);
	virtual ~BaseMainMenuUI();
	
	virtual void OnPush();
	virtual void OnPop();
	virtual void OnResize();
	BOOL OnTransition(float delta, BOOL isTransitioningOut, BOOL started);
	
protected:
	virtual void AddControls()                             {}
	virtual void PositionControls()                        {}
	
private:
	float m_fadeProgress;
	float m_fadeAlpha;
	float m_fadeToAlpha;
	float m_fadeFromAlpha;
};

#endif
