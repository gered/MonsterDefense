#include "../../../../framework/debug.h"

#include "monsterpreset.h"
#include "../../components/affectedbygravitycomponent.h"
#include "../../components/animationcomponent.h"
#include "../../components/autoanimationcomponent.h"
#include "../../components/blobshadowcomponent.h"
#include "../../components/boundingspherecomponent.h"
#include "../../components/buffscomponent.h"
#include "../../components/deadcomponent.h"
#include "../../components/colorcomponent.h"
#include "../../components/lifecomponent.h"
#include "../../components/npccomponent.h"
#include "../../components/orientationxzcomponent.h"
#include "../../components/physicscomponent.h"
#include "../../components/playercomponent.h"
#include "../../components/positioncomponent.h"
#include "../../components/pushercomponent.h"
#include "../../components/pushablecomponent.h"
#include "../../components/statecomponent.h"
#include "../../components/suppressspawnparticlescomponent.h"
#include "../../events/scoreevent.h"
#include "../../../../entities/entity.h"
#include "../../../../entities/entitymanager.h"
#include "../../../../events/eventmanager.h"

MonsterPreset::MonsterPreset(EntityManager *entityManager)
	: BaseCharacterEntityPreset(entityManager)
{
	STACK_TRACE;
}

MonsterPreset::~MonsterPreset()
{
	STACK_TRACE;
}

Entity* MonsterPreset::Create(EntityPresetArgs *args)
{
	STACK_TRACE;
	Entity *entity = GetEntityManager()->Add();
	
	entity->Add<AffectedByGravityComponent>();
	entity->Add<AnimationComponent>();
	entity->Add<AutoAnimationComponent>();
	entity->Add<BlobShadowComponent>();
	entity->Add<BoundingSphereComponent>();
	entity->Add<ColorComponent>()->color = COLOR_WHITE;
	entity->Add<LifeComponent>();
	entity->Add<NPCComponent>();
	entity->Add<OrientationXZComponent>();
	entity->Add<PhysicsComponent>();
	entity->Add<PositionComponent>();
	entity->Add<PushableComponent>();
	entity->Add<PusherComponent>();
	entity->Add<StateComponent>();
	entity->Add<BuffsComponent>();
	
	return entity;
}

void MonsterPreset::OnSpawn(Entity *entity)
{
	STACK_TRACE;
	if (!entity->Has<SuppressSpawnParticlesComponent>())
		SpawnGreenSmokeAt(entity);
}

void MonsterPreset::OnDespawn(Entity *entity)
{
	STACK_TRACE;
	if (entity->Has<DeadComponent>())
		SpawnPuffOfSmokeAt(entity);
}

void MonsterPreset::OnHeal(Entity *entity, Entity *source)
{
	STACK_TRACE;
}

void MonsterPreset::OnHurt(Entity *entity, Entity *source)
{
	STACK_TRACE;
}

void MonsterPreset::OnKilled(Entity *entity, Entity *killer)
{
	STACK_TRACE;
}

void MonsterPreset::AwardScoreToPlayer(Entity *sourceOfScore, int32_t score)
{
	STACK_TRACE;
	Entity *player = GetEntityManager()->GetWith<PlayerComponent>();
	if (player != NULL)
	{
		ScoreEvent *scoreEvent = new ScoreEvent(player);
		scoreEvent->scoreAwarded = score;
		
		// attack the source entity's position to the score event if available
		if (sourceOfScore != NULL)
		{
			scoreEvent->hasSourcePosition = TRUE;
			scoreEvent->sourcePosition = sourceOfScore->Get<PositionComponent>()->position;
		}
		
		GetEventManager()->Queue(scoreEvent);
	}
}
