#ifndef __GAME_DEBUGINFOPROCESS_H_INCLUDED__
#define __GAME_DEBUGINFOPROCESS_H_INCLUDED__

#include "../processes/gameprocess.h"

class Entity;
class GameState;
class ProcessManager;
class RenderContext;

class DebugInfoProcess : public GameProcess
{
public:
	static GAMEPROCESS_TYPE GetType()
	{
		static GAMEPROCESS_TYPE typeName = "DebugInfoProcess";
		return typeName;
	}
	GAMEPROCESS_TYPE GetTypeOf() const                   { return GetType(); }

	DebugInfoProcess(GameState *gameState, ProcessManager *processManager);
	virtual ~DebugInfoProcess();

	void OnRender(RenderContext *renderContext);
	
private:
	void ShowEntityInfo(Entity *entity, RenderContext *renderContext, uint16_t &y);
};

#endif

