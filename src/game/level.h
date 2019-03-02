#ifndef __GAME_LEVEL_H_INCLUDED__
#define __GAME_LEVEL_H_INCLUDED__

#include "../framework/common.h"
#include "../framework/math/vector3.h"
#include "../entities/entitypreset.h"
#include "entities/powerups/powerup.h"

class Entity;
class World;

class Level
{
public:
	Level(World *world);
	virtual ~Level();

	virtual BOOL Load() = 0;
	virtual void OnStartGame() = 0;
	virtual void OnUpdate(float delta) = 0;

	World* GetWorld() const                                { return m_world; }

	template<class T> Entity* AddCharacter(const Vector3 &position, float yOrientation, BOOL suppressSpawnEvent = FALSE);
	template<class T> Entity* AddCharacterAtRandomLocation();
	template<class T> Entity* AddChest(const Vector3 &position, float yOrientation, BOOL suppressSpawnEvent = FALSE);
	template<class T> Entity* AddChestAtRandomLocation();
	
	Entity* AddCharacter(ENTITYPRESET_TYPE presetName, const Vector3 &position, float yOrientation, BOOL suppressSpawnEvent = FALSE);
	Entity* AddCharacterAtRandomLocation(ENTITYPRESET_TYPE presetName);
	Entity* AddChest(POWERUP_TYPE containedPowerUp, const Vector3 &position, float yOrientation, BOOL suppressSpawnEvent = FALSE);
	Entity* AddChestAtRandomLocation(POWERUP_TYPE containedPowerUp);
	
	void AddStarsAt(const Vector3 &position, float spawnRadius = 0.0f);

private:
	World *m_world;
};

template<class T>
Entity* Level::AddCharacter(const Vector3 &position, float yOrientation, BOOL suppressSpawnEvent)
{
	return AddCharacter(T::GetType(), position, yOrientation, suppressSpawnEvent);
}

template<class T>
Entity* Level::AddCharacterAtRandomLocation()
{
	return AddCharacterAtRandomLocation(T::GetType());
}

template<class T>
Entity* Level::AddChest(const Vector3 &position, float yOrientation, BOOL suppressSpawnEvent)
{
	return AddChest(T::GetType(), position, yOrientation, suppressSpawnEvent);
}

template<class T>
Entity* Level::AddChestAtRandomLocation()
{
	return AddChestAtRandomLocation(T::GetType());
}

#endif
