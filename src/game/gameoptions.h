#ifndef __GAME_GAMEOPTIONS_H_INCLUDED__
#define __GAME_GAMEOPTIONS_H_INCLUDED__

#include "../framework/common.h"

struct GameOptions
{
	GameOptions();
	
	BOOL showMonsterHealth;
	BOOL showDebugInfo;
	float virtualGamePadSize;
};

inline GameOptions::GameOptions()
{
	showMonsterHealth = FALSE;
	showDebugInfo = FALSE;
	virtualGamePadSize = 1.0f;
}

#endif
