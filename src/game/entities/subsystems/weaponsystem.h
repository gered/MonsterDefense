#ifndef __GAME_ENTITIES_SUBSYSTEMS_WEAPONSYSTEM_H_INCLUDED__
#define __GAME_ENTITIES_SUBSYSTEMS_WEAPONSYSTEM_H_INCLUDED__

#include "../../../entities/componentsystem.h"

class EntityManager;
class EventManager;
struct Event;

class WeaponSystem : public ComponentSystem
{
public:
	static COMPONENTSYSTEM_TYPE GetType()
	{
		static COMPONENTSYSTEM_TYPE typeName = "WeaponSystem";
		return typeName;
	}
	COMPONENTSYSTEM_TYPE GetTypeOf() const                 { return GetType(); }
	
	WeaponSystem(EntityManager *entityManager, EventManager *eventManager);
	virtual ~WeaponSystem();
	
	void OnUpdate(float delta);
	
	BOOL Handle(const Event *event);
};

#endif

