#include "../../../framework/debug.h"

#include "heartpowerup.h"
#include "../components/billboardcomponent.h"
#include "../components/particlesonpowerupcollectioncomponent.h"
#include "../globalcomponents/contentcachecomponent.h"
#include "../presets/particles/heartparticlepreset.h"
#include "../../../contexts/contentcache.h"
#include "../../../entities/entity.h"
#include "../../../entities/entitymanager.h"

HeartPowerUp::HeartPowerUp(EntityManager *entityManager)
	: PowerUp(entityManager)
{
	STACK_TRACE;
}

HeartPowerUp::~HeartPowerUp()
{
	STACK_TRACE;
}

void HeartPowerUp::OnEntityCreation(Entity *powerUpEntity)
{
	STACK_TRACE;
	ASSERT(powerUpEntity != NULL);
	
	ContentCacheComponent *content = GetEntityManager()->GetGlobalComponent<ContentCacheComponent>();
	
	BillboardComponent *billboard = powerUpEntity->Add<BillboardComponent>();
	billboard->textureAtlas = content->cache->GetItems();
	billboard->textureAtlasTileIndex = 19;
	billboard->width = 0.6f;
	billboard->height = 0.6f;

	ParticlesOnPowerUpCollectionComponent *particlesOnCollection = powerUpEntity->Add<ParticlesOnPowerUpCollectionComponent>();
	particlesOnCollection->particleInfo.particlePreset = HeartParticlePreset::GetType();
	particlesOnCollection->particleInfo.spawnInRadiusAroundPosition = TRUE;
	particlesOnCollection->particleInfo.minimumAmount = 5;
	particlesOnCollection->particleInfo.maximumAmount = 10;
}

void HeartPowerUp::OnPickup(Entity *powerUpEntity, Entity *collector)
{
	STACK_TRACE;
	HealEntity(collector, powerUpEntity, 0.25f);
}
