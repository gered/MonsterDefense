#include "../../../../framework/debug.h"

#include "bluezombiepreset.h"
#include "../../components/aifollowandattacktargetcomponent.h"
#include "../../components/airandommovementcomponent.h"
#include "../../components/aitargetcomponent.h"
#include "../../components/aitargetablecomponent.h"
#include "../../components/aitargetplayercomponent.h"
#include "../../components/aitimerscomponent.h"
#include "../../components/keyframemeshcomponent.h"
#include "../../components/lifecomponent.h"
#include "../../components/physicscomponent.h"
#include "../../components/weaponcomponent.h"
#include "../../globalcomponents/contentmanagercomponent.h"
#include "../../globalcomponents/weaponcollectioncomponent.h"
#include "../../weapons/weaponcollection.h"
#include "../../weapons/fistwithsmallknockbackweapon.h"
#include "../../../../contexts/contentcache.h"
#include "../../../../entities/entity.h"
#include "../../../../entities/entitymanager.h"
#include "../../../../events/eventmanager.h"
#include "../../../../framework/content/contentmanager.h"
#include "../../../../framework/graphics/texture.h"

BlueZombiePreset::BlueZombiePreset(EntityManager *entityManager)
	: ZombiePreset(entityManager)
{
	STACK_TRACE;
}

BlueZombiePreset::~BlueZombiePreset()
{
	STACK_TRACE;
}

Entity* BlueZombiePreset::Create(EntityPresetArgs *args)
{
	STACK_TRACE;
	Entity *entity = ZombiePreset::Create(args);
	
	ContentManagerComponent *content = GetEntityManager()->GetGlobalComponent<ContentManagerComponent>();
	ASSERT(content != NULL);
	WeaponCollectionComponent *weapons = GetEntityManager()->GetGlobalComponent<WeaponCollectionComponent>();
	ASSERT(weapons != NULL);
	
	KeyframeMeshComponent *keyframeMesh = entity->Get<KeyframeMeshComponent>();
	keyframeMesh->texture = content->content->Get<Texture>("assets://characters/zombie_1.png");
	
	PhysicsComponent *physics = entity->Get<PhysicsComponent>();
	physics->maxWalkSpeed = 4.0f;
	physics->walkingAcceleration = 3.0f;
	
	LifeComponent *life = entity->Get<LifeComponent>();
	life->health = 100;
	life->maxHealth = 100;
	
	entity->Add<AITargetableComponent>();
	
	entity->Add<AITargetComponent>();
	
	entity->Add<AIFollowAndAttackTargetComponent>();
	
	AIRandomMovementComponent *randomMovement = entity->Add<AIRandomMovementComponent>();
	randomMovement->percentChanceToStartMoving = 0.5f;
	randomMovement->timeToMoveMin = 0.25f;
	randomMovement->timeToMoveMax = 2.0f;
	
	AITargetPlayerComponent *targetPlayer = entity->Add<AITargetPlayerComponent>();
	targetPlayer->maximumDistanceToFindTarget = 5.0f;
	targetPlayer->maximumDistanceToKeepTarget = 5.0f;
	
	AITimersComponent *aiTimers = entity->Add<AITimersComponent>();
	aiTimers->hasAfterAttackPauseTimer = TRUE;
	aiTimers->afterAttackPauseTimeMin = 0.5f;
	aiTimers->afterAttackPauseTimeMax = 0.5f;
	aiTimers->hasAfterHurtPauseTimer = TRUE;
	aiTimers->afterHurtPauseTimeMin = 0.25f;
	aiTimers->afterHurtPauseTimeMax = 0.5f;
	aiTimers->hasWaitBeforeAttackingTimer = TRUE;
	aiTimers->waitBeforeAttackingTimeMin = 0.25f;
	aiTimers->waitBeforeAttackingTimeMax = 1.0f;
	
	entity->Add<WeaponComponent>()->weapon = weapons->weapons->Get<FistWithSmallKnockbackWeapon>();
	
	return entity;
}

void BlueZombiePreset::OnKilled(Entity *entity, Entity *killer)
{
	STACK_TRACE;
	ZombiePreset::OnKilled(entity, killer);

	if (killer != NULL && GetEntityManager()->IsValid(killer))
		AwardScoreToPlayer(entity, 200);
}
