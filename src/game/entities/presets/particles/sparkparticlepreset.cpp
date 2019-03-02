#include "../../../../framework/debug.h"

#include "sparkparticlepreset.h"
#include "../../../../framework/common.h"
#include "../../components/billboardcomponent.h"
#include "../../components/colorcomponent.h"
#include "../../components/particlecomponent.h"
#include "../../components/flatvelocityparticlecomponent.h"
#include "../../globalcomponents/contentcachecomponent.h"
#include "../../../../contexts/contentcache.h"
#include "../../../../entities/entity.h"
#include "../../../../entities/entitymanager.h"
#include "../../../../framework/content/contentmanager.h"
#include "../../../../framework/graphics/color.h"
#include "../../../../framework/math/mathhelpers.h"
#include "../../../../framework/math/vector3.h"

SparkParticlePreset::SparkParticlePreset(EntityManager *entityManager)
	: ParticlePreset(entityManager)
{
	STACK_TRACE;
}

SparkParticlePreset::~SparkParticlePreset()
{
	STACK_TRACE;
}

Entity* SparkParticlePreset::Create(EntityPresetArgs *args)
{
	STACK_TRACE;
	Entity *entity = ParticlePreset::Create(args);

	ContentCacheComponent *contentCache = GetEntityManager()->GetGlobalComponent<ContentCacheComponent>();
	ASSERT(contentCache != NULL);

	entity->Get<ParticleComponent>()->lifeTimeLeft = Rnd(0.1f, 0.5f);

	BillboardComponent *billboard = entity->Add<BillboardComponent>();
	billboard->textureAtlas = contentCache->cache->GetParticles();
	billboard->textureAtlasTileIndex = 7;
	billboard->width = 0.5f;
	billboard->height = 0.5f;

	entity->Add<ColorComponent>()->color = COLOR_WHITE;

	// random direction (360 degrees along any axis)
	Vector3 direction;
	direction.x = Rnd(-1.0f, 1.0f);
	direction.y = Rnd(-1.0f, 1.0f);
	direction.z = Rnd(-1.0f, 1.0f);
	direction = Vector3::Normalize(direction);

	entity->Add<FlatVelocityParticleComponent>()->velocity = direction * 2.0f;

	return entity;
}
