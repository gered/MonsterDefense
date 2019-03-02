#ifndef __GAME_LEVELS_MAINMENULEVEL_H_INCLUDED__
#define __GAME_LEVELS_MAINMENULEVEL_H_INCLUDED__

#include "../level.h"

class World;

class MainMenuLevel : public Level
{
public:
	MainMenuLevel(World *world);
	virtual ~MainMenuLevel();
	
	BOOL Load();
	void OnStartGame();
	void OnUpdate(float delta);
};

#endif
