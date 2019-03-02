#include "../../../../framework/debug.h"

#include "smokeparticlepreset.h"
#include "../../../../framework/common.h"
#include "particlepresetproperties.h"
#include "../../components/animationcomponent.h"
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

SmokeParticlePreset::SmokeParticlePreset(EntityManager *entityManager)
	: ParticlePreset(entityManager)
{
	STACK_TRACE;
}

SmokeParticlePreset::~SmokeParticlePreset()
{
	STACK_TRACE;
}

Entity* SmokeParticlePreset::Create(EntityPresetArgs *args)
{
	STACK_TRACE;
	Entity *entity = ParticlePreset::Create(args);
	
	ParticlePresetProperties *spawnArgs = NULL;
	if (args != NULL)
		spawnArgs = args->As<ParticlePresetProperties>();

	ContentCacheComponent *contentCache = GetEntityManager()->GetGlobalComponent<ContentCacheComponent>();
	ASSERT(contentCache != NULL);

	float frameDelay = Rnd(0.1f, 0.2f);

	ParticleComponent *particle = entity->Get<ParticleComponent>();
	particle->lastsUntilAnimationFinishes = TRUE;
	particle->lifeTimeLeft = frameDelay;

	AnimationComponent *animation = entity->Add<AnimationComponent>();
	animation->currentSequence = AnimationSequence(0, 7, frameDelay);

	BillboardComponent *billboard = entity->Add<BillboardComponent>();
	billboard->textureAtlas = contentCache->cache->GetParticles();
	billboard->textureAtlasTileIndex = 0;
	billboard->width = 0.5f;
	billboard->height = 0.5f;

	Color color = COLOR_WHITE;
	if (spawnArgs != NULL)
	{
		color.r = Rnd(spawnArgs->lowColor.r, spawnArgs->highColor.r);
		color.g = Rnd(spawnArgs->lowColor.g, spawnArgs->highColor.g);
		color.b = Rnd(spawnArgs->lowColor.b, spawnArgs->highColor.b);
		color.a = Rnd(spawnArgs->lowColor.a, spawnArgs->highColor.a);
	}

	entity->Add<ColorComponent>()->color = color;

	Vector3 direction = GetDirectionFromAngles(DegreesToRadians(Rnd(80.0f, 100.0f)), DegreesToRadians(Rnd(-10.0f, 10.0f)));
	direction = Vector3::Normalize(direction);

	entity->Add<FlatVelocityParticleComponent>()->velocity = direction;

	return entity;
}
