#ifndef __GAME_GAMEPLAY_STARTMESSAGEPROCESS_H_INCLUDED__
#define __GAME_GAMEPLAY_STARTMESSAGEPROCESS_H_INCLUDED__

#include "../../framework/common.h"
#include "../../processes/gameprocess.h"

class GameState;
class ProcessManager;
class RenderContext;
struct Event;

class StartMessageProcess : public GameProcess
{
public:
	static GAMEPROCESS_TYPE GetType()
	{
		static GAMEPROCESS_TYPE typeName = "StartMessageProcess";
		return typeName;
	}
	GAMEPROCESS_TYPE GetTypeOf() const                   { return GetType(); }
	
	StartMessageProcess(GameState *gameState, ProcessManager *processManager);
	virtual ~StartMessageProcess();
	
	void OnRender(RenderContext *renderContext);
	void OnUpdate(float delta);
	
private:
	float m_time;
	float m_alpha;
	float m_scale;
};

#endif
