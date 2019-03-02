#include "../../../framework/debug.h"

#include "playerpreset.h"
#include "../components/affectedbygravitycomponent.h"
#include "../components/aitargetablecomponent.h"
#include "../components/animationcomponent.h"
#include "../components/autoanimationcomponent.h"
#include "../components/blobshadowcomponent.h"
#include "../components/boundingspherecomponent.h"
#include "../components/buffscomponent.h"
#include "../components/canbeattackedcomponent.h"
#include "../components/chestopenercomponent.h"
#include "../components/colorcomponent.h"
#include "../components/combatmodifierscomponent.h"
#include "../components/lifecomponent.h"
#include "../components/keyframemeshcomponent.h"
#include "../components/orientationxzcomponent.h"
#include "../components/physicscomponent.h"
#include "../components/positioncomponent.h"
#include "../components/powerupcollectorcomponent.h"
#include "../components/playercomponent.h"
#include "../components/pushablecomponent.h"
#include "../components/pushercomponent.h"
#include "../components/renderoffsetcomponent.h"
#include "../components/scorecomponent.h"
#include "../components/statecomponent.h"
#include "../components/weaponcomponent.h"
#include "../components/weaponlistcomponent.h"
#include "../globalcomponents/contentmanagercomponent.h"
#include "../globalcomponents/weaponcollectioncomponent.h"
#include "../weapons/weaponcollection.h"
#include "../weapons/fistweapon.h"
#include "../../physicsconstants.h"
#include "../../../contexts/contentcache.h"
#include "../../../entities/entity.h"
#include "../../../entities/entitymanager.h"
#include "../../../framework/content/contentmanager.h"
#include "../../../framework/assets/animation/keyframemesh.h"
#include "../../../framework/graphics/texture.h"

PlayerPreset::PlayerPreset(EntityManager *entityManager)
	: BaseCharacterEntityPreset(entityManager)
{
	STACK_TRACE;
}

PlayerPreset::~PlayerPreset()
{
	STACK_TRACE;
}

Entity* PlayerPreset::Create(EntityPresetArgs *args)
{
	STACK_TRACE;
	ContentManagerComponent *content = GetEntityManager()->GetGlobalComponent<ContentManagerComponent>();
	ASSERT(content != NULL);
	WeaponCollectionComponent *weapons = GetEntityManager()->GetGlobalComponent<WeaponCollectionComponent>();
	ASSERT(weapons != NULL);

	KeyframeMesh *mesh = content->content->Get<KeyframeMesh>("assets://characters/adventurer.mesh");

	Entity *entity = GetEntityManager()->Add();

	entity->Add<AffectedByGravityComponent>();

	entity->Add<AnimationComponent>()
		->AddSequence("idle", *mesh->GetAnimation("IDLE"), 0.05f)
		->AddSequence("walk", *mesh->GetAnimation("WALK"), 0.018f)
		->AddSequence("dead", *mesh->GetAnimation("KILLED"), 0.10f)
		->AddSequence("punch", *mesh->GetAnimation("THRUST_RIGHT"), 0.05f)
		->AddSequence("shoot", *mesh->GetAnimation("SLICE_RIGHT"), 0.02f)
		->SetSequence("idle");

	entity->Add<AutoAnimationComponent>()
		->Set(ENTITYSTATE_IDLE, "idle")
		->Set(ENTITYSTATE_WALKING, "walk")
		->Set(ENTITYSTATE_DEAD, "dead", FALSE, TRUE)
		->Set(ENTITYSTATE_PUNCHING, "punch", FALSE, TRUE)
		->Set(ENTITYSTATE_SHOOTING, "shoot", FALSE, TRUE);
	
	entity->Add<StateComponent>()->state = ENTITYSTATE_IDLE;

	entity->Add<BoundingSphereComponent>()
		->bounds.radius = 0.49f;

	LifeComponent *life = entity->Add<LifeComponent>();
	life->health = 1000;
	life->maxHealth = 1000;

	entity->Add<OrientationXZComponent>();

	PhysicsComponent *physics = entity->Add<PhysicsComponent>();
	physics->friction = FRICTION_NORMAL;
	physics->maxWalkSpeed = 10.0f;
	physics->walkingAcceleration = 6.0f;
	physics->SetBoundsRadius(0.49f);
	
	entity->Add<PushableComponent>();
	entity->Add<PusherComponent>();

	entity->Add<PositionComponent>();

	entity->Add<RenderOffsetComponent>()
		->offset = Vector3(0.0f, -0.5f, 0.0f);

	KeyframeMeshComponent *keyframeMesh = entity->Add<KeyframeMeshComponent>();
	keyframeMesh->mesh = mesh;
	keyframeMesh->texture = content->content->Get<Texture>("assets://characters/adventurer.png");

	entity->Add<BlobShadowComponent>();

	entity->Add<PlayerComponent>();

	WeaponListComponent *weaponList = entity->Add<WeaponListComponent>();
	weaponList->weapons.push_back(weapons->weapons->Get<FistWeapon>());

	WeaponComponent *weapon = entity->Add<WeaponComponent>();
	weapon->weapon = *(weaponList->weapons.begin());

	CanBeAttackedComponent *canBeAttacked = entity->Add<CanBeAttackedComponent>();
	canBeAttacked->byNPC = TRUE;
		
	entity->Add<AITargetableComponent>();

	entity->Add<ChestOpenerComponent>();
	
	entity->Add<PowerUpCollectorComponent>();
	
	entity->Add<ScoreComponent>();
	
	entity->Add<ColorComponent>()->color = COLOR_WHITE;
	
	entity->Add<BuffsComponent>();
	
	entity->Add<CombatModifiersComponent>();
	
	return entity;
}

void PlayerPreset::OnDespawn(Entity *entity)
{
	SpawnPuffOfSmokeAt(entity);
}
