#ifndef __GAME_ENTITIES_SUBSYSTEMS_ENTITYSTATESYSTEM_H_INCLUDED__
#define __GAME_ENTITIES_SUBSYSTEMS_ENTITYSTATESYSTEM_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../entities/componentsystem.h"

class Entity;
class EntityManager;
class EventManager;
struct Event;

class EntityStateSystem : public ComponentSystem
{
public:
	static COMPONENTSYSTEM_TYPE GetType()
	{
		static COMPONENTSYSTEM_TYPE typeName = "EntityStateSystem";
		return typeName;
	}
	COMPONENTSYSTEM_TYPE GetTypeOf() const                 { return GetType(); }
	
	EntityStateSystem(EntityManager *entityManager, EventManager *eventManager);
	virtual ~EntityStateSystem();
	
	void OnUpdate(float delta);
	
	BOOL Handle(const Event *event);
};

#endif

