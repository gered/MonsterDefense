#ifndef __GAME_WORLD_H_INCLUDED__
#define __GAME_WORLD_H_INCLUDED__

#include "../framework/common.h"
#include "../events/eventlistenerex.h"
#include "../framework/math/point3.h"

#include <stl/vector.h>

class BuffCollection;
class ChunkVertexGenerator;
class EntityManager;
class GameApp;
class Level;
class PowerUpCollection;
class RenderContext;
class TileMeshCollection;
class TileMap;
class WeaponCollection;
struct Event;
struct Point3;

typedef stl::vector<Point3> WalkableTilesList;

class World : public EventListenerEx
{
public:
	World(GameApp *gameApp);
	virtual ~World();

	BOOL Initialize();

	void OnLostContext();
	void OnNewContext();
	void OnRender(RenderContext *renderContext);
	void OnResize();
	void OnUpdate(float delta);
	BOOL Handle(const Event *event);

	template<class T> void SetLevel(); 

	GameApp* GetGameApp() const                            { return m_gameApp; }
	EntityManager* GetEntityManager() const                { return m_entityManager; }
	TileMap* GetTileMap() const                            { return m_tileMap; }
	
	PowerUpCollection* GetPowerUps() const                 { return m_powerUps; }
	BuffCollection* GetBuffs() const                       { return m_buffs; }
	WeaponCollection* GetWeapon() const                    { return m_weapons; }

	void AddWalkableTile(const Point3 &tileCoords);
	Point3 GetRandomWalkableSpace();

private:
	void RegisterEntityDataClasses();
	void SetLevel(Level *level);
	void UpdateMonsterPower();

	BOOL m_isGameStarted;
	uint32_t m_elapsedTicks;
	GameApp *m_gameApp;
	EntityManager *m_entityManager;
	PowerUpCollection *m_powerUps;
	BuffCollection *m_buffs;
	WeaponCollection *m_weapons;
	TileMeshCollection *m_tileMeshes;
	ChunkVertexGenerator *m_chunkVertexGenerator;
	TileMap *m_tileMap;
	Level *m_level;
	WalkableTilesList m_walkableTiles;
};

template<class T>
void World::SetLevel()
{
	T *level = new T(this);
	SetLevel(level);
}

inline void World::AddWalkableTile(const Point3 &tileCoords)
{
	m_walkableTiles.push_back(tileCoords);
}

#endif
