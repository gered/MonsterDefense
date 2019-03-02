#include "../../../../framework/debug.h"

#include "textparticlepreset.h"
#include "textparticlepresetproperties.h"
#include "../../../../framework/common.h"
#include "../../components/colorcomponent.h"
#include "../../components/particlecomponent.h"
#include "../../components/positioncomponent.h"
#include "../../components/textcomponent.h"
#include "../../globalcomponents/contentcachecomponent.h"
#include "../../../../contexts/contentcache.h"
#include "../../../../entities/entity.h"
#include "../../../../entities/entitymanager.h"
#include "../../../../framework/content/contentmanager.h"
#include <string.h>

TextParticlePreset::TextParticlePreset(EntityManager *entityManager)
	: ParticlePreset(entityManager)
{
	STACK_TRACE;
}

TextParticlePreset::~TextParticlePreset()
{
	STACK_TRACE;
}

Entity* TextParticlePreset::Create(EntityPresetArgs *args)
{
	STACK_TRACE;
	Entity *entity = ParticlePreset::Create(args);

	TextParticlePresetProperties *spawnArgs = NULL;
	if (args != NULL)
		spawnArgs = args->As<TextParticlePresetProperties>();
		
	ContentCacheComponent *contentCache = GetEntityManager()->GetGlobalComponent<ContentCacheComponent>();
	ASSERT(contentCache != NULL);
	
	TextComponent *text = entity->Add<TextComponent>();
	ColorComponent *color = entity->Add<ColorComponent>();
	
	if (spawnArgs != NULL)
	{
		ParticleComponent *particle = entity->Get<ParticleComponent>();
		PositionComponent *position = entity->Get<PositionComponent>();
		
		text->font = contentCache->cache->GetSmallFont();
		text->useBillboardRendering = spawnArgs->useBillboardRendering;
		text->billboardSizePerPixel = spawnArgs->billboardSizePerPixel;
		strncpy(text->text, spawnArgs->text, TEXTCOMPONENT_MAX_LENGTH);
		
		position->position = spawnArgs->position;
		
		particle->lifeTimeLeft = spawnArgs->lifeTime;
		
		color->color = spawnArgs->color;
	}
	
	return entity;
}
