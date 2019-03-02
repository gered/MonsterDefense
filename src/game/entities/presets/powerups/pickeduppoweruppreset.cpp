#include "../../../../framework/debug.h"

#include "pickeduppoweruppreset.h"
#include "poweruppresetproperties.h"
#include "../particles/sparkleparticlepreset.h"
#include "../../components/boundingspherecomponent.h"
#include "../../components/collectedpowerupcomponent.h"
#include "../../components/colorcomponent.h"
#include "../../components/particletrailcomponent.h"
#include "../../components/positioncomponent.h"
#include "../../components/positionoffsetcomponent.h"
#include "../../components/powerupcomponent.h"
#include "../../powerups/powerup.h"
#include "../../../physicsconstants.h"
#include "../../../../entities/entity.h"
#include "../../../../entities/entitymanager.h"

PickedUpPowerUpPreset::PickedUpPowerUpPreset(EntityManager *entityManager)
: EntityPreset(entityManager)
{
	STACK_TRACE;
}

PickedUpPowerUpPreset::~PickedUpPowerUpPreset()
{
	STACK_TRACE;
}

Entity* PickedUpPowerUpPreset::Create(EntityPresetArgs *args)
{
	STACK_TRACE;
	ASSERT(args != NULL);
	PowerUpPresetProperties *spawnArgs = args->As<PowerUpPresetProperties>();
	ASSERT(spawnArgs->powerUp != NULL);
	
	Entity *entity = GetEntityManager()->Add();

	entity->Add<PositionComponent>();
	entity->Add<ColorComponent>();
	entity->Add<PositionOffsetComponent>();

	entity->Add<BoundingSphereComponent>()
		->bounds.radius = 0.3f;

	CollectedPowerUpComponent *collected = entity->Add<CollectedPowerUpComponent>();
	collected->powerUp = spawnArgs->powerUp;
	collected->maxFloatY = 1.5f;
	collected->alpha = 1.0f;

	ParticleTrailComponent *particleTrail = entity->Add<ParticleTrailComponent>();
	particleTrail->particleInfo.particlePreset = SparkleParticlePreset::GetType();
	particleTrail->particleInfo.minimumAmount = 1;
	particleTrail->particleInfo.maximumAmount = 2;
	particleTrail->particleInfo.spawnInRadiusAroundPosition = TRUE;
	particleTrail->particleInfo.includePositionOffset = TRUE;
	particleTrail->particleInfo.lowColor = COLOR_YELLOW;
	particleTrail->particleInfo.highColor = COLOR_YELLOW;
	particleTrail->minTime = 0.05f;
	particleTrail->maxTime = 0.2f;

	// let the power up class do it's own specific creation stuff
	spawnArgs->powerUp->OnEntityCreation(entity);
	
	return entity;
}
