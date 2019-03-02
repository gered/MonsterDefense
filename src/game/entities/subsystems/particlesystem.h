#ifndef __GAME_ENTITIES_SUBSYSTEMS_PARTICLESYSTEM_H_INCLUDED__
#define __GAME_ENTITIES_SUBSYSTEMS_PARTICLESYSTEM_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../entities/componentsystem.h"

class Entity;
class EntityManager;
class EventManager;
struct Event;

class ParticleSystem : public ComponentSystem
{
public:
	static COMPONENTSYSTEM_TYPE GetType()
	{
		static COMPONENTSYSTEM_TYPE typeName = "ParticleSystem";
		return typeName;
	}
	COMPONENTSYSTEM_TYPE GetTypeOf() const                 { return GetType(); }

	ParticleSystem(EntityManager *entityManager, EventManager *eventManager);
	virtual ~ParticleSystem();

	void OnUpdate(float delta);

	BOOL Handle(const Event *event);
};

#endif

