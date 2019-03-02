#include "../../framework/debug.h"

#include "testlevel.h"
#include "../world.h"
#include "../entities/components/chestcomponent.h"
#include "../entities/components/notvisiblecomponent.h"
#include "../entities/components/npccomponent.h"
#include "../entities/components/playercomponent.h"
#include "../entities/components/positioncomponent.h"
#include "../entities/presets/playerpreset.h"
#include "../entities/presets/monsters/bluezombiepreset.h"
#include "../entities/presets/monsters/greenzombiepreset.h"
#include "../entities/presets/monsters/purplezombiepreset.h"
#include "../entities/presets/monsters/redzombiepreset.h"
#include "../entities/powerups/attackbuffpowerup.h"
#include "../entities/powerups/attackspeedbuffpowerup.h"
#include "../entities/powerups/defensebuffpowerup.h"
#include "../../gameapp.h"
#include "../../entities/entity.h"
#include "../../entities/entitymanager.h"
#include "../../entities/entitypreset.h"
#include "../../events/event.h"
#include "../../tilemap/tile.h"
#include "../../tilemap/tilelightdefs.h"
#include "../../tilemap/tilemap.h"

TestLevel::TestLevel(World *world)
	: Level(world)
{
	STACK_TRACE;
}

TestLevel::~TestLevel()
{
	STACK_TRACE;
}

BOOL TestLevel::Load()
{
	STACK_TRACE;

	TileMap *tileMap = GetWorld()->GetTileMap();
	tileMap->SetSize(2, 2, 2, 10, 10, 10);

	// WORLD //////////////////////////////////////////////////////////////////

	for (uint32_t y = 0; y < tileMap->GetHeight(); ++y)
	{
		for (uint32_t z = 0; z < tileMap->GetDepth(); ++z)
		{
			for (uint32_t x = 0; x < tileMap->GetWidth(); ++x)
			{
				Tile *tile = tileMap->Get(x, y, z);

				// wall around the edges (4 high)
				if ((x == 0 || x == tileMap->GetWidth() - 1 || z == 0 || z == tileMap->GetDepth() - 1))
				{
					if (y <= 4)
						tile->Set(3, TILE_COLLIDABLE);
				}

				// middle lowest area is the floor
				else if (y == 0)
				{
					tile->Set(1, TILE_COLLIDABLE);
				}
			}
		}
	}

	// 2x2 pillars in each corner, 5 out from the wall
	for (uint32_t y = 1; y <= 4; ++y)
	{
		for (uint32_t z = 4; z <= 5; ++z)
		{
			for (uint32_t x = 4; x <= 5; ++x)
			{
				tileMap->Get(x, y, z)->Set(5, TILE_COLLIDABLE);
			}
		}
	}

	for (uint32_t y = 1; y <= 4; ++y)
	{
		for (uint32_t z = 15; z >= 14; --z)
		{
			for (uint32_t x = 4; x <= 5; ++x)
			{
				tileMap->Get(x, y, z)->Set(5, TILE_COLLIDABLE);
			}
		}
	}

	for (uint32_t y = 1; y <= 4; ++y)
	{
		for (uint32_t z = 4; z <= 5; ++z)
		{
			for (uint32_t x = 15; x >= 14; --x)
			{
				tileMap->Get(x, y, z)->Set(5, TILE_COLLIDABLE);
			}
		}
	}

	for (uint32_t y = 1; y <= 4; ++y)
	{
		for (uint32_t z = 15; z >= 14; --z)
		{
			for (uint32_t x = 15; x >= 14; --x)
			{
				tileMap->Get(x, y, z)->Set(5, TILE_COLLIDABLE);
			}
		}
	}

	for (uint32_t z = 0; z < tileMap->GetDepth(); ++z)
	{
		for (uint32_t x = 0; x < tileMap->GetWidth(); ++x)
		{
			Tile *tile = tileMap->Get(x, 1, z);
			if (tile->tile == NO_TILE)
			{
				SetBit(TILE_WALKABLE_SURFACE, tile->flags);
				GetWorld()->AddWalkableTile(Point3(x, 1, z));
			}
		}
	}


	///////////////////////////////////////////////////////////////////////////

	tileMap->UpdateVertices();

	Entity *player = AddCharacter<PlayerPreset>(Vector3(10.5f, 1.5f, 10.5f), RADIANS_0);
	player->Add<NotVisibleComponent>();

	m_secondsSinceLastMobSpawnAttempt = 0.0f;
	m_secondsSinceLastChestSpawnAttempt = 8.0f;

	return TRUE;
}

void TestLevel::OnStartGame()
{
	STACK_TRACE;
	Entity *player = GetWorld()->GetEntityManager()->GetWith<PlayerComponent>();
	AddStarsAt(player->Get<PositionComponent>()->position, 0.5f);
	
	player->Remove<NotVisibleComponent>();
	
	//AddCharacter<RedZombiePreset>(Vector3(11.5f, 1.5f, 5.5f), RADIANS_0);
	//AddCharacter<BlueZombiePreset>(Vector3(7.5f, 1.5f, 5.5f), RADIANS_0);
}

void TestLevel::OnUpdate(float delta)
{
	STACK_TRACE;
	m_secondsSinceLastMobSpawnAttempt += delta;
	m_secondsSinceLastChestSpawnAttempt += delta;

	if (m_secondsSinceLastMobSpawnAttempt > 1.0f)
	{
		m_secondsSinceLastMobSpawnAttempt = 0.0f;

		EntityList mobList;
		GetWorld()->GetEntityManager()->GetAllWith<NPCComponent>(mobList);
		uint32_t chance = Rnd(1, 10);
		if (mobList.size() < 15 && chance < 7)
		{
			int mobRoll = Rnd(0, 100);
			if (mobRoll >= 0 && mobRoll < 40)
				AddCharacterAtRandomLocation<PurpleZombiePreset>();
			else if (mobRoll >= 40 && mobRoll < 60)
				AddCharacterAtRandomLocation<BlueZombiePreset>();
			else if (mobRoll >= 60 && mobRoll < 75)
				AddCharacterAtRandomLocation<GreenZombiePreset>();
			else if (mobRoll >= 75)
				AddCharacterAtRandomLocation<RedZombiePreset>();
		}
	}

	if (m_secondsSinceLastChestSpawnAttempt > 8.0f)
	{
		m_secondsSinceLastChestSpawnAttempt = 0.0f;

		uint32_t chance = Rnd(1, 2);
		Entity *existingChest = GetWorld()->GetEntityManager()->GetWith<ChestComponent>();
		if (existingChest == NULL && chance == 1)
		{
			int powerUpRoll = Rnd(1, 3);
			if (powerUpRoll == 1)
				AddChestAtRandomLocation<AttackBuffPowerUp>();
			else if (powerUpRoll == 2)
				AddChestAtRandomLocation<AttackSpeedBuffPowerUp>();
			else if (powerUpRoll == 3)
				AddChestAtRandomLocation<DefenseBuffPowerUp>();
		}
	}
}
