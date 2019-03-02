#include "../../../../framework/debug.h"

#include "heartparticlepreset.h"
#include "../../../../framework/common.h"
#include "../../components/billboardcomponent.h"
#include "../../components/particlecomponent.h"
#include "../../globalcomponents/contentcachecomponent.h"
#include "../../../../contexts/contentcache.h"
#include "../../../../entities/entity.h"
#include "../../../../entities/entitymanager.h"
#include "../../../../framework/content/contentmanager.h"
#include "../../../../framework/graphics/color.h"

HeartParticlePreset::HeartParticlePreset(EntityManager *entityManager)
: ParticlePreset(entityManager)
{
	STACK_TRACE;
}

HeartParticlePreset::~HeartParticlePreset()
{
	STACK_TRACE;
}

Entity* HeartParticlePreset::Create(EntityPresetArgs *args)
{
	STACK_TRACE;
	Entity *entity = ParticlePreset::Create(args);
	
	ContentCacheComponent *contentCache = GetEntityManager()->GetGlobalComponent<ContentCacheComponent>();
	ASSERT(contentCache != NULL);
	
	entity->Get<ParticleComponent>()->lifeTimeLeft = Rnd(0.3f, 0.6f);
	
	BillboardComponent *billboard = entity->Add<BillboardComponent>();
	billboard->textureAtlas = contentCache->cache->GetParticles();
	billboard->textureAtlasTileIndex = 27;
	billboard->width = 0.3f;
	billboard->height = 0.3f;
	
	return entity;
}
