#include "../../../../framework/debug.h"

#include "scatterbulletpreset.h"
#include "../particles/particlepresetproperties.h"
#include "../particles/sparkparticlepreset.h"
#include "../particles/sparkleparticlepreset.h"
#include "../../components/billboardcomponent.h"
#include "../../components/bulletcomponent.h"
#include "../../components/orientationxzcomponent.h"
#include "../../components/particletrailcomponent.h"
#include "../../components/physicscomponent.h"
#include "../../components/positioncomponent.h"
#include "../../globalcomponents/contentcachecomponent.h"
#include "../../../../contexts/contentcache.h"
#include "../../../../entities/entity.h"
#include "../../../../entities/entitymanager.h"
#include "../../../../framework/math/mathhelpers.h"
#include "../../../../framework/math/vector3.h"

ScatterBulletPreset::ScatterBulletPreset(EntityManager *entityManager)
: BulletPreset(entityManager)
{
	STACK_TRACE;
}

ScatterBulletPreset::~ScatterBulletPreset()
{
	STACK_TRACE;
}

Entity* ScatterBulletPreset::Create(EntityPresetArgs *args)
{
	STACK_TRACE;
	Entity *entity = BulletPreset::Create(args);
	
	ContentCacheComponent *contentCache = GetEntityManager()->GetGlobalComponent<ContentCacheComponent>();
	ASSERT(contentCache != NULL);
	
	BillboardComponent *billboard = entity->Add<BillboardComponent>();
	billboard->textureAtlas = contentCache->cache->GetParticles();
	billboard->textureAtlasTileIndex = 25;
	billboard->width = 0.2f;
	billboard->height = 0.2f;
	
	BulletComponent *bullet = entity->Get<BulletComponent>();
	bullet->flatDamageAmount = 25;
	
	ParticleTrailComponent *particleTrail = entity->Add<ParticleTrailComponent>();
	particleTrail->particleInfo.particlePreset = SparkleParticlePreset::GetType();
	particleTrail->particleInfo.minimumAmount = 1;
	particleTrail->particleInfo.maximumAmount = 1;
	particleTrail->particleInfo.spawnInRadiusAroundPosition = TRUE;
	particleTrail->particleInfo.lowColor = Color(0.7f, 1.0f, 1.0f);
	particleTrail->particleInfo.highColor = Color(0.7f, 1.0f, 1.0f);
	particleTrail->minTime = 0.2f;
	particleTrail->maxTime = 0.3f;
	
	return entity;
}

void ScatterBulletPreset::OnCollision(Entity *entity, Entity *victim)
{
	STACK_TRACE;
	PositionComponent *bulletPosition = entity->Get<PositionComponent>();
	
	// spawn spark particles where the bullet collided
	ParticlePresetProperties particleSpawnArgs;
	particleSpawnArgs.spawnPosition = bulletPosition->position;
	
	uint32_t numSparks = Rnd(10, 20);
	for (uint32_t i = 0; i < numSparks; ++i)
		GetEntityManager()->AddUsingPreset<SparkParticlePreset>(&particleSpawnArgs);
}
