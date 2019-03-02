#include "../../../framework/debug.h"

#include "attackbuffpowerup.h"
#include "../buffs/attackbuff.h"
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

AttackBuffPowerUp::AttackBuffPowerUp(EntityManager *entityManager)
	: PowerUp(entityManager)
{
	STACK_TRACE;
}

AttackBuffPowerUp::~AttackBuffPowerUp()
{
	STACK_TRACE;
}

void AttackBuffPowerUp::OnEntityCreation(Entity *powerUpEntity)
{
	STACK_TRACE;
	ASSERT(powerUpEntity != NULL);
	
	ContentCacheComponent *content = GetEntityManager()->GetGlobalComponent<ContentCacheComponent>();
	BuffCollectionComponent *buffs = GetEntityManager()->GetGlobalComponent<BuffCollectionComponent>();
	
	BillboardComponent *billboard = powerUpEntity->Add<BillboardComponent>();
	billboard->textureAtlas = content->cache->GetItems();
	billboard->textureAtlasTileIndex = buffs->buffs->Get<AttackBuff>()->GetImageIndex();
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

void AttackBuffPowerUp::OnPickup(Entity *powerUpEntity, Entity *collector)
{
	STACK_TRACE;
	BuffAddedEvent *e = new BuffAddedEvent(collector);
	e->type = AttackBuff::GetType();
	GetEntityManager()->GetEventManager()->Queue(e);
}
