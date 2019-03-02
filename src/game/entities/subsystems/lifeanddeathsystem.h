#ifndef __GAME_ENTITIES_SUBSYSTEMS_LIFEANDDEATHSYSTEM_H_INCLUDED__
#define __GAME_ENTITIES_SUBSYSTEMS_LIFEANDDEATHSYSTEM_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../entities/componentsystem.h"

class EntityManager;
class EventManager;
struct Event;

class LifeAndDeathSystem : public ComponentSystem
{
public:
	static COMPONENTSYSTEM_TYPE GetType()
	{
		static COMPONENTSYSTEM_TYPE typeName = "LifeAndDeathSystem";
		return typeName;
	}
	COMPONENTSYSTEM_TYPE GetTypeOf() const                 { return GetType(); }

	LifeAndDeathSystem(EntityManager *entityManager, EventManager *eventManager);
	virtual ~LifeAndDeathSystem();

	void OnUpdate(float delta);
	
	BOOL Handle(const Event *event);
};

#endif

