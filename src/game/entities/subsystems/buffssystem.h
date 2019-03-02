#ifndef __GAME_ENTITIES_SUBSYSTEMS_BUFFSSYSTEM_H_INCLUDED__
#define __GAME_ENTITIES_SUBSYSTEMS_BUFFSSYSTEM_H_INCLUDED__

#include "../../../entities/componentsystem.h"

class EntityManager;
class EventManager;
struct Event;

class BuffsSystem : public ComponentSystem
{
public:
	static COMPONENTSYSTEM_TYPE GetType()
	{
		static COMPONENTSYSTEM_TYPE typeName = "BuffsSystem";
		return typeName;
	}
	COMPONENTSYSTEM_TYPE GetTypeOf() const                 { return GetType(); }
	
	BuffsSystem(EntityManager *entityManager, EventManager *eventManager);
	virtual ~BuffsSystem();
	
	void OnUpdate(float delta);
	
	BOOL Handle(const Event *event);
};

#endif

