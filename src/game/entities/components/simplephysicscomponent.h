#ifndef __GAME_ENTITIES_COMPONENTS_SIMPLEPHYSICSCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_SIMPLEPHYSICSCOMPONENT_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../entities/component.h"
#include "../../../framework/math/boundingbox.h"
#include "../../../framework/math/collisionpacket.h"
#include "../../../framework/math/point3.h"
#include "../../../framework/math/vector3.h"

class SimplePhysicsComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "SimplePhysicsComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }

	SimplePhysicsComponent();
	virtual ~SimplePhysicsComponent();
	void Reset();

	Vector3 velocity;
	float speed;
	Vector3 nextMovement;

	BoundingBox possibleCollisionArea;
	CollisionPacket collisionPacket;
	Point3 collisionTilePosition;

	Vector3 nearestIntersection;
	Vector3 hitTriangle[3];

	SimplePhysicsComponent* SetBoundsRadius(float radius);
};

inline SimplePhysicsComponent::SimplePhysicsComponent()
{
	Reset();
}

inline SimplePhysicsComponent::~SimplePhysicsComponent()
{
}

inline void SimplePhysicsComponent::Reset()
{
	velocity = ZERO_VECTOR;
	speed = 0.0f;
	nextMovement = ZERO_VECTOR;
	possibleCollisionArea = BoundingBox();
	collisionPacket = CollisionPacket();
	collisionTilePosition = ZERO_POINT;
	nearestIntersection = ZERO_VECTOR;
}

inline SimplePhysicsComponent* SimplePhysicsComponent::SetBoundsRadius(float radius)
{
	collisionPacket.ellipsoidRadius = Vector3(radius, radius, radius);
	return this;
}

#endif

