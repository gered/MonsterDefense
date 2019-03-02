#ifndef __GAME_MAINMENU_TITLETEXTPROCESS_H_INCLUDED__
#define __GAME_MAINMENU_TITLETEXTPROCESS_H_INCLUDED__

#include "../../processes/gameprocess.h"
#include <stl/string.h>

class GameState;
class ProcessManager;
class RenderContext;

class TitleTextProcess : public GameProcess
{
public:
	static GAMEPROCESS_TYPE GetType()
	{
		static GAMEPROCESS_TYPE typeName = "TitleTextProcess";
		return typeName;
	}
	GAMEPROCESS_TYPE GetTypeOf() const                   { return GetType(); }
	
	TitleTextProcess(GameState *gameState, ProcessManager *processManager);
	virtual ~TitleTextProcess();
	
	void OnRender(RenderContext *renderContext);
	
private:
	stl::string m_versionText;
};

#endif

