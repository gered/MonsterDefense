#include "../../../../framework/debug.h"

#include "starparticlepreset.h"
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

StarParticlePreset::StarParticlePreset(EntityManager *entityManager)
	: ParticlePreset(entityManager)
{
	STACK_TRACE;
}

StarParticlePreset::~StarParticlePreset()
{
	STACK_TRACE;
}

Entity* StarParticlePreset::Create(EntityPresetArgs *args)
{
	STACK_TRACE;
	Entity *entity = ParticlePreset::Create(args);

	ContentCacheComponent *contentCache = GetEntityManager()->GetGlobalComponent<ContentCacheComponent>();
	ASSERT(contentCache != NULL);

	entity->Get<ParticleComponent>()->lifeTimeLeft = Rnd(0.4f, 0.7f);
	
	BillboardComponent *billboard = entity->Add<BillboardComponent>();
	billboard->textureAtlas = contentCache->cache->GetParticles();
	billboard->textureAtlasTileIndex = 24;
	billboard->width = 0.3f;
	billboard->height = 0.3f;

	// random color
	Color color;
	color.r = Rnd(0.5f, 1.0f);
	color.g = Rnd(0.5f, 1.0f);
	color.b = Rnd(0.5f, 1.0f);
	color.a = 1.0f;
	entity->Add<ColorComponent>()->color = color;

	// randon direction (360 degrees along any axis)
	Vector3 direction;
	direction.x = Rnd(-1.0f, 1.0f);
	direction.y = Rnd(-1.0f, 1.0f);
	direction.z = Rnd(-1.0f, 1.0f);
	direction = Vector3::Normalize(direction);

	entity->Add<FlatVelocityParticleComponent>()->velocity = direction * 2.0f;

	return entity;
}
