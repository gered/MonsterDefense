#ifndef __GAME_ENTITIES_SUBSYSTEMS_POWERUPSYSTEM_H_INCLUDED__
#define __GAME_ENTITIES_SUBSYSTEMS_POWERUPSYSTEM_H_INCLUDED__

#include "../../../entities/componentsystem.h"

class EntityManager;
class EventManager;
struct Event;

class PowerUpSystem : public ComponentSystem
{
public:
	static COMPONENTSYSTEM_TYPE GetType()
	{
		static COMPONENTSYSTEM_TYPE typeName = "PowerUpSystem";
		return typeName;
	}
	COMPONENTSYSTEM_TYPE GetTypeOf() const                 { return GetType(); }

	PowerUpSystem(EntityManager *entityManager, EventManager *eventManager);
	virtual ~PowerUpSystem();

	void OnUpdate(float delta);
	
	BOOL Handle(const Event *event);
};

#endif

