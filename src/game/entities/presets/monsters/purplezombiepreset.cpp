#include "../../../../framework/debug.h"

#include "purplezombiepreset.h"
#include "../../components/aiaggroattackercomponent.h"
#include "../../components/aifollowandattacktargetcomponent.h"
#include "../../components/airandommovementcomponent.h"
#include "../../components/aitargetablecomponent.h"
#include "../../components/aitargetcomponent.h"
#include "../../components/aitargetplayercomponent.h"
#include "../../components/aitimerscomponent.h"
#include "../../components/combatmodifierscomponent.h"
#include "../../components/keyframemeshcomponent.h"
#include "../../components/lifecomponent.h"
#include "../../components/physicscomponent.h"
#include "../../components/weaponcomponent.h"
#include "../../globalcomponents/contentmanagercomponent.h"
#include "../../globalcomponents/weaponcollectioncomponent.h"
#include "../../weapons/fistwithsmallknockbackweapon.h"
#include "../../weapons/weaponcollection.h"
#include "../../../../contexts/contentcache.h"
#include "../../../../entities/entity.h"
#include "../../../../entities/entitymanager.h"
#include "../../../../events/eventmanager.h"
#include "../../../../framework/content/contentmanager.h"
#include "../../../../framework/graphics/texture.h"

PurpleZombiePreset::PurpleZombiePreset(EntityManager *entityManager)
	: ZombiePreset(entityManager)
{
	STACK_TRACE;
}

PurpleZombiePreset::~PurpleZombiePreset()
{
	STACK_TRACE;
}

Entity* PurpleZombiePreset::Create(EntityPresetArgs *args)
{
	STACK_TRACE;
	Entity *entity = ZombiePreset::Create(args);
	
	ContentManagerComponent *content = GetEntityManager()->GetGlobalComponent<ContentManagerComponent>();
	ASSERT(content != NULL);
	WeaponCollectionComponent *weapons = GetEntityManager()->GetGlobalComponent<WeaponCollectionComponent>();
	ASSERT(weapons != NULL);
	
	KeyframeMeshComponent *keyframeMesh = entity->Get<KeyframeMeshComponent>();
	keyframeMesh->texture = content->content->Get<Texture>("assets://characters/zombie_4.png");
	
	PhysicsComponent *physics = entity->Get<PhysicsComponent>();
	physics->maxWalkSpeed = 4.0f;
	physics->walkingAcceleration = 3.0f;
	
	LifeComponent *life = entity->Get<LifeComponent>();
	life->health = 100;
	life->maxHealth = 100;
	
	CombatModifiersComponent *combatModifiers = entity->Add<CombatModifiersComponent>();
	combatModifiers->damageModifier = 2.0f;
	
	entity->Add<AITargetableComponent>();
	
	entity->Add<AITargetComponent>();
	
	AIAggroAttackerComponent *aggroAttacker = entity->Add<AIAggroAttackerComponent>();
	aggroAttacker->maximumAggroDistance = 10.0f;
	aggroAttacker->percentChanceToSuccessfullyAcquireTarget = 0.1f;
	
	entity->Add<AIFollowAndAttackTargetComponent>();

	AIRandomMovementComponent *randomMovement = entity->Add<AIRandomMovementComponent>();
	randomMovement->percentChanceToStartMoving = 0.75f;
	randomMovement->timeToMoveMin = 0.25f;
	randomMovement->timeToMoveMax = 2.0f;
	
	AITargetPlayerComponent *targetPlayer = entity->Add<AITargetPlayerComponent>();
	targetPlayer->maximumDistanceToFindTarget = 5.0f;
	targetPlayer->maximumDistanceToKeepTarget = 10.0f;
	targetPlayer->percentChanceToSuccessfullyAcquireTarget = 0.1f;
	
	AITimersComponent *aiTimers = entity->Add<AITimersComponent>();
	aiTimers->hasAfterAttackPauseTimer = TRUE;
	aiTimers->afterAttackPauseTimeMin = 0.5f;
	aiTimers->afterAttackPauseTimeMax = 0.5f;
	aiTimers->hasAfterHurtPauseTimer = TRUE;
	aiTimers->afterHurtPauseTimeMin = 0.25f;
	aiTimers->afterHurtPauseTimeMax = 0.5f;
	aiTimers->hasWaitBeforeAttackingTimer = TRUE;
	aiTimers->waitBeforeAttackingTimeMin = 0.15f;
	aiTimers->waitBeforeAttackingTimeMax = 0.5f;
	
	entity->Add<WeaponComponent>()->weapon = weapons->weapons->Get<FistWithSmallKnockbackWeapon>();
	
	return entity;
}

void PurpleZombiePreset::OnKilled(Entity *entity, Entity *killer)
{
	STACK_TRACE;
	ZombiePreset::OnKilled(entity, killer);

	if (killer != NULL && GetEntityManager()->IsValid(killer))
		AwardScoreToPlayer(entity, 100);
}