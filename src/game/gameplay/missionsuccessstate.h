#ifndef __GAME_GAMEPLAY_MISSIONSUCCESSSTATE_H_INCLUDED__
#define __GAME_GAMEPLAY_MISSIONSUCCESSSTATE_H_INCLUDED__

#include "../../states/gwengamestateuicontroller.h"

class GwenGameState;
class RenderContext;

namespace Gwen
{
	namespace Controls
	{
		class Base;
	}
}

class MissionSuccessState : public GwenGameStateUIController
{
public:
	MissionSuccessState(GwenGameState *gameState);
	virtual ~MissionSuccessState();
	
	void OnPush();
	void OnPop();
	void OnRender(RenderContext *renderContext);
	void OnResize();
	void OnUpdate(float delta);
	BOOL OnTransition(float delta, BOOL isTransitioningOut, BOOL started);
	
	void OnRetry(Gwen::Controls::Base *sender);
	void OnQuit(Gwen::Controls::Base *sender);
	
private:
	void SetupControls();
	
	float m_elapsedTime;
	float m_textScale;
	float m_textAlpha;
	float m_gwenAlpha;
	uint16_t m_uiAreaHeight;
};

#endif
