#include "../../../framework/debug.h"
#include "../../../framework/common.h"

#include "npcsystem.h"
#include "../components/aiaggroattackercomponent.h"
#include "../components/aifollowandattacktargetcomponent.h"
#include "../components/airandommovementcomponent.h"
#include "../components/airunawayafterattackingcomponent.h"
#include "../components/aitargetcomponent.h"
#include "../components/aitargetablecomponent.h"
#include "../components/aitargetnearestcomponent.h"
#include "../components/aitargetplayercomponent.h"
#include "../components/aitimerscomponent.h"
#include "../components/attackcooldowncomponent.h"
#include "../components/boundingspherecomponent.h"
#include "../components/deadcomponent.h"
#include "../components/npccomponent.h"
#include "../components/orientationxzcomponent.h"
#include "../components/playercomponent.h"
#include "../components/positioncomponent.h"
#include "../events/attackevent.h"
#include "../events/hurtevent.h"
#include "../events/killedevent.h"
#include "../events/moveforwardevent.h"
#include "../../../entities/entity.h"
#include "../../../entities/entitymanager.h"
#include "../../../events/eventmanager.h"
#include "../../../framework/math/boundingsphere.h"
#include "../../../framework/math/common.h"
#include "../../../framework/math/mathhelpers.h"
#include "../../../framework/math/vector3.h"

const float TOO_CLOSE_TOLERANCE = 0.1f;
const float TIME_BETWEEN_TARGET_ACQUIRES = 1.0f;
const float TIME_BETWEEN_MOVEMENT_ATTEMPTS = 0.5f;

NPCSystem::NPCSystem(EntityManager *entityManager, EventManager *eventManager)
	: ComponentSystem(entityManager, eventManager)
{
	STACK_TRACE;
	ListenFor<AttackEvent>();
	ListenFor<HurtEvent>();
	ListenFor<KilledEvent>();
}

NPCSystem::~NPCSystem()
{
	STACK_TRACE;
	StopListeningFor<AttackEvent>();
	StopListeningFor<HurtEvent>();
	StopListeningFor<KilledEvent>();
}

void NPCSystem::OnUpdate(float delta)
{
	STACK_TRACE;
	EntityList list;
	GetEntityManager()->GetAllWith<NPCComponent>(list);
	for (EntityList::iterator i = list.begin(); i != list.end(); ++i)
	{
		Entity *entity = *i;
		
		AITimersComponent *timers = entity->Get<AITimersComponent>();
		if (timers != NULL)
		{
			if (Process(timers, entity, delta))
				continue;
		}
		
		AIRunAwayAfterAttackingComponent *runAwayAfterAttacking = entity->Get<AIRunAwayAfterAttackingComponent>();
		if (runAwayAfterAttacking != NULL)
		{
			if (Process(runAwayAfterAttacking, entity, delta))
				continue;
		}
				
		AIFollowAndAttackTargetComponent *followAndAttackTarget = entity->Get<AIFollowAndAttackTargetComponent>();
		if (followAndAttackTarget != NULL)
		{
			if (Process(followAndAttackTarget, timers, entity, delta))
				continue;
		}

		AIRandomMovementComponent *randomMovement = entity->Get<AIRandomMovementComponent>();
		if (randomMovement != NULL)
		{
			if (Process(randomMovement, entity, delta))
				continue;
		}
	}
}

BOOL NPCSystem::Process(AITimersComponent *timers, Entity *entity, float delta)
{
	STACK_TRACE;
	BOOL isCurrentlyPausedByATimer = FALSE;
	
	if (timers->hasAfterAttackPauseTimer && timers->afterAttackPauseTimeLeft > 0.0f)
	{
		timers->afterAttackPauseTimeLeft -= delta;
		if (timers->afterAttackPauseTimeLeft < 0.0f)
			timers->afterAttackPauseTimeLeft = 0.0f;
		else
			isCurrentlyPausedByATimer = TRUE;
	}
	
	if (timers->hasAfterHurtPauseTimer && timers->afterHurtPauseTimeLeft > 0.0f)
	{
		timers->afterHurtPauseTimeLeft -= delta;
		if (timers->afterHurtPauseTimeLeft < 0.0f)
			timers->afterHurtPauseTimeLeft = 0.0f;
		else
			isCurrentlyPausedByATimer = TRUE;
	}

	if (timers->hasWaitBeforeAttackingTimer && timers->waitBeforeAttackingTimeLeft > 0.0f)
	{
		// this timer doesn't result in movement being paused. only attacks
		timers->waitBeforeAttackingTimeLeft -= delta;
		if (timers->waitBeforeAttackingTimeLeft < 0.0f)
			timers->waitBeforeAttackingTimeLeft = 0.0f;
	}
	
	return isCurrentlyPausedByATimer;
}

BOOL NPCSystem::Process(AIRandomMovementComponent *ai, Entity *entity, float delta)
{
	STACK_TRACE;	
	if (ai->timeToMoveLeft > 0.0f)
	{
		MoveForwardEvent *moveEvent = new MoveForwardEvent(entity);
		GetEventManager()->Queue(moveEvent);

		ai->timeToMoveLeft -= delta;
		if (ai->timeToMoveLeft <= 0.0f)
			ai->timeToMoveLeft = 0.0f;
	}
	else
	{
		if (ai->timeBeforeNextMovementAttempt > 0.0f)
		{
			// countdown the timer and return the existing target for now
			ai->timeBeforeNextMovementAttempt -= delta;
			if (ai->timeBeforeNextMovementAttempt < 0.0f)
				ai->timeBeforeNextMovementAttempt = 0.0f;
		}
		else
		{
			float chanceToMoveRoll = Rnd(0.0f, 1.0f);
			if (chanceToMoveRoll < ai->percentChanceToStartMoving)
			{
				ai->timeToMoveLeft = Rnd(ai->timeToMoveMin, ai->timeToMoveMax);
				entity->Get<OrientationXZComponent>()->angle = Rnd(1, 36) * DegreesToRadians(10.0f);

				MoveForwardEvent *moveEvent = new MoveForwardEvent(entity);
				GetEventManager()->Queue(moveEvent);
			}
			else
				ai->timeBeforeNextMovementAttempt = TIME_BETWEEN_MOVEMENT_ATTEMPTS;
		}
	}
	
	return TRUE;
}

BOOL NPCSystem::Process(AIRunAwayAfterAttackingComponent *ai, Entity *entity, float delta)
{
	STACK_TRACE;
	if (ai->waitBeforeRunningTimeLeft > 0.0f)
	{
		ai->waitBeforeRunningTimeLeft -= delta;
		if (ai->waitBeforeRunningTimeLeft <= 0.0f)
			ai->waitBeforeRunningTimeLeft = 0.0f;
		
		return TRUE;
	}
	
	else if (ai->runAwayTimeLeft > 0.0f)
	{
		entity->Get<OrientationXZComponent>()->angle = ai->runAwayAngle;
		
		MoveForwardEvent *moveEvent = new MoveForwardEvent(entity);
		GetEventManager()->Queue(moveEvent);
		
		ai->runAwayTimeLeft -= delta;
		if (ai->runAwayTimeLeft <= 0.0f)
		{
			ai->runAwayTimeLeft = 0.0f;
			
			// done running for this time period, now roll for a chance to run
			// again in a new direction
			float chanceToRunRoll = Rnd(0.0f, 1.0f);
			if (chanceToRunRoll < ai->percentChanceToKeepRunning)
			{
				ai->waitBeforeRunningTimeLeft = Rnd(ai->waitBeforeRunningTimeMin, ai->waitBeforeRunningTimeMax);
				ai->runAwayTimeLeft = Rnd(ai->runAwayTimeMin, ai->runAwayTimeMax);
				ai->runAwayAngle = Rnd(1, 36) * DegreesToRadians(10.0f);
				
				// start running in the next tick
			}
		}
		
		return TRUE;
	}
	
	return FALSE;
}

BOOL NPCSystem::Process(AIFollowAndAttackTargetComponent *ai, AITimersComponent *timers, Entity *entity, float delta)
{
	STACK_TRACE;
	AITargetComponent *aiTarget = entity->Get<AITargetComponent>();
	ASSERT(aiTarget != NULL);
	
	AcquireTarget(entity, aiTarget, delta);
	Entity *target = aiTarget->target;
	
	if (target == NULL)
		return FALSE;
	if (!GetEntityManager()->IsValid(target))
		return FALSE;
	
	BoundingSphereComponent *targetBounds = target->Get<BoundingSphereComponent>();
	BoundingSphereComponent *entityBounds = entity->Get<BoundingSphereComponent>();
	
	float tooCloseDistance = targetBounds->bounds.radius + entityBounds->bounds.radius + TOO_CLOSE_TOLERANCE;
	Vector3 targetPosition = target->Get<PositionComponent>()->position;
	Vector3 entityPosition = entity->Get<PositionComponent>()->position;
	
	// make the entity face the player
	float newEntityOrientation = GetAngleBetweenPoints(targetPosition.x, targetPosition.z, entityPosition.x, entityPosition.z);
	entity->Get<OrientationXZComponent>()->angle = newEntityOrientation;
	
	if (Vector3::Distance(targetPosition, entityPosition) > tooCloseDistance)
	{
		// if the entity isn't too close to the player, continue moving
		MoveForwardEvent *moveEvent = new MoveForwardEvent(entity);
		GetEventManager()->Queue(moveEvent);
		
		if (timers != NULL && timers->hasWaitBeforeAttackingTimer)
		{
			if (timers->waitBeforeAttackingTimeLeft == 0.0f)
				timers->waitBeforeAttackingTimeLeft = Rnd(timers->waitBeforeAttackingTimeMin, timers->waitBeforeAttackingTimeMax);
		}
	}
	else if (!entity->Has<AttackCooldownComponent>())
	{
		if (timers != NULL && timers->hasWaitBeforeAttackingTimer && timers->waitBeforeAttackingTimeLeft > 0.0f)
			return TRUE;
		
		// if we're up close to the player, then we can attack them
		AttackEvent *attackEvent = new AttackEvent(entity);
		GetEventManager()->Queue(attackEvent);
	}
	
	return TRUE;
}

void NPCSystem::AcquireTarget(Entity *entity, AITargetComponent *aiTarget, float delta)
{
	STACK_TRACE;
	if (entity->Has<AIAggroAttackerComponent>())
	{
		AIAggroAttackerComponent *aggroTarget = entity->Get<AIAggroAttackerComponent>();
		if (aggroTarget->currentlyAggroToTarget)
		{
			if (aggroTarget->timeBeforeNextDistanceCheck > 0.0f)
			{
				// countdown the timer and return the existing target for now
				aggroTarget->timeBeforeNextDistanceCheck -= delta;
				if (aggroTarget->timeBeforeNextDistanceCheck < 0.0f)
					aggroTarget->timeBeforeNextDistanceCheck = 0.0f;

				// don't allow anything else to change the existing target
				return;
			}
			else
			{
				if (!GetEntityManager()->IsValid(aiTarget->target) || aiTarget->target->Has<DeadComponent>())
				{
					// target is dead, so untarget+aggro it
					aiTarget->target = NULL;
					aggroTarget->currentlyAggroToTarget = FALSE;
				}
				else
				{				
					// ensure player is within range to be considered a target
					PositionComponent *targetPosition = aiTarget->target->Get<PositionComponent>();
					PositionComponent *entityPosition = entity->Get<PositionComponent>();
					
					// set timer for next acquire
					aggroTarget->timeBeforeNextDistanceCheck = TIME_BETWEEN_TARGET_ACQUIRES;
					
					float distance = Vector3::Distance(targetPosition->position, entityPosition->position);
					if (distance > aggroTarget->maximumAggroDistance)
					{
						// clear existing target (out of range), and let any of the
						// other AI target acquisition stuff below run (if present)
						aiTarget->target = NULL;
						aggroTarget->currentlyAggroToTarget = FALSE;
					}
					else
						// keep existing target, and don't let any other AI target
						// stuff change the target
						return;
				}
			}
		}
	}
	
	if (entity->Has<AITargetPlayerComponent>())
	{
		AITargetPlayerComponent *targetPlayer = entity->Get<AITargetPlayerComponent>();
		if (targetPlayer->timeBeforeNextAcquire > 0.0f)
		{
			// countdown the timer and return the existing target for now
			targetPlayer->timeBeforeNextAcquire -= delta;
			if (targetPlayer->timeBeforeNextAcquire < 0.0f)
				targetPlayer->timeBeforeNextAcquire = 0.0f;
		}
		else
		{
			Entity *player = GetEntityManager()->GetWith<PlayerComponent>();
			if (player == NULL || player->Has<DeadComponent>())
			{
				aiTarget->target = NULL;
				return;
			}
			else
			{
				float maxDistance;
				if (aiTarget->target == NULL)
					maxDistance = targetPlayer->maximumDistanceToFindTarget;
				else
					maxDistance = targetPlayer->maximumDistanceToKeepTarget;
				
				// ensure player is within range to be considered a target
				PositionComponent *playerPosition = player->Get<PositionComponent>();
				PositionComponent *entityPosition = entity->Get<PositionComponent>();
				
				// set timer for next acquire
				targetPlayer->timeBeforeNextAcquire = TIME_BETWEEN_TARGET_ACQUIRES;

				float distance = Vector3::Distance(playerPosition->position, entityPosition->position);
				if (distance < maxDistance)
				{
					float chanceToAcquireRoll = Rnd(0.0f, 1.0f);
					if (chanceToAcquireRoll <= targetPlayer->percentChanceToSuccessfullyAcquireTarget)
						aiTarget->target = player;
					return;
				}
				else
				{
					aiTarget->target = NULL;
					return;
				}
			}
		}
	}
	else if (entity->Has<AITargetNearestComponent>())
	{
		AITargetNearestComponent *targetNearest = entity->Get<AITargetNearestComponent>();
		if (targetNearest->timeBeforeNextAcquire > 0.0f)
		{
			// countdown the timer and return the existing target for now
			targetNearest->timeBeforeNextAcquire -= delta;
			if (targetNearest->timeBeforeNextAcquire < 0.0f)
				targetNearest->timeBeforeNextAcquire = 0.0f;
		}
		else
		{
			PositionComponent *entityPosition = entity->Get<PositionComponent>();
			
			Entity *existingTarget = aiTarget->target;
			
			// check if we should keep following the existing target 
			// (assuming we have an existing target and it's not dead)
			if (existingTarget != NULL && !existingTarget->Has<DeadComponent>())
			{
				PositionComponent *targetPosition = existingTarget->Get<PositionComponent>();
				float distance = Vector3::Distance(entityPosition->position, targetPosition->position);
				if (distance < targetNearest->maximumDistanceToKeepTarget)
					return;
			}
			
			// if we get to here, we need to check for another target nearby
			
			Entity *closestTarget = NULL;
			float closestDistanceSquared = 0.0f;
			
			// attempt to find a target
			EntityList targetables;
			GetEntityManager()->GetAllWith<AITargetableComponent>(targetables);
			for (EntityList::iterator i = targetables.begin(); i != targetables.end(); ++i)
			{
				Entity *targetable = *i;

				// targetable and entity could potentially be the same entity...
				if (targetable == entity)
					continue;
				
				// don't target dead entities
				if (targetable->Has<DeadComponent>())
					continue;
				
				PositionComponent *targetablePosition = targetable->Get<PositionComponent>();
				
				float distanceSquared = Vector3::DistanceSquared(entityPosition->position, targetablePosition->position);
				if (closestTarget == NULL || (closestTarget != NULL && distanceSquared < closestDistanceSquared))
				{
					closestTarget = targetable;
					closestDistanceSquared = distanceSquared;
				}
			}
			
			// set timer for next acquire
			targetNearest->timeBeforeNextAcquire = TIME_BETWEEN_TARGET_ACQUIRES;
			
			float closestDistance = sqrtf(closestDistanceSquared);
			if (closestDistance < targetNearest->maximumDistanceToFindTarget)
			{
				float chanceToAcquireRoll = Rnd(0.0f, 1.0f);
				if (chanceToAcquireRoll <= targetNearest->percentChanceToSuccessfullyAcquireTarget)
					aiTarget->target = closestTarget;
				return;
			}
			else
			{
				aiTarget->target = NULL;
				return;
			}
		}
	}
	
	return;
}

BOOL NPCSystem::Handle(const Event *event)
{
	STACK_TRACE;
	if (event->Is<AttackEvent>())
	{
		Entity *attacker = event->As<AttackEvent>()->GetEntity();
		if (!attacker->Has<NPCComponent>())
			return FALSE;
		
		// reset related timers
		AITimersComponent *timers = attacker->Get<AITimersComponent>();
		if (timers != NULL)
		{
			if (timers->hasAfterAttackPauseTimer)
				timers->afterAttackPauseTimeLeft = Rnd(timers->afterAttackPauseTimeMin, timers->afterAttackPauseTimeMax);
		}
		
		// check if this entity will randomly run away post-attack
		AIRunAwayAfterAttackingComponent *runAwayAfterAttacking = attacker->Get<AIRunAwayAfterAttackingComponent>();
		if (runAwayAfterAttacking != NULL)
		{
			float chanceToRunRoll = Rnd(0.0f, 1.0f);
			if (chanceToRunRoll < runAwayAfterAttacking->percentChanceToRunAway)
			{
				runAwayAfterAttacking->waitBeforeRunningTimeLeft = Rnd(runAwayAfterAttacking->waitBeforeRunningTimeMin, runAwayAfterAttacking->waitBeforeRunningTimeMax);
				runAwayAfterAttacking->runAwayTimeLeft = Rnd(runAwayAfterAttacking->runAwayTimeMin, runAwayAfterAttacking->runAwayTimeMax);
				runAwayAfterAttacking->runAwayAngle = Rnd(1, 36) * DegreesToRadians(10.0f);
				
				// start running in the next tick
			}
		}
	}
	if (event->Is<KilledEvent>())
	{
		const KilledEvent *e = event->As<KilledEvent>();
		if (e->GetEntity()->Has<NPCComponent>())
			e->GetEntity()->Remove<NPCComponent>();
	}
	if (event->Is<HurtEvent>())
	{
		const HurtEvent *e = event->As<HurtEvent>();
		Entity *hurtEntity = e->GetEntity();
		Entity *attacker = e->source;

		// reset related timers
		AITimersComponent *timers = hurtEntity->Get<AITimersComponent>();
		if (timers != NULL)
		{
			if (timers->hasAfterHurtPauseTimer)
				timers->afterHurtPauseTimeLeft = Rnd(timers->afterHurtPauseTimeMin, timers->afterHurtPauseTimeMax);
		}
		
		// check if we need to aggro to the attacker
		AIAggroAttackerComponent *aggroAttacker = hurtEntity->Get<AIAggroAttackerComponent>();
		if (aggroAttacker != NULL && GetEntityManager()->IsValid(attacker))
		{
			AITargetComponent *existingTarget = hurtEntity->Get<AITargetComponent>();
			ASSERT(existingTarget != NULL);
			if (aggroAttacker->onlyIfNoExistingTarget == FALSE || existingTarget->target == NULL)
			{
				float chanceToAcquireRoll = Rnd(0.0f, 1.0f);
				if (chanceToAcquireRoll <= aggroAttacker->percentChanceToSuccessfullyAcquireTarget)
				{
					aggroAttacker->currentlyAggroToTarget = TRUE;
					aggroAttacker->timeBeforeNextDistanceCheck = TIME_BETWEEN_TARGET_ACQUIRES;
					existingTarget->target = attacker;
				}
			}
		}
	}

	return FALSE;
}

