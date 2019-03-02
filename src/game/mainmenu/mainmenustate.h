#ifndef __GAME_MAINMENU_MAINMENUSTATE_H_INCLUDED__
#define __GAME_MAINMENU_MAINMENUSTATE_H_INCLUDED__

#include "../../states/gamestate.h"

class GameApp;
class OverheadFollowCamera;
class RenderContext;
class StateManager;
class World;
struct Event;

class MainMenuState : public GameState
{
public:
	static GAMESTATE_TYPE GetType()
	{
		static GAMESTATE_TYPE typeName = "MainMenuState";
		return typeName;
	}
	GAMESTATE_TYPE GetTypeOf() const                       { return GetType(); }
	
	MainMenuState(GameApp *gameApp, StateManager *stateManager);
	virtual ~MainMenuState();
	
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
	
	World* GetWorld() const                                { return m_world; }
	uint16_t GetMenuSideWidth() const                      { return m_menuSideWidth; }
	
private:
	World *m_world;
	OverheadFollowCamera *m_camera;
	
	float m_rotation;
	uint16_t m_menuSideWidth;
};

#endif

