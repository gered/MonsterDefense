#ifndef __GAME_ENTITIES_SUBSYSTEMS_BULLETSYSTEM_H_INCLUDED__
#define __GAME_ENTITIES_SUBSYSTEMS_BULLETSYSTEM_H_INCLUDED__

#include "../../../entities/componentsystem.h"

class EntityManager;
class EventManager;
struct Event;

class BulletSystem : public ComponentSystem
{
public:
	static COMPONENTSYSTEM_TYPE GetType()
	{
		static COMPONENTSYSTEM_TYPE typeName = "BulletSystem";
		return typeName;
	}
	COMPONENTSYSTEM_TYPE GetTypeOf() const                 { return GetType(); }

	BulletSystem(EntityManager *entityManager, EventManager *eventManager);
	virtual ~BulletSystem();

	void OnUpdate(float delta);

	BOOL Handle(const Event *event);
};

#endif

