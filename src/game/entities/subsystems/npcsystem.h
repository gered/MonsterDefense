#ifndef __GAME_ENTITIES_SUBSYSTEMS_NPCSYSTEM_H_INCLUDED__
#define __GAME_ENTITIES_SUBSYSTEMS_NPCSYSTEM_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../entities/componentsystem.h"

class Entity;
class EntityManager;
class EventManager;
class AIFollowAndAttackTargetComponent;
class AIRandomMovementComponent;
class AIRunAwayAfterAttackingComponent;
class AITargetComponent;
class AITimersComponent;
struct Event;

class NPCSystem : public ComponentSystem
{
public:
	static COMPONENTSYSTEM_TYPE GetType()
	{
		static COMPONENTSYSTEM_TYPE typeName = "NPCSystem";
		return typeName;
	}
	COMPONENTSYSTEM_TYPE GetTypeOf() const                 { return GetType(); }

	NPCSystem(EntityManager *entityManager, EventManager *eventManager);
	virtual ~NPCSystem();

	void OnUpdate(float delta);

	BOOL Handle(const Event *event);

private:
	BOOL Process(AITimersComponent *timers, Entity *entity, float delta);
	BOOL Process(AIRandomMovementComponent *ai, Entity *entity, float delta);
	BOOL Process(AIRunAwayAfterAttackingComponent *ai, Entity *entity, float delta);
	BOOL Process(AIFollowAndAttackTargetComponent *ai, AITimersComponent *timers, Entity *entity, float delta);
	
	void AcquireTarget(Entity *entity, AITargetComponent *aiTarget, float delta);
};

#endif

