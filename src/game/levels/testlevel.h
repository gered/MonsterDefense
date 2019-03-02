#ifndef __GAME_LEVELS_TESTLEVEL_H_INCLUDED__
#define __GAME_LEVELS_TESTLEVEL_H_INCLUDED__

#include "../level.h"

class World;

class TestLevel : public Level
{
public:
	TestLevel(World *world);
	virtual ~TestLevel();

	BOOL Load();
	void OnStartGame();
	void OnUpdate(float delta);

private:
	float m_secondsSinceLastMobSpawnAttempt;
	float m_secondsSinceLastChestSpawnAttempt;
};

#endif
