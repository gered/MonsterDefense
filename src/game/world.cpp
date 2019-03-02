#include "../framework/debug.h"

#include "world.h"
#include "gamemodesettings.h"
#include "level.h"
#include "entities/buffs/buffcollection.h"
#include "entities/globalcomponents/monsterpowercomponent.h"
#include "entities/powerups/powerupcollection.h"
#include "entities/weapons/weaponcollection.h"
#include "events/introfinishedevent.h"
#include "../gameapp.h"
#include "../contexts/contentcache.h"
#include "../contexts/rendercontext.h"
#include "../events/event.h"
#include "../entities/entity.h"
#include "../entities/entitymanager.h"
#include "../framework/graphics/color.h"
#include "../framework/graphics/graphicsdevice.h"
#include "../framework/graphics/standardshader.h"
#include "../framework/math/matrix3x3.h"
#include "../framework/math/matrix4x4.h"
#include "../framework/math/point3.h"
#include "../framework/math/vector3.h"
#include "../tilemap/chunkvertexgenerator.h"
#include "../tilemap/tilemap.h"
#include "../tilemap/tilemesh.h"
#include "../tilemap/tilemeshcollection.h"
#include "../tilemap/tilemeshdefs.h"
#include "../tilemap/tilemaprenderer.h"

World::World(GameApp *gameApp)
	: EventListenerEx(gameApp->GetEventManager())
{
	STACK_TRACE;
	ASSERT(gameApp != NULL);
	m_gameApp = gameApp;

	m_entityManager = NULL;
	m_powerUps = NULL;
	m_buffs = NULL;
	m_weapons = NULL;
	m_chunkVertexGenerator = NULL;
	m_tileMap = NULL;
	m_tileMeshes = NULL;
	m_isGameStarted = FALSE;
	m_elapsedTicks = 0;
	m_level = NULL;
	
	ListenFor<IntroFinishedEvent>();
}

World::~World()
{
	STACK_TRACE;
	SAFE_DELETE(m_level);
	SAFE_DELETE(m_powerUps);
	SAFE_DELETE(m_buffs);
	SAFE_DELETE(m_weapons);
	SAFE_DELETE(m_entityManager);
	SAFE_DELETE(m_tileMap);
	SAFE_DELETE(m_chunkVertexGenerator);
	SAFE_DELETE(m_tileMeshes);
	
	StopListeningFor<IntroFinishedEvent>();
}

BOOL World::Initialize()
{
	STACK_TRACE;

	m_tileMeshes = new TileMeshCollection(m_gameApp->GetContentCache()->GetEnvironment());

	// add all tile meshes used by any level
	m_tileMeshes->AddCube(0, SIDE_ALL);
	m_tileMeshes->AddCube(1, SIDE_ALL);
	m_tileMeshes->AddCube(2, SIDE_ALL);
	m_tileMeshes->AddCube(3, SIDE_ALL);
	m_tileMeshes->AddCube(4, SIDE_ALL);
	m_tileMeshes->AddCube(5, SIDE_ALL);

	m_chunkVertexGenerator = new ChunkVertexGenerator();
	m_tileMap = new TileMap(m_tileMeshes, m_chunkVertexGenerator, NULL, m_gameApp->GetGraphicsDevice());

	m_entityManager = new EntityManager(m_gameApp->GetEventManager());

	RegisterEntityDataClasses();
	
	UpdateMonsterPower();

	return TRUE;
}

void World::OnLostContext()
{
	STACK_TRACE;
	m_entityManager->OnLostContext();
}

void World::OnNewContext()
{
	STACK_TRACE;
	m_entityManager->OnNewContext();
}

void World::OnRender(RenderContext *renderContext)
{
	STACK_TRACE;
	// prep shader for rendering the world with
	Matrix4x4 worldPos = Matrix4x4::CreateTranslation(0.0f, 0.0f, 0.0f);
	Matrix4x4 worldModelView = renderContext->GetGraphicsDevice()->GetModelViewMatrix() * worldPos;
	Matrix3x3 worldModelViewNormal = Matrix3x3::Transpose(Matrix3x3::Inverse(Matrix3x3(worldPos)));
	renderContext->GetNormalTerrainShader()->SetModelViewMatrix(worldModelView);
	renderContext->GetNormalTerrainShader()->SetProjectionMatrix(renderContext->GetGraphicsDevice()->GetProjectionMatrix());
	renderContext->GetNormalTerrainShader()->SetUniform("u_ambient", renderContext->GetAmbientLight());
	renderContext->GetNormalTerrainShader()->SetUniform("u_lightDir", renderContext->GetCommonLightDir());
	renderContext->GetNormalTerrainShader()->SetUniform("u_lightColor", COLOR_WHITE);
	renderContext->GetNormalTerrainShader()->SetUniform("u_modelViewNormalMatrix", worldModelViewNormal);

	renderContext->GetTileMapRenderer()->Render(m_tileMap, renderContext->GetNormalTerrainShader());
	m_entityManager->OnRender(renderContext);
	renderContext->GetTileMapRenderer()->RenderAlpha(m_tileMap, renderContext->GetNormalTerrainShader());
}

void World::OnResize()
{
	STACK_TRACE;
	m_entityManager->OnResize();
}

void World::OnUpdate(float delta)
{
	STACK_TRACE;
	if (m_isGameStarted)
		m_elapsedTicks += (uint32_t)(delta * 1000);

	m_entityManager->OnUpdate(delta);

	if (m_level != NULL)
		m_level->OnUpdate(delta);
	
	UpdateMonsterPower();
}

BOOL World::Handle(const Event *event)
{
	STACK_TRACE;
	if (event->Is<IntroFinishedEvent>())
	{
		ASSERT(m_level != NULL);
		if (m_level != NULL)
		{
			m_level->OnStartGame();
			m_isGameStarted = TRUE;
		}
	}
	return FALSE;
}

void World::SetLevel(Level *level)
{
	STACK_TRACE;
	ASSERT(level != NULL);
	
	// clear existing level if any
	// TODO: other pre-cleaning steps here?
	SAFE_DELETE(m_level);

	m_level = level;
	m_walkableTiles.clear();
	BOOL loadResult = m_level->Load();
	ASSERT(loadResult == TRUE);
}

Point3 World::GetRandomWalkableSpace()
{
	STACK_TRACE;
	ASSERT(m_walkableTiles.size() > 0);
	if (m_walkableTiles.size() == 0)
		return ZERO_POINT;
	
	int32_t walkableTileIndex = Rnd(0, m_walkableTiles.size() - 1);
	return m_walkableTiles[walkableTileIndex];
}

void World::UpdateMonsterPower()
{
	STACK_TRACE;
	GameModeSettings *modeSettings = GetGameApp()->GetModeSettings();
	MonsterPowerComponent *monsterPower = GetEntityManager()->GetGlobalComponent<MonsterPowerComponent>();
	ASSERT(monsterPower != NULL);
	
	float attackModifier = 1.0f;
	float defenseModifier = 1.0f;
	float spawnRateModifier = 0.0f;
	float attackPercentAboveBase = 0.0f;
	float defensePercentAboveBase = 0.0f;
	float spawnRatePercentAboveBase = 0.0f;
	
	// base modifiers set via current game difficulty
	switch (modeSettings->difficulty)
	{
		case GAME_DIFFICULTY_EASY:
			attackModifier = 0.75f;
			defenseModifier = 1.0f;
			spawnRateModifier = 0.0f;
			break;
			
		case GAME_DIFFICULTY_NORMAL:
			attackModifier = 1.0f;
			defenseModifier = 1.0f;
			spawnRateModifier = 0.2f;
			break;
		
		case GAME_DIFFICULTY_HARD:
			attackModifier = 1.25f;
			defenseModifier = 1.0f;
			spawnRateModifier = 0.4f;
			break;
	}
	
	if (GetGameApp()->GetModeSettings()->type == GAME_TYPE_ENDLESS)
	{
		// calculate added monster stat bonuses based on elapsed game time
		uint32_t scalingAmount = m_elapsedTicks / 10000;
		attackPercentAboveBase = (0.01f * scalingAmount);
		defensePercentAboveBase = 0.0f;
		spawnRatePercentAboveBase = (0.01f * scalingAmount);
		
		// add extra bonuses to base modifiers
		attackModifier *= (1.0f + attackPercentAboveBase);
		defenseModifier *= (1.0f + defensePercentAboveBase);
		spawnRateModifier *= (1.0f + spawnRatePercentAboveBase);
	}

	monsterPower->attackModifier = attackModifier;
	monsterPower->defenseModifier = defenseModifier;
	monsterPower->spawnRateModifier = spawnRateModifier;
	monsterPower->attackPercentAboveBase = attackPercentAboveBase;
	monsterPower->defensePercentAboveBase = defensePercentAboveBase;
	monsterPower->spawnRatePercentAboveBase = spawnRatePercentAboveBase;
}
