#ifndef __GAME_ENTITIES_SUBSYSTEMS_PHYSICSSYSTEM_H_INCLUDED__
#define __GAME_ENTITIES_SUBSYSTEMS_PHYSICSSYSTEM_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../entities/componentsystem.h"

class Entity;
class EntityManager;
class EventManager;
class Force;
class AffectedByTerrainFrictionComponent;
class PhysicsComponent;
class PositionComponent;
class WorldComponent;
struct Event;
struct Vector3;

struct TerrainFrictionValues
{
	float friction;
	float walkAcceleration;
	float maxWalkSpeed;
};

class PhysicsSystem : public ComponentSystem
{
public:
	static COMPONENTSYSTEM_TYPE GetType()
	{
		static COMPONENTSYSTEM_TYPE typeName = "PhysicsSystem";
		return typeName;
	}
	COMPONENTSYSTEM_TYPE GetTypeOf() const                 { return GetType(); }

	PhysicsSystem(EntityManager *entityManager, EventManager *eventManager);
	virtual ~PhysicsSystem();

	void OnUpdate(float delta);

	BOOL Handle(const Event *event);

private:
	void TriggerStateChangeEvents(PhysicsComponent *physics, Entity *entity);

	void Move(Entity *entity, const Vector3 &direction);
	void Stop(Entity *entity);
	void Jump(Entity *entity);
	void ApplyForce(PhysicsComponent *physics, Force *force);
	void ApplyForce(PhysicsComponent *physics, const Vector3 &direction, float strength, float friction);
	void ProcessForces(PhysicsComponent *physics, float delta);
	
	void PushIntersectingEntities(Entity *pusher);

	void UpdateIsWalkingState(PhysicsComponent *physics);
	void UpdateIsInMotionState(PhysicsComponent *physics);
	void UpdateIsFallingState(PhysicsComponent *physics, PositionComponent *position);
	void UpdateIsOnGroundState(PhysicsComponent *physics);

	void UpdateStandingTileCoords(PhysicsComponent *physics, PositionComponent *position, WorldComponent *world);
	void UpdatePossibleCollisionArea(PhysicsComponent *physics, PositionComponent *position);
	Vector3 ProcessMovement(PhysicsComponent *physics, PositionComponent *position, WorldComponent *world, float delta);
	Vector3 ProcessGravity(PhysicsComponent *physics, PositionComponent *position, WorldComponent *world, float delta);
	Vector3 GetNewPositionForMovement(PhysicsComponent *physics, WorldComponent *world, const Vector3 &position, const Vector3 &velocity, Vector3 &responseVelocity, BOOL canSlide, BOOL onlySlideIfTooSteep = FALSE, float steepYAngle = 0.0f);
	void CheckForCollisionsWithWorld(PhysicsComponent *physics, WorldComponent *world);

	TerrainFrictionValues GetTerrainFrictionFor(PhysicsComponent *physics, AffectedByTerrainFrictionComponent *affectedByTerrainFriction);
};

#endif

