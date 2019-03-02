#include "../../../../framework/debug.h"

#include "redzombiepreset.h"
#include "../../components/aifollowandattacktargetcomponent.h"
#include "../../components/airandommovementcomponent.h"
#include "../../components/airunawayafterattackingcomponent.h"
#include "../../components/aitargetcomponent.h"
#include "../../components/aitargetablecomponent.h"
#include "../../components/aitargetplayercomponent.h"
#include "../../components/aitimerscomponent.h"
#include "../../components/combatmodifierscomponent.h"
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

RedZombiePreset::RedZombiePreset(EntityManager *entityManager)
	: ZombiePreset(entityManager)
{
	STACK_TRACE;
}

RedZombiePreset::~RedZombiePreset()
{
	STACK_TRACE;
}

Entity* RedZombiePreset::Create(EntityPresetArgs *args)
{
	STACK_TRACE;
	Entity *entity = ZombiePreset::Create(args);
	
	ContentManagerComponent *content = GetEntityManager()->GetGlobalComponent<ContentManagerComponent>();
	ASSERT(content != NULL);
	WeaponCollectionComponent *weapons = GetEntityManager()->GetGlobalComponent<WeaponCollectionComponent>();
	ASSERT(weapons != NULL);
	
	KeyframeMeshComponent *keyframeMesh = entity->Get<KeyframeMeshComponent>();
	keyframeMesh->texture = content->content->Get<Texture>("assets://characters/zombie_3.png");

	PhysicsComponent *physics = entity->Get<PhysicsComponent>();
	physics->maxWalkSpeed = 8.0f;
	physics->walkingAcceleration = 6.0f;
	
	LifeComponent *life = entity->Get<LifeComponent>();
	life->health = 50;
	life->maxHealth = 50;

	CombatModifiersComponent *combatModifiers = entity->Add<CombatModifiersComponent>();
	combatModifiers->damageModifier = 0.5f;

	entity->Add<AITargetableComponent>();
	
	entity->Add<AITargetComponent>();
	
	entity->Add<AIFollowAndAttackTargetComponent>();
	
	AIRandomMovementComponent *randomMovement = entity->Add<AIRandomMovementComponent>();
	randomMovement->percentChanceToStartMoving = 0.5f;
	randomMovement->timeToMoveMin = 0.25f;
	randomMovement->timeToMoveMax = 1.0f;
	
	AITargetPlayerComponent *targetPlayer = entity->Add<AITargetPlayerComponent>();
	targetPlayer->maximumDistanceToFindTarget = 9.0f;
	targetPlayer->maximumDistanceToKeepTarget = 9.0f;
	
	AIRunAwayAfterAttackingComponent *runAwayAfterAttacking = entity->Add<AIRunAwayAfterAttackingComponent>();
	runAwayAfterAttacking->waitBeforeRunningTimeMin = 0.2f;
	runAwayAfterAttacking->waitBeforeRunningTimeMax = 0.2f;
	runAwayAfterAttacking->runAwayTimeMin = 0.75f;
	runAwayAfterAttacking->runAwayTimeMax = 1.5f;
	runAwayAfterAttacking->percentChanceToRunAway = 0.75f;
	runAwayAfterAttacking->percentChanceToKeepRunning = 0.5f;
	
	AITimersComponent *aiTimers = entity->Add<AITimersComponent>();
	aiTimers->hasAfterHurtPauseTimer = TRUE;
	aiTimers->afterHurtPauseTimeMin = 0.25f;
	aiTimers->afterHurtPauseTimeMax = 0.5f;
	
	entity->Add<WeaponComponent>()->weapon = weapons->weapons->Get<FistWithSmallKnockbackWeapon>();
	
	return entity;
}

void RedZombiePreset::OnKilled(Entity *entity, Entity *killer)
{
	STACK_TRACE;
	ZombiePreset::OnKilled(entity, killer);

	if (killer != NULL && GetEntityManager()->IsValid(killer))
		AwardScoreToPlayer(entity, 75);
}
