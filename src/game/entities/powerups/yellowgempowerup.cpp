#include "../../../framework/debug.h"

#include "yellowgempowerup.h"
#include "../components/billboardcomponent.h"
#include "../components/particlesonpowerupcollectioncomponent.h"
#include "../globalcomponents/contentcachecomponent.h"
#include "../presets/particles/sparkleparticlepreset.h"
#include "../../../contexts/contentcache.h"
#include "../../../entities/entity.h"
#include "../../../entities/entitymanager.h"

YellowGemPowerUp::YellowGemPowerUp(EntityManager *entityManager)
	: PowerUp(entityManager)
{
	STACK_TRACE;
}

YellowGemPowerUp::~YellowGemPowerUp()
{
	STACK_TRACE;
}

void YellowGemPowerUp::OnEntityCreation(Entity *powerUpEntity)
{
	STACK_TRACE;
	ASSERT(powerUpEntity != NULL);
	
	ContentCacheComponent *content = GetEntityManager()->GetGlobalComponent<ContentCacheComponent>();
	
	BillboardComponent *billboard = powerUpEntity->Add<BillboardComponent>();
	billboard->textureAtlas = content->cache->GetItems();
	billboard->textureAtlasTileIndex = 4;
	billboard->width = 0.5f;
	billboard->height = 0.5f;

	ParticlesOnPowerUpCollectionComponent *particlesOnCollection = powerUpEntity->Add<ParticlesOnPowerUpCollectionComponent>();
	particlesOnCollection->particleInfo.particlePreset = SparkleParticlePreset::GetType();
	particlesOnCollection->particleInfo.spawnInRadiusAroundPosition = TRUE;
	particlesOnCollection->particleInfo.minimumAmount = 5;
	particlesOnCollection->particleInfo.maximumAmount = 10;
	particlesOnCollection->particleInfo.lowColor = COLOR_YELLOW;
	particlesOnCollection->particleInfo.highColor = COLOR_YELLOW;
}

void YellowGemPowerUp::OnPickup(Entity *powerUpEntity, Entity *collector)
{
	STACK_TRACE;
	AwardScoreToEntity(collector, powerUpEntity, 1000);
}
