#ifndef __GAME_GAMEPLAY_PAUSEMENUUI_H_INCLUDED__
#define __GAME_GAMEPLAY_PAUSEMENUUI_H_INCLUDED__

#include "../../states/gwengamestateuicontroller.h"

class GwenGameState;

namespace Gwen
{
	namespace Controls
	{
		class Base;
	}
}

class PauseMenuUI : public GwenGameStateUIController
{
public:
	PauseMenuUI(GwenGameState *gameState);
	virtual ~PauseMenuUI();
	
	void OnPush();
	void OnPop();
	void OnResize();
	void OnUpdate(float delta);
	
	void OnResumeGame(Gwen::Controls::Base *sender);
	void OnRestart(Gwen::Controls::Base *sender);
	void OnOptions(Gwen::Controls::Base *sender);
	void OnQuit(Gwen::Controls::Base *sender);
	
private:
	void PositionControls();
};

#endif
