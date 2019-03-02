#include "../../framework/debug.h"

#include "mainmenulevel.h"
#include "../world.h"
#include "../entities/components/animationcomponent.h"
#include "../entities/components/playercomponent.h"
#include "../entities/components/positioncomponent.h"
#include "../entities/presets/playerpreset.h"
#include "../entities/presets/monsters/bluezombiepreset.h"
#include "../entities/presets/monsters/greenzombiepreset.h"
#include "../entities/presets/monsters/purplezombiepreset.h"
#include "../entities/presets/monsters/redzombiepreset.h"
#include "../../gameapp.h"
#include "../../entities/entity.h"
#include "../../entities/entitymanager.h"
#include "../../entities/entitypreset.h"
#include "../../events/event.h"
#include "../../tilemap/tile.h"
#include "../../tilemap/tilelightdefs.h"
#include "../../tilemap/tilemap.h"

MainMenuLevel::MainMenuLevel(World *world)
	: Level(world)
{
	STACK_TRACE;
}

MainMenuLevel::~MainMenuLevel()
{
	STACK_TRACE;
}

BOOL MainMenuLevel::Load()
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
	
	AddCharacter<PlayerPreset>(Vector3(10.5f, 1.5f, 10.5f), RADIANS_0);
	
	AddCharacter<BlueZombiePreset>(Vector3(10.5f, 1.5f, 7.5f), RADIANS_180, TRUE);
	AddCharacter<BlueZombiePreset>(Vector3(9.5f,  1.5f, 7.5f), RADIANS_180, TRUE);
	AddCharacter<GreenZombiePreset>(Vector3(9.5f,  1.5f, 8.5f), RADIANS_180, TRUE);
	AddCharacter<BlueZombiePreset>(Vector3(11.5f, 1.5f, 8.5f), RADIANS_180, TRUE);
	AddCharacter<RedZombiePreset>(Vector3(10.5f, 1.5f, 6.5f), RADIANS_180, TRUE);
	AddCharacter<RedZombiePreset>(Vector3(11.5f, 1.5f, 7.0f), RADIANS_180, TRUE);
	AddCharacter<PurpleZombiePreset>(Vector3(8.0f, 1.5f, 8.5f), RADIANS_180, TRUE);
	AddCharacter<BlueZombiePreset>(Vector3(7.5f, 1.5f, 10.5f), RADIANS_180, TRUE);
	AddCharacter<BlueZombiePreset>(Vector3(7.0f, 1.5f, 9.5f), RADIANS_180, TRUE);
	AddCharacter<BlueZombiePreset>(Vector3(9.0f, 1.5f, 12.5f), RADIANS_180, TRUE);
	AddCharacter<GreenZombiePreset>(Vector3(10.5f, 1.5f, 12.5f), RADIANS_180, TRUE);
	AddCharacter<RedZombiePreset>(Vector3(9.5f, 1.5f, 12.0f), RADIANS_180, TRUE);
	AddCharacter<GreenZombiePreset>(Vector3(9.0f, 1.5f, 10.0f), RADIANS_180, TRUE);
	AddCharacter<GreenZombiePreset>(Vector3(11.5f, 1.5f, 12.5f), RADIANS_180, TRUE);
	AddCharacter<GreenZombiePreset>(Vector3(12.5f, 1.5f, 9.5f), RADIANS_180, TRUE);
	AddCharacter<BlueZombiePreset>(Vector3(13.5f, 1.5f, 10.5f), RADIANS_180, TRUE);
	AddCharacter<RedZombiePreset>(Vector3(12.5f, 1.5f, 10.5f), RADIANS_180, TRUE);
	
	return TRUE;
}

void MainMenuLevel::OnStartGame()
{
	STACK_TRACE;
}

void MainMenuLevel::OnUpdate(float delta)
{
	STACK_TRACE;
}
