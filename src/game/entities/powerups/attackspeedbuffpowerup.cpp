#include "../../../framework/debug.h"

#include "attackspeedbuffpowerup.h"
#include "../buffs/attackspeedbuff.h"
#include "../buffs/buffcollection.h"
#include "../components/billboardcomponent.h"
#include "../components/particlesonpowerupcollectioncomponent.h"
#include "../events/buffaddedevent.h"
#include "../globalcomponents/contentcachecomponent.h"
#include "../globalcomponents/buffcollectioncomponent.h"
#include "../presets/particles/sparkleparticlepreset.h"
#include "../../../contexts/contentcache.h"
#include "../../../entities/entity.h"
#include "../../../entities/entitymanager.h"
#include "../../../events/eventmanager.h"

AttackSpeedBuffPowerUp::AttackSpeedBuffPowerUp(EntityManager *entityManager)
	: PowerUp(entityManager)
{
	STACK_TRACE;
}

AttackSpeedBuffPowerUp::~AttackSpeedBuffPowerUp()
{
	STACK_TRACE;
}

void AttackSpeedBuffPowerUp::OnEntityCreation(Entity *powerUpEntity)
{
	STACK_TRACE;
	ASSERT(powerUpEntity != NULL);
	
	ContentCacheComponent *content = GetEntityManager()->GetGlobalComponent<ContentCacheComponent>();
	BuffCollectionComponent *buffs = GetEntityManager()->GetGlobalComponent<BuffCollectionComponent>();
	
	BillboardComponent *billboard = powerUpEntity->Add<BillboardComponent>();
	billboard->textureAtlas = content->cache->GetItems();
	billboard->textureAtlasTileIndex = buffs->buffs->Get<AttackSpeedBuff>()->GetImageIndex();
	billboard->width = 0.5f;
	billboard->height = 0.5f;
	
	ParticlesOnPowerUpCollectionComponent *particlesOnCollection = powerUpEntity->Add<ParticlesOnPowerUpCollectionComponent>();
	particlesOnCollection->particleInfo.particlePreset = SparkleParticlePreset::GetType();
	particlesOnCollection->particleInfo.spawnInRadiusAroundPosition = TRUE;
	particlesOnCollection->particleInfo.minimumAmount = 5;
	particlesOnCollection->particleInfo.maximumAmount = 10;
	particlesOnCollection->particleInfo.lowColor = COLOR_GREEN;
	particlesOnCollection->particleInfo.highColor = COLOR_GREEN;
}

void AttackSpeedBuffPowerUp::OnPickup(Entity *powerUpEntity, Entity *collector)
{
	STACK_TRACE;
	BuffAddedEvent *e = new BuffAddedEvent(collector);
	e->type = AttackSpeedBuff::GetType();
	GetEntityManager()->GetEventManager()->Queue(e);
}
