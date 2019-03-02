#ifndef __GAME_GAMEPLAY_INTROCOUNTDOWNSTATE_H_INCLUDED__
#define __GAME_GAMEPLAY_INTROCOUNTDOWNSTATE_H_INCLUDED__

#include "../../states/gamestate.h"

class GameApp;
class RenderContext;
class StateManager;

class IntroCountdownState : public GameState
{
public:
	static GAMESTATE_TYPE GetType()
	{
		static GAMESTATE_TYPE typeName = "IntroCountdownState";
		return typeName;
	}
	GAMESTATE_TYPE GetTypeOf() const                       { return GetType(); }
	
	IntroCountdownState(GameApp *gameApp, StateManager *stateManager);
	virtual ~IntroCountdownState();
	
	void OnPush();
	void OnPop();
	void OnAppLostFocus();
	void OnRender(RenderContext *renderContext);
	void OnUpdate(float delta);
	
private:
	float m_initialWaitTime;
	float m_time;
	float m_alpha;
	float m_scale;
	int m_number;
	char m_message[5];
};

#endif
