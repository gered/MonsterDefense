#include "../../../../framework/debug.h"

#include "chestpreset.h"
#include "../particles/starparticlepreset.h"
#include "../../components/animationcomponent.h"
#include "../../components/autoanimationcomponent.h"
#include "../../components/blobshadowcomponent.h"
#include "../../components/boundingspherecomponent.h"
#include "../../components/chestcomponent.h"
#include "../../components/colorcomponent.h"
#include "../../components/despawntimercomponent.h"
#include "../../components/keyframemeshcomponent.h"
#include "../../components/orientationxzcomponent.h"
#include "../../components/particlesonspawncomponent.h"
#include "../../components/physicscomponent.h"
#include "../../components/positioncomponent.h"
#include "../../components/renderoffsetcomponent.h"
#include "../../components/statecomponent.h"
#include "../../globalcomponents/contentmanagercomponent.h"
#include "../../../physicsconstants.h"
#include "../../../../contexts/contentcache.h"
#include "../../../../entities/entity.h"
#include "../../../../entities/entitymanager.h"
#include "../../../../framework/content/contentmanager.h"
#include "../../../../framework/assets/animation/keyframemesh.h"
#include "../../../../framework/graphics/texture.h"

ChestPreset::ChestPreset(EntityManager *entityManager)
	: EntityPreset(entityManager)
{
	STACK_TRACE;
}

ChestPreset::~ChestPreset()
{
	STACK_TRACE;
}

Entity* ChestPreset::Create(EntityPresetArgs *args)
{
	STACK_TRACE;
	ContentManagerComponent *content = GetEntityManager()->GetGlobalComponent<ContentManagerComponent>();
	ASSERT(content != NULL);

	KeyframeMesh *mesh = content->content->Get<KeyframeMesh>("assets://objects/chest.mesh");

	Entity *entity = GetEntityManager()->Add();

	entity->Add<AnimationComponent>()
		->AddSequence("idle", *mesh->GetAnimation("IDLE"))
		->AddSequence("open", *mesh->GetAnimation("OPEN"), 0.05f)
		->AddSequence("close", *mesh->GetAnimation("CLOSE"), 0.5f);
	
	entity->Add<AutoAnimationComponent>()
		->Set(ENTITYSTATE_IDLE, "idle")
		->Set(ENTITYSTATE_OPEN, "open", FALSE);
	
	entity->Add<StateComponent>()->state = ENTITYSTATE_IDLE;

	entity->Add<BoundingSphereComponent>()
		->bounds.radius = 0.49f;

	entity->Add<ChestComponent>();
	
	DespawnTimerComponent *despawnTimer = entity->Add<DespawnTimerComponent>();
	despawnTimer->timeLeft = 30.0f;
	despawnTimer->startFlickeringAtTimeLeft = 5.0f;
	
	ParticlesOnSpawnComponent *particlesOnSpawn = entity->Add<ParticlesOnSpawnComponent>();
	particlesOnSpawn->particleInfo.particlePreset = StarParticlePreset::GetType();
	particlesOnSpawn->particleInfo.spawnInRadiusAroundPosition = TRUE;
	particlesOnSpawn->particleInfo.minimumAmount = 50;
	particlesOnSpawn->particleInfo.maximumAmount = 50;

	float angle = Rnd(1, 4) * 90.0f;
	entity->Add<OrientationXZComponent>()->angle = DegreesToRadians(angle);

	PhysicsComponent *physics = entity->Add<PhysicsComponent>();
	physics->friction = FRICTION_NORMAL;
	physics->maxWalkSpeed = 0.0f;
	physics->walkingAcceleration = 0.0f;
	physics->SetBoundsRadius(0.49f);

	entity->Add<PositionComponent>();

	entity->Add<RenderOffsetComponent>()
		->offset = Vector3(0.0f, -0.5f, 0.0f);

	KeyframeMeshComponent *keyframeMesh = entity->Add<KeyframeMeshComponent>();
	keyframeMesh->mesh = mesh;
	keyframeMesh->texture = content->content->Get<Texture>("assets://objects/chest.png");

	entity->Add<BlobShadowComponent>();
	
	entity->Add<ColorComponent>();

	return entity;
}
