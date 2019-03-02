#include "../framework/debug.h"
#include "../framework/common.h"

#include "level.h"
#include "world.h"
#include "entities/components/boundingspherecomponent.h"
#include "entities/components/chestcomponent.h"
#include "entities/components/orientationxzcomponent.h"
#include "entities/components/positioncomponent.h"
#include "entities/events/spawnedevent.h"
#include "entities/events/spawnparticlesevent.h"
#include "entities/powerups/powerup.h"
#include "entities/powerups/powerupcollection.h"
#include "entities/presets/objects/chestpreset.h"
#include "entities/presets/particles/starparticlepreset.h"
#include "../gameapp.h"
#include "../entities/entity.h"
#include "../entities/entitymanager.h"
#include "../entities/entitypreset.h"
#include "../events/eventmanager.h"
#include "../framework/math/point3.h"
#include "../framework/math/vector3.h"

Level::Level(World *world)
{
	STACK_TRACE;
	ASSERT(world != NULL);
	m_world = world;
}

Level::~Level()
{
	STACK_TRACE;
}

Entity* Level::AddCharacter(ENTITYPRESET_TYPE presetName, const Vector3 &position, float yOrientation, BOOL suppressSpawnEvent)
{
	STACK_TRACE;
	Entity *character = GetWorld()->GetEntityManager()->AddUsingPreset(presetName);
	ASSERT(character != NULL);
	
	character->Get<PositionComponent>()->position = position;
	character->Get<OrientationXZComponent>()->angle = yOrientation;
	
	if (!suppressSpawnEvent)
	{
		SpawnedEvent spawnEvent(character);
		GetWorld()->GetGameApp()->GetEventManager()->Trigger(&spawnEvent);
	}
	
	return character;
}

Entity* Level::AddCharacterAtRandomLocation(ENTITYPRESET_TYPE presetName)
{
	STACK_TRACE;
	Entity *character = GetWorld()->GetEntityManager()->AddUsingPreset(presetName);
	ASSERT(character != NULL);

	Point3 position = GetWorld()->GetRandomWalkableSpace();

	float boundingRadius = character->Get<BoundingSphereComponent>()->bounds.radius;

	Vector3 finalPos;
	finalPos.x = (float)position.x + 0.5f;
	finalPos.y = (float)position.y + boundingRadius;
	finalPos.z = (float)position.z + 0.5f;
	
	// HACK: adding this extra bit to the y coord to see if it solves a problem
	//       where entities seem to be getting stuck? not sure if it's only
	//       happening when they spawn or after they've been moving around for
	//       a bit though... so this may not do anything at all!
	finalPos.y += 0.1f;

	int32_t orientationAngleIncrement = Rnd(0, 7);
	float finalOrientation = orientationAngleIncrement * RADIANS_45;

	character->Get<PositionComponent>()->position = finalPos;
	character->Get<OrientationXZComponent>()->angle = finalOrientation;

	SpawnedEvent spawnEvent(character);
	GetWorld()->GetGameApp()->GetEventManager()->Trigger(&spawnEvent);

	return character;
}

Entity* Level::AddChest(POWERUP_TYPE containedPowerUp, const Vector3 &position, float yOrientation, BOOL suppressSpawnEvent)
{
	STACK_TRACE;
	PowerUp *powerUp = GetWorld()->GetPowerUps()->Get(containedPowerUp);
	ASSERT(powerUp != NULL);
	Entity *chest = GetWorld()->GetEntityManager()->AddUsingPreset<ChestPreset>();
	ASSERT(chest != NULL);
	
	chest->Get<PositionComponent>()->position = position;
	chest->Get<OrientationXZComponent>()->angle = yOrientation;
	chest->Get<ChestComponent>()->powerUp = powerUp;
	
	if (!suppressSpawnEvent)
	{
		SpawnedEvent spawnEvent(chest);
		GetWorld()->GetGameApp()->GetEventManager()->Trigger(&spawnEvent);
	}

	return chest;
}

Entity* Level::AddChestAtRandomLocation(POWERUP_TYPE containedPowerUp)
{
	STACK_TRACE;
	PowerUp *powerUp = GetWorld()->GetPowerUps()->Get(containedPowerUp);
	ASSERT(powerUp != NULL);
	Entity *chest = GetWorld()->GetEntityManager()->AddUsingPreset<ChestPreset>();
	ASSERT(chest != NULL);
	
	Point3 position = GetWorld()->GetRandomWalkableSpace();
	
	float boundingRadius = chest->Get<BoundingSphereComponent>()->bounds.radius;
	
	Vector3 finalPos;
	finalPos.x = (float)position.x + 0.5f;
	finalPos.y = (float)position.y + boundingRadius;
	finalPos.z = (float)position.z + 0.5f;
	
	int32_t orientationAngleIncrement = Rnd(0, 7);
	float finalOrientation = orientationAngleIncrement * RADIANS_45;
	
	chest->Get<PositionComponent>()->position = finalPos;
	chest->Get<OrientationXZComponent>()->angle = finalOrientation;
	chest->Get<ChestComponent>()->powerUp = powerUp;
	
	SpawnedEvent spawnEvent(chest);
	GetWorld()->GetGameApp()->GetEventManager()->Trigger(&spawnEvent);

	return chest;	
}

void Level::AddStarsAt(const Vector3 &position, float spawnRadius)
{
	STACK_TRACE;
	SpawnParticlesEvent particlesEvent;
	particlesEvent.particleInfo.particlePreset = StarParticlePreset::GetType();
	particlesEvent.particleInfo.position = position;
	particlesEvent.particleInfo.spawnInRadiusAroundPosition = TRUE;
	particlesEvent.particleInfo.radius = spawnRadius;
	particlesEvent.particleInfo.minimumAmount = 50;
	particlesEvent.particleInfo.maximumAmount = 50;
	GetWorld()->GetGameApp()->GetEventManager()->Trigger(&particlesEvent);
}
