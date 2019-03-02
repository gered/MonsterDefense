#ifndef __GAME_GAMEMODESETTINGS_H_INCLUDED__
#define __GAME_GAMEMODESETTINGS_H_INCLUDED__

#include "../framework/common.h"

enum GAME_TYPES
{
	GAME_TYPE_ENDLESS,
	GAME_TYPE_TIMED
};

enum GAME_DIFFICULTIES
{
	GAME_DIFFICULTY_EASY,
	GAME_DIFFICULTY_NORMAL,
	GAME_DIFFICULTY_HARD
};

struct GameModeSettings
{
	GAME_TYPES type;
	GAME_DIFFICULTIES difficulty;
	uint32_t durationMs;
	
	GameModeSettings();
};

inline GameModeSettings::GameModeSettings()
{
	type = GAME_TYPE_ENDLESS;
	difficulty = GAME_DIFFICULTY_NORMAL;
	durationMs = 0;
}

#endif
