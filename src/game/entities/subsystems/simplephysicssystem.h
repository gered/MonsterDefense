#ifndef __GAME_ENTITIES_SUBSYSTEMS_SIMPLEPHYSICSSYSTEM_H_INCLUDED__
#define __GAME_ENTITIES_SUBSYSTEMS_SIMPLEPHYSICSSYSTEM_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../entities/componentsystem.h"

class Entity;
class EntityManager;
class EventManager;
class PositionComponent;
class SimplePhysicsComponent;
class WorldComponent;
struct Event;
struct Vector3;

class SimplePhysicsSystem : public ComponentSystem
{
public:
	static COMPONENTSYSTEM_TYPE GetType()
	{
		static COMPONENTSYSTEM_TYPE typeName = "SimplePhysicsSystem";
		return typeName;
	}
	COMPONENTSYSTEM_TYPE GetTypeOf() const                 { return GetType(); }

	SimplePhysicsSystem(EntityManager *entityManager, EventManager *eventManager);
	virtual ~SimplePhysicsSystem();

	void OnUpdate(float delta);

	BOOL Handle(const Event *event);

private:
	void Move(Entity *entity, const Vector3 &direction);
	void Stop(Entity *entity);

	void UpdatePossibleCollisionArea(SimplePhysicsComponent *physics, PositionComponent *position);
	Vector3 ProcessMovement(SimplePhysicsComponent *physics, PositionComponent *position, WorldComponent *world, float delta);
	void CheckForCollisionsWithWorld(SimplePhysicsComponent *physics, WorldComponent *world);
};

#endif
