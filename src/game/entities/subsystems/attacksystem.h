#ifndef __GAME_ENTITIES_SUBSYSTEMS_ATTACKSYSTEM_H_INCLUDED__
#define __GAME_ENTITIES_SUBSYSTEMS_ATTACKSYSTEM_H_INCLUDED__

#include "../../../entities/componentsystem.h"

class EntityManager;
class EventManager;
struct Event;

class AttackSystem : public ComponentSystem
{
public:
	static COMPONENTSYSTEM_TYPE GetType()
	{
		static COMPONENTSYSTEM_TYPE typeName = "AttackSystem";
		return typeName;
	}
	COMPONENTSYSTEM_TYPE GetTypeOf() const                 { return GetType(); }
	
	AttackSystem(EntityManager *entityManager, EventManager *eventManager);
	virtual ~AttackSystem();
	
	void OnUpdate(float delta);
	
	BOOL Handle(const Event *event);
};

#endif

