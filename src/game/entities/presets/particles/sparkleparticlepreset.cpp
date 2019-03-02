#include "../../../../framework/debug.h"

#include "sparkleparticlepreset.h"
#include "particlepresetproperties.h"
#include "../../../../framework/common.h"
#include "../../components/billboardcomponent.h"
#include "../../components/colorcomponent.h"
#include "../../components/particlecomponent.h"
#include "../../globalcomponents/contentcachecomponent.h"
#include "../../../../contexts/contentcache.h"
#include "../../../../entities/entity.h"
#include "../../../../entities/entitymanager.h"
#include "../../../../framework/content/contentmanager.h"
#include "../../../../framework/graphics/color.h"

SparkleParticlePreset::SparkleParticlePreset(EntityManager *entityManager)
: ParticlePreset(entityManager)
{
	STACK_TRACE;
}

SparkleParticlePreset::~SparkleParticlePreset()
{
	STACK_TRACE;
}

Entity* SparkleParticlePreset::Create(EntityPresetArgs *args)
{
	STACK_TRACE;
	Entity *entity = ParticlePreset::Create(args);
	
	ParticlePresetProperties *spawnArgs = NULL;
	if (args != NULL)
		spawnArgs = args->As<ParticlePresetProperties>();
	
	ContentCacheComponent *contentCache = GetEntityManager()->GetGlobalComponent<ContentCacheComponent>();
	ASSERT(contentCache != NULL);
	
	entity->Get<ParticleComponent>()->lifeTimeLeft = Rnd(0.1f, 0.5f);
	
	BillboardComponent *billboard = entity->Add<BillboardComponent>();
	billboard->textureAtlas = contentCache->cache->GetParticles();
	billboard->textureAtlasTileIndex = 24;
	billboard->width = 0.3f;
	billboard->height = 0.3f;
	
	Color color = COLOR_YELLOW;
	if (spawnArgs != NULL)
	{
		color.r = Rnd(spawnArgs->lowColor.r, spawnArgs->highColor.r);
		color.g = Rnd(spawnArgs->lowColor.g, spawnArgs->highColor.g);
		color.b = Rnd(spawnArgs->lowColor.b, spawnArgs->highColor.b);
		color.a = Rnd(spawnArgs->lowColor.a, spawnArgs->highColor.a);
	}
	
	entity->Add<ColorComponent>()->color = color;
	
	return entity;
}
