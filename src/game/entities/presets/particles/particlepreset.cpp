#include "../../../../framework/debug.h"

#include "particlepreset.h"
#include "../../../../framework/common.h"
#include "particlepresetproperties.h"
#include "../../components/particlecomponent.h"
#include "../../components/positioncomponent.h"
#include "../../../../entities/entity.h"
#include "../../../../entities/entitymanager.h"
#include "../../../../framework/math/mathhelpers.h"
#include "../../../../framework/math/vector3.h"

ParticlePreset::ParticlePreset(EntityManager *entityManager)
: EntityPreset(entityManager)
{
	STACK_TRACE;
}

ParticlePreset::~ParticlePreset()
{
	STACK_TRACE;
}

Entity* ParticlePreset::Create(EntityPresetArgs *args)
{
	STACK_TRACE;
	Entity *entity = GetEntityManager()->Add();
	entity->Add<ParticleComponent>();
	entity->Add<PositionComponent>();

	if (args != NULL)
	{
		ParticlePresetProperties *spawnArgs = args->As<ParticlePresetProperties>();
		if (spawnArgs != NULL)
			SetPositionFromArgs(entity, spawnArgs);
	}
	
	return entity;
}

void ParticlePreset::SetPositionFromArgs(Entity *entity, const ParticlePresetProperties *args)
{
	STACK_TRACE;
	PositionComponent *position = entity->Get<PositionComponent>();
	
	position->position = args->spawnPosition;
	
	if (args->canSpawnAroundPosition && args->canOnlySpawnAroundPositionOnXZ)
	{
		float x = Rnd(-args->radius, args->radius);
		float z = Rnd(-args->radius, args->radius);
		
		Vector3 offset(x, 0.0f, z);
		position->position += offset;
	}
	else if (args->canSpawnAroundPosition)
	{
		float x = Rnd(-args->radius, args->radius);
		float y = Rnd(-args->radius, args->radius);
		float z = Rnd(-args->radius, args->radius);
		
		Vector3 offset(x, y, z);
		position->position += offset;
	}	
}
