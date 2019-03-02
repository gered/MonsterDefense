#include "../../../../framework/debug.h"

#include "greenzombiepreset.h"
#include "../../components/aiaggroattackercomponent.h"
#include "../../components/aifollowandattacktargetcomponent.h"
#include "../../components/airandommovementcomponent.h"
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
#include "../../weapons/fistweapon.h"
#include "../../../../contexts/contentcache.h"
#include "../../../../entities/entity.h"
#include "../../../../entities/entitymanager.h"
#include "../../../../events/eventmanager.h"
#include "../../../../framework/content/contentmanager.h"
#include "../../../../framework/graphics/texture.h"

GreenZombiePreset::GreenZombiePreset(EntityManager *entityManager)
	: ZombiePreset(entityManager)
{
	STACK_TRACE;
}

GreenZombiePreset::~GreenZombiePreset()
{
	STACK_TRACE;
}

Entity* GreenZombiePreset::Create(EntityPresetArgs *args)
{
	STACK_TRACE;
	Entity *entity = ZombiePreset::Create(args);
	
	ContentManagerComponent *content = GetEntityManager()->GetGlobalComponent<ContentManagerComponent>();
	ASSERT(content != NULL);
	WeaponCollectionComponent *weapons = GetEntityManager()->GetGlobalComponent<WeaponCollectionComponent>();
	ASSERT(weapons != NULL);
	
	KeyframeMeshComponent *keyframeMesh = entity->Get<KeyframeMeshComponent>();
	keyframeMesh->texture = content->content->Get<Texture>("assets://characters/zombie_2.png");
	
	PhysicsComponent *physics = entity->Get<PhysicsComponent>();
	physics->maxWalkSpeed = 3.0f;
	physics->walkingAcceleration = 2.0f;
	
	LifeComponent *life = entity->Get<LifeComponent>();
	life->health = 250;
	life->maxHealth = 250;
	
	CombatModifiersComponent *combatModifiers = entity->Add<CombatModifiersComponent>();
	combatModifiers->damageModifier = 3.0f;
	
	entity->Add<AITargetableComponent>();
	
	entity->Add<AITargetComponent>();
	
	entity->Add<AIAggroAttackerComponent>()->maximumAggroDistance = 10.0f;
	
	entity->Add<AIFollowAndAttackTargetComponent>();
	
	AIRandomMovementComponent *randomMovement = entity->Add<AIRandomMovementComponent>();
	randomMovement->percentChanceToStartMoving = 0.5f;
	randomMovement->timeToMoveMin = 0.25f;
	randomMovement->timeToMoveMax = 2.0f;
	
	AITargetPlayerComponent *targetPlayer = entity->Add<AITargetPlayerComponent>();
	targetPlayer->maximumDistanceToFindTarget = 2.5f;
	targetPlayer->maximumDistanceToKeepTarget = 10.0f;
	
	AITimersComponent *aiTimers = entity->Add<AITimersComponent>();
	aiTimers->hasAfterAttackPauseTimer = TRUE;
	aiTimers->afterAttackPauseTimeMin = 0.5f;
	aiTimers->afterAttackPauseTimeMax = 0.5f;
	aiTimers->hasAfterHurtPauseTimer = TRUE;
	aiTimers->afterHurtPauseTimeMin = 0.15f;
	aiTimers->afterHurtPauseTimeMax = 0.5f;
	aiTimers->hasWaitBeforeAttackingTimer = TRUE;
	aiTimers->waitBeforeAttackingTimeMin = 0.15f;
	aiTimers->waitBeforeAttackingTimeMax = 0.5f;
	
	entity->Add<WeaponComponent>()->weapon = weapons->weapons->Get<FistWeapon>();
	
	return entity;
}

void GreenZombiePreset::OnKilled(Entity *entity, Entity *killer)
{
	STACK_TRACE;
	ZombiePreset::OnKilled(entity, killer);

	if (killer != NULL && GetEntityManager()->IsValid(killer))
		AwardScoreToPlayer(entity, 500);
}
