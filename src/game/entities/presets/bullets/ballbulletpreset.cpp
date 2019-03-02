#include "../../../../framework/debug.h"

#include "ballbulletpreset.h"
#include "../particles/particlepresetproperties.h"
#include "../particles/smokeparticlepreset.h"
#include "../particles/sparkparticlepreset.h"
#include "../../components/billboardcomponent.h"
#include "../../components/bulletcomponent.h"
#include "../../components/orientationxzcomponent.h"
#include "../../components/particletrailcomponent.h"
#include "../../components/physicscomponent.h"
#include "../../components/positioncomponent.h"
#include "../../forces/force.h"
#include "../../globalcomponents/contentcachecomponent.h"
#include "../../../../contexts/contentcache.h"
#include "../../../../entities/entity.h"
#include "../../../../entities/entitymanager.h"
#include "../../../../framework/math/mathhelpers.h"
#include "../../../../framework/math/vector3.h"

const float KNOCKBACK_FORCE_STRENGTH = 10.0f;
const float KNOCKBACK_FORCE_FRICTION = 0.8f;

BallBulletPreset::BallBulletPreset(EntityManager *entityManager)
: BulletPreset(entityManager)
{
	STACK_TRACE;
}

BallBulletPreset::~BallBulletPreset()
{
	STACK_TRACE;
}

Entity* BallBulletPreset::Create(EntityPresetArgs *args)
{
	STACK_TRACE;
	Entity *entity = BulletPreset::Create(args);
	
	ContentCacheComponent *contentCache = GetEntityManager()->GetGlobalComponent<ContentCacheComponent>();
	ASSERT(contentCache != NULL);
	
	BillboardComponent *billboard = entity->Add<BillboardComponent>();
	billboard->textureAtlas = contentCache->cache->GetParticles();
	billboard->textureAtlasTileIndex = 26;
	billboard->width = 0.3f;
	billboard->height = 0.3f;
	
	BulletComponent *bullet = entity->Get<BulletComponent>();
	bullet->flatDamageAmount = 50;
	
	ParticleTrailComponent *particleTrail = entity->Add<ParticleTrailComponent>();
	particleTrail->particleInfo.particlePreset = SmokeParticlePreset::GetType();
	particleTrail->particleInfo.minimumAmount = 1;
	particleTrail->particleInfo.maximumAmount = 1;
	particleTrail->particleInfo.spawnInRadiusAroundPosition = TRUE;
	particleTrail->particleInfo.lowColor = Color(0.3f, 0.3f, 0.3f, 0.5f);
	particleTrail->particleInfo.highColor = Color(0.3f, 0.3f, 0.3f, 0.5f);
	particleTrail->minTime = 0.05f;
	particleTrail->maxTime = 0.2f;
	
	return entity;
}

void BallBulletPreset::OnCollision(Entity *entity, Entity *victim)
{
	STACK_TRACE;
	PositionComponent *bulletPosition = entity->Get<PositionComponent>();
	
	// spawn spark particles where the bullet collided
	ParticlePresetProperties particleSpawnArgs;
	particleSpawnArgs.spawnPosition = bulletPosition->position;
	
	uint32_t numSparks = Rnd(5, 10);
	for (uint32_t i = 0; i < numSparks; ++i)
		GetEntityManager()->AddUsingPreset<SparkParticlePreset>(&particleSpawnArgs);

	if (victim != NULL)
	{
		// add knockback to the victim
		PhysicsComponent *victimPhysics = victim->Get<PhysicsComponent>();
		if (victimPhysics != NULL)
		{
			Vector3 knockbackDirection = GetDirectionFromYAxisOrientation(entity->Get<OrientationXZComponent>()->angle);
			
			Force *knockbackForce = new Force(knockbackDirection, KNOCKBACK_FORCE_STRENGTH, KNOCKBACK_FORCE_FRICTION);
			victimPhysics->forces.push_back(knockbackForce);
		}
	}
}
