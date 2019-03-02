#ifndef __GAME_GAMEPLAY_GAMEPLAYSTATE_H_INCLUDED__
#define __GAME_GAMEPLAY_GAMEPLAYSTATE_H_INCLUDED__

#include "../../states/gamestate.h"

class GameApp;
class OverheadFollowCamera;
class RenderContext;
class StateManager;
class World;
struct Event;

class GamePlayState : public GameState
{
public:
	static GAMESTATE_TYPE GetType()
	{
		static GAMESTATE_TYPE typeName = "GamePlayState";
		return typeName;
	}
	GAMESTATE_TYPE GetTypeOf() const                       { return GetType(); }

	GamePlayState(GameApp *gameApp, StateManager *stateManager);
	virtual ~GamePlayState();

	void OnPush();
	void OnPop();
	void OnPause(BOOL dueToOverlay);
	void OnResume(BOOL fromOverlay);
	void OnAppGainFocus();
	void OnAppLostFocus();
	void OnLostContext();
	void OnNewContext();
	void OnRender(RenderContext *renderContext);
	void OnResize();
	void OnUpdate(float delta);
	BOOL OnTransition(float delta, BOOL isTransitioningOut, BOOL started);

	BOOL Handle(const Event *event);
	
	int32_t GetPlayerScore() const                         { return m_playerScore; }
	
	World* GetWorld() const                                { return m_world; }                               

private:
	World *m_world;
	OverheadFollowCamera *m_camera;
	
	BOOL m_missionFailure;
	BOOL m_missionSuccess;
	float m_timeLeftBeforeShowMissionResultScreen;
	
	BOOL m_introIsRunning;
	
	int32_t m_playerScore;
};

#endif

