#ifndef __GAME_ENTITIES_COMPONENTS_PHYSICSCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_PHYSICSCOMPONENT_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../entities/component.h"
#include "../forces/force.h"
#include "../../../framework/math/boundingbox.h"
#include "../../../framework/math/collisionpacket.h"
#include "../../../framework/math/point3.h"
#include "../../../framework/math/ray.h"
#include "../../../framework/math/vector3.h"

struct Tile;

class PhysicsComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "PhysicsComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }

	PhysicsComponent();
	virtual ~PhysicsComponent();
	void Reset();
	void ClearForces();

	Vector3 velocity;
	Vector3 walkingVelocity;
	float walkingAcceleration;
	float maxWalkSpeed;
	float friction;

	ForceList forces;
	Vector3 forceVelocity;

	BoundingBox possibleCollisionArea;
	uint32_t collisionRecursionDepth;
	CollisionPacket collisionPacket;
	Point3 collisionTilePosition;
	Point3 standingOnTilePosition;
	const Tile *standingOnTile;
	
	BOOL isWalking;
	BOOL wasWalking;
	BOOL isInMotion;
	BOOL wasInMotion;
	BOOL isFalling;
	BOOL wasFalling;
	BOOL isOnGround;
	BOOL wasOnGround;
	BOOL isSliding;
	BOOL wasSliding;

	float fallDistance;
	float lastYPosition;
	float currentFallDistance;

	Ray slidingPlaneNormal;
	Vector3 nearestIntersection;
	Vector3 hitTriangle[3];

	PhysicsComponent* SetBoundsRadius(float radius);
};

inline PhysicsComponent::PhysicsComponent()
{
	Reset();
}

inline PhysicsComponent::~PhysicsComponent()
{
	ClearForces();
}

inline void PhysicsComponent::Reset()
{
	velocity = ZERO_VECTOR;
	walkingVelocity = ZERO_VECTOR;
	walkingAcceleration = 0.0f;
	maxWalkSpeed = 0.0f;
	friction = 0.0f;
	ClearForces();
	forceVelocity = ZERO_VECTOR;
	possibleCollisionArea = BoundingBox();
	collisionRecursionDepth = 0;
	collisionPacket = CollisionPacket();
	collisionTilePosition = ZERO_POINT;
	standingOnTilePosition = ZERO_POINT;
	standingOnTile = NULL;
	isWalking = FALSE;
	wasWalking = FALSE;
	isInMotion = FALSE;
	wasInMotion = FALSE;
	isFalling = FALSE;
	wasFalling = FALSE;
	isOnGround = FALSE;
	wasOnGround = FALSE;
	isSliding = FALSE;
	wasSliding = FALSE;
	fallDistance = 0.0f;
	lastYPosition = 0.0f;
	currentFallDistance = 0.0f;
}

inline void PhysicsComponent::ClearForces()
{
	while (!forces.empty())
	{
		Force *force = forces.front();
		SAFE_DELETE(force);
		forces.pop_front();
	}
}

inline PhysicsComponent* PhysicsComponent::SetBoundsRadius(float radius)
{
	collisionPacket.ellipsoidRadius = Vector3(radius, radius, radius);
	return this;
}

#endif

