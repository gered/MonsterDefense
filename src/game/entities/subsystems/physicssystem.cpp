#include "../../../framework/debug.h"

#include "physicssystem.h"
#include "../components/affectedbyfalldamagecomponent.h"
#include "../components/affectedbygravitycomponent.h"
#include "../components/affectedbyterrainfrictioncomponent.h"
#include "../components/boundingspherecomponent.h"
#include "../components/orientationxzcomponent.h"
#include "../components/physicscomponent.h"
#include "../components/positioncomponent.h"
#include "../components/pushablecomponent.h"
#include "../components/pushercomponent.h"
#include "../components/removephysicswhenstillcomponent.h"
#include "../events/entitystatechangeevent.h"
#include "../events/hurtevent.h"
#include "../events/jumpevent.h"
#include "../events/moveforwardevent.h"
#include "../events/moveindirectionevent.h"
#include "../events/stopmovementevent.h"
#include "../forces/force.h"
#include "../forces/jumpforce.h"
#include "../globalcomponents/worldcomponent.h"
#include "../../physicsconstants.h"
#include "../../../entities/entity.h"
#include "../../../entities/entitymanager.h"
#include "../../../events/event.h"
#include "../../../events/eventmanager.h"
#include "../../../framework/math/boundingbox.h"
#include "../../../framework/math/boundingsphere.h"
#include "../../../framework/math/intersectiontester.h"
#include "../../../framework/math/mathhelpers.h"
#include "../../../framework/math/matrix4x4.h"
#include "../../../framework/math/plane.h"
#include "../../../framework/math/ray.h"
#include "../../../framework/math/vector3.h"
#include "../../../tilemap/tile.h"
#include "../../../tilemap/tilemap.h"
#include "../../../tilemap/tilemesh.h"
#include "../../../tilemap/tilemeshcollection.h"

PhysicsSystem::PhysicsSystem(EntityManager *entityManager, EventManager *eventManager)
	: ComponentSystem(entityManager, eventManager)
{
	STACK_TRACE;
	ListenFor<JumpEvent>();
	ListenFor<MoveForwardEvent>();
	ListenFor<MoveInDirectionEvent>();
	ListenFor<StopMovementEvent>();
}

PhysicsSystem::~PhysicsSystem()
{
	STACK_TRACE;
	StopListeningFor<JumpEvent>();
	StopListeningFor<MoveForwardEvent>();
	StopListeningFor<MoveInDirectionEvent>();
	StopListeningFor<StopMovementEvent>();
}

void PhysicsSystem::OnUpdate(float delta)
{
	STACK_TRACE;
	WorldComponent *world = GetEntityManager()->GetGlobalComponent<WorldComponent>();
	ASSERT(world != NULL);

	EntityList list;

	// process complex physics first
	GetEntityManager()->GetAllWith<PhysicsComponent>(list);
	for (EntityList::iterator i = list.begin(); i != list.end(); ++i)
	{
		Entity *entity = *i;
		PhysicsComponent *physics = entity->Get<PhysicsComponent>();

		PositionComponent *position = entity->Get<PositionComponent>();
		ASSERT(position != NULL);

		// to accurately check if the entity is walking we need to check this first!
		// walking only adds velocity in the XZ plane. forces can add velocity in any
		// direction, so if we added them first, we couldn't just check XZ
		UpdateIsWalkingState(physics);
		
		// push other intersecting entities out of the way of this one
		// (they may themselves push back when we process them in this loop too)
		if (entity->Has<PusherComponent>())
			PushIntersectingEntities(entity);

		// apply forces. this directly affects velocity, so it needs to go next!
		ProcessForces(physics, delta);

		TerrainFrictionValues frictionValues = GetTerrainFrictionFor(physics, entity->Get<AffectedByTerrainFrictionComponent>());

		// apply friction and then combine walking and force velocity's into one
		// to get the entity's current velocity for this tick
		physics->walkingVelocity *= frictionValues.friction;
		physics->forceVelocity *= frictionValues.friction;

		// walking velocity is limited by a maximum speed for the entity
		Vector3 fixedWalkingVelocity = physics->walkingVelocity;
		if (Vector3::Length(fixedWalkingVelocity) > frictionValues.maxWalkSpeed)
			fixedWalkingVelocity = Vector3::SetLength(fixedWalkingVelocity, frictionValues.maxWalkSpeed);

		physics->velocity = (fixedWalkingVelocity * delta) + physics->forceVelocity;

		UpdateIsInMotionState(physics);
		UpdateIsFallingState(physics, position);

		// figure out what section of the world we could possible collide with 
		// based on the entity's current position and velocity
		UpdatePossibleCollisionArea(physics, position);

		// handle movement and gravity collision detection and response
		physics->wasSliding = physics->isSliding;
		physics->isSliding = FALSE;
		Vector3 postResponseVelocity;
		postResponseVelocity = ProcessMovement(physics, position, world, delta);
		if (entity->Has<AffectedByGravityComponent>())
			postResponseVelocity += ProcessGravity(physics, position, world, delta);
		physics->velocity = postResponseVelocity;

		UpdateIsOnGroundState(physics);

		if (entity->Has<AffectedByFallDamageComponent>())
		{
			// if the entity just stopped falling down and the fall height was 
			// too large, then we need to apply fall damage
			if (physics->wasFalling && !physics->isFalling && physics->fallDistance >= MINIMUM_HIGH_FALL_DISTANCE)
			{
				// minimum damage for falling from so high up
				float damagePercent = FALL_DAMAGE_PERCENT_PER_HEIGHT;

				// now high over the minimum were they? each step over the minimum
				// equates to additional damage
				int stepsOver = (int)((physics->fallDistance - MINIMUM_HIGH_FALL_DISTANCE) / HIGHER_FALL_DISTANCE_STEPS);
				damagePercent += (FALL_DAMAGE_PERCENT_PER_HEIGHT * stepsOver);

				HurtEvent *event = new HurtEvent(entity);
				event->usePercent = TRUE;
				event->percent = damagePercent;
				GetEntityManager()->GetEventManager()->Queue(event);
			}
		}

		UpdateStandingTileCoords(physics, position, world);

		TriggerStateChangeEvents(physics, entity);
		
		// FINAL thing to do -- check if we should remove the physics component
		if (entity->Has<RemovePhysicsWhenStillComponent>() && physics->isInMotion == FALSE)
			entity->Remove<PhysicsComponent>();
	}
}

void PhysicsSystem::TriggerStateChangeEvents(PhysicsComponent *physics, Entity *entity)
{
	STACK_TRACE;
	EntityStateChangeEvent event(entity);

	if (physics->isFalling && !physics->wasFalling)
	{
		event.state = ENTITYSTATE_FALLING;
		GetEntityManager()->GetEventManager()->Trigger(&event);
	}
	else if (physics->wasFalling && physics->isWalking)
	{
		event.state = ENTITYSTATE_WALKING;
		GetEntityManager()->GetEventManager()->Trigger(&event);
	}
	else if (physics->wasFalling && !physics->isFalling)
	{
		event.state = ENTITYSTATE_IDLE;
		GetEntityManager()->GetEventManager()->Trigger(&event);
	}
	else if (physics->isWalking && !physics->wasWalking && !physics->isFalling)
	{
		event.state = ENTITYSTATE_WALKING;
		GetEntityManager()->GetEventManager()->Trigger(&event);
	}
	else if (physics->wasWalking && !physics->isWalking)
	{
		event.state = ENTITYSTATE_IDLE;
		GetEntityManager()->GetEventManager()->Trigger(&event);
	}
}

BOOL PhysicsSystem::Handle(const Event *event)
{
	STACK_TRACE;
	if (event->Is<MoveInDirectionEvent>())
	{
		const MoveInDirectionEvent *e = event->As<MoveInDirectionEvent>();
		if (e->GetEntity()->Has<PhysicsComponent>())
		{
			Move(e->GetEntity(), e->direction);
			return TRUE;
		}
	}
	if (event->Is<MoveForwardEvent>())
	{
		const MoveForwardEvent *e = event->As<MoveForwardEvent>();
		if (e->GetEntity()->Has<PhysicsComponent>())
		{
			OrientationXZComponent *orientation = e->GetEntity()->Get<OrientationXZComponent>();
			ASSERT(orientation != NULL);
			Move(e->GetEntity(), GetDirectionFromYAxisOrientation(orientation->angle));
			return TRUE;
		}
	}
	if (event->Is<StopMovementEvent>())
	{
		const StopMovementEvent *e = event->As<StopMovementEvent>();
		if (e->GetEntity()->Has<PhysicsComponent>())
		{
			Stop(e->GetEntity());
			return TRUE;
		}
	}
	else if (event->Is<JumpEvent>())
	{
		const JumpEvent *e = event->As<JumpEvent>();
		if (e->GetEntity()->Has<PhysicsComponent>())
		{
			Jump(e->GetEntity());
			return TRUE;
		}
	}

	return FALSE;
}

void PhysicsSystem::Move(Entity *entity, const Vector3 &direction)
{
	STACK_TRACE;
	PhysicsComponent *physics = entity->Get<PhysicsComponent>();
	ASSERT(physics != NULL);

	AffectedByTerrainFrictionComponent *affectedByTerrainFriction = entity->Get<AffectedByTerrainFrictionComponent>();
	TerrainFrictionValues frictionValues = GetTerrainFrictionFor(physics, affectedByTerrainFriction);

	physics->walkingVelocity += (direction * frictionValues.walkAcceleration);
}

void PhysicsSystem::Stop(Entity *entity)
{
	STACK_TRACE;
	PhysicsComponent *physics = entity->Get<PhysicsComponent>();
	ASSERT(physics != NULL);
	physics->velocity = ZERO_VECTOR;
	physics->walkingVelocity = ZERO_VECTOR;
}

void PhysicsSystem::Jump(Entity *entity)
{
	STACK_TRACE;
	PhysicsComponent *physics = entity->Get<PhysicsComponent>();
	ASSERT(physics != NULL);

	if (!physics->isOnGround)
		return;

	Force *newForce = new JumpForce(UP, GRAVITY_SPEED * 3.5f, 0.95f, physics);
	ApplyForce(physics, newForce);
}

void PhysicsSystem::ApplyForce(PhysicsComponent *physics, Force *force)
{
	STACK_TRACE;
	physics->forces.push_back(force);
}

void PhysicsSystem::ApplyForce(PhysicsComponent *physics, const Vector3 &direction, float strength, float friction)
{
	STACK_TRACE;
	Force *newForce = new Force(direction, strength, friction);
	ApplyForce(physics, newForce);
}

void PhysicsSystem::ProcessForces(PhysicsComponent *physics, float delta)
{
	STACK_TRACE;
	ForceList::iterator i;
	for (i = physics->forces.begin(); i != physics->forces.end(); ++i)
	{
		Force *force = *i;
		force->Update(delta);
		physics->forceVelocity += force->GetCurrentForce();
	}

	// clean up dead forces
	i = physics->forces.begin();
	while (i != physics->forces.end())
	{
		Force *force = *i;
		if (!force->IsActive())
		{
			physics->forces.erase(i++);
			SAFE_DELETE(force);
		}
		else
			++i;
	}
}

void PhysicsSystem::PushIntersectingEntities(Entity *pusher)
{
	STACK_TRACE;
	PusherComponent *pusherInfo = pusher->Get<PusherComponent>();
	ASSERT(pusherInfo != NULL);
	BoundingSphereComponent *pusherBounds = pusher->Get<BoundingSphereComponent>();
	ASSERT(pusherBounds != NULL);
	PhysicsComponent *pusherPhysics = pusher->Get<PhysicsComponent>();
	PositionComponent *pusherPosition = pusher->Get<PositionComponent>();
	
	EntityList list;
	GetEntityManager()->GetAllWith<PushableComponent>(list);
	for (EntityList::iterator i = list.begin(); i != list.end(); ++i)
	{
		Entity *pushee = *i;
		PushableComponent *pushableInfo = pushee->Get<PushableComponent>();
		
		PositionComponent *pusheePosition = pushee->Get<PositionComponent>();
		ASSERT(pusheePosition != NULL);
		BoundingSphereComponent *pusheeBounds = pushee->Get<BoundingSphereComponent>();
		ASSERT(pusheeBounds != NULL);
		
		// are the pusher and potential-pushee colliding?
		if (IntersectionTester::Test(pusherBounds->bounds, pusheeBounds->bounds))
		{
			// handle pushing the pushee out of the way by the pusher first ...
			float pushStrength = fabs(Vector3::Distance(pusheePosition->position, pusherPosition->position));
			Vector3 directionToPush = Vector3::Normalize(pusheePosition->position - pusherPosition->position);
			
			// adjust push strength by the pusher's "push strength"
			pushStrength *= pusherInfo->strength;
			
			// add push force to pushee
			PhysicsComponent *pusheePhysics = pushee->Get<PhysicsComponent>();
			ASSERT(pusheePhysics != NULL);
			Force *push = new Force(directionToPush, pushStrength, FRICTION_NORMAL);
			pusheePhysics->forces.push_back(push);
			

			// now handle the reactionary "push-back" force to the pusher
			float pushBackStrength = pushStrength;
			Vector3 directionToPushBack = -directionToPush;
			
			// adjust pushback strength by the pushee's "push resistance"
			pushBackStrength *= pushableInfo->resistance;
			
			// don't bother pushing back if there's no strength...
			if (!IsCloseEnough(pushBackStrength, 0.0f))
			{
				const float PUSHBACK_FRICTION = 0.1f;  // TODO: seems like a hack...
				Force *pushBack = new Force(directionToPushBack, pushBackStrength, PUSHBACK_FRICTION);
				pusherPhysics->forces.push_back(pushBack);
			}
		}
	}
}

void PhysicsSystem::UpdateIsWalkingState(PhysicsComponent *physics)
{
	STACK_TRACE;
	physics->wasWalking = physics->isWalking;

	if (!physics->isOnGround)
	{
		physics->isWalking = FALSE;
		return;
	}

	Vector3 temp = physics->walkingVelocity;
	temp.y = 0.0f;   // walking only allows the entity to get a velocity in the XZ plane

	float velocityLength = Vector3::Length(temp);
	if (IsCloseEnough(velocityLength, 0.0f))
		physics->isWalking = FALSE;
	else
		physics->isWalking = TRUE;
}

void PhysicsSystem::UpdateIsInMotionState(PhysicsComponent *physics)
{
	STACK_TRACE;
	physics->wasInMotion = physics->isInMotion;

	float velocityLength = Vector3::Length(physics->velocity);
	if (IsCloseEnough(velocityLength, 0.0f))
		physics->isInMotion = FALSE;
	else
		physics->isInMotion = TRUE;
}

void PhysicsSystem::UpdateIsFallingState(PhysicsComponent *physics, PositionComponent *position)
{
	STACK_TRACE;
	float currentYPosition = position->position.y;

	if (physics->isFalling)
		physics->wasFalling = TRUE;
	else
		physics->wasFalling = FALSE;

	if (physics->isOnGround)
	{
		// not falling anymore, total fall distance will be recorded for the
		// rest of this tick, but will be cleared next tick when this runs again
		physics->isFalling = FALSE;
		physics->fallDistance = physics->currentFallDistance;
		physics->currentFallDistance = 0.0f;
	}
	else if (currentYPosition < physics->lastYPosition)
	{
		// falling (current Y coord is lower then the one from last tick)
		physics->isFalling = TRUE;
		physics->currentFallDistance += (physics->lastYPosition - currentYPosition);
	}

	physics->lastYPosition = currentYPosition;
}

void PhysicsSystem::UpdateIsOnGroundState(PhysicsComponent *physics)
{
	STACK_TRACE;
	physics->wasOnGround = physics->isOnGround;
	if (physics->collisionPacket.foundCollision && IsCloseEnough(physics->velocity.y, 0.0f, ON_GROUND_ZERO_TOLERANCE))
		physics->isOnGround = TRUE;
	else
		physics->isOnGround = FALSE;
}

void PhysicsSystem::UpdateStandingTileCoords(PhysicsComponent *physics, PositionComponent *position, WorldComponent *world)
{
	STACK_TRACE;
	if (physics->isOnGround)
	{;
		uint32_t feetX = (uint32_t)position->position.x;
		uint32_t feetY = (uint32_t)(position->position.y - physics->collisionPacket.ellipsoidRadius.y - 0.01f);
		uint32_t feetZ = (uint32_t)position->position.z;

		// prefer to take the tile just below the entity's feet
		// however, if that tile is empty, take the collision tile
		// (this can sometimes work better, as the collision tile could
		// be a tile that the entity is just standing on the very edge of, but
		// in fact be mostly standing on an adjacent tile... which could be the
		// one we calculate to be under the entity's feet)

		physics->standingOnTile = world->world->Get(feetX, feetY, feetZ);
		if (physics->standingOnTile->tile != NO_TILE)
		{
			// check the distance from the entity's feet to the nearest 
			// collidable surface on this tile before we accept it as the
			// "standing on" tile
			Vector3 feetPosition = position->position;
			feetPosition.y -= physics->collisionPacket.ellipsoidRadius.y;
			Ray down(feetPosition, DOWN);
			Vector3 collisionPoint;
			float collisionDistance = 0.0f;
			if (world->world->CheckForCollisionWithTile(down, collisionPoint, feetX, feetY, feetZ))
				collisionDistance = Vector3::Length(feetPosition - collisionPoint);

			// TODO: this distance check causes issues with ramps. as you walk
			//       up/down the ramp, the "standing tile" tends to flip back
			//       and forth a bit depending on how far up or down the ramp
			//       you are from the top, assuming the top of the ramp is 
			//       next to a flat surface
			if (collisionDistance <= 0.1f)
			{
				// the top of the foot tile is close enough, use it
				physics->standingOnTilePosition.x = feetX;
				physics->standingOnTilePosition.y = feetY;
				physics->standingOnTilePosition.z = feetZ;
				return;
			}
		}

		// use the collision tile
		physics->standingOnTile = world->world->Get(physics->collisionTilePosition.x, physics->collisionTilePosition.y, physics->collisionTilePosition.z);
		physics->standingOnTilePosition.x = physics->collisionTilePosition.x;
		physics->standingOnTilePosition.y = physics->collisionTilePosition.y;
		physics->standingOnTilePosition.z = physics->collisionTilePosition.z;
	}
	else
	{
		// not standing on anything
		physics->standingOnTilePosition.x = 0;
		physics->standingOnTilePosition.y = 0;
		physics->standingOnTilePosition.z = 0;
		physics->standingOnTile = NULL;
	}
}

void PhysicsSystem::UpdatePossibleCollisionArea(PhysicsComponent *physics, PositionComponent *position)
{
	STACK_TRACE;
	Vector3 newPosition = position->position + physics->velocity;
	const Vector3 &radius = physics->collisionPacket.ellipsoidRadius;

	// we +/- 0.5f to the y coord to ensure we check tiles below/above
	// so that things like gravity checks work better

	physics->possibleCollisionArea.min.x = Min(position->position.x, newPosition.x) - radius.x;
	physics->possibleCollisionArea.min.y = Min(position->position.y, newPosition.y) - radius.y - 0.5f;
	physics->possibleCollisionArea.min.z = Min(position->position.z, newPosition.z) - radius.z;

	physics->possibleCollisionArea.max.x = Max(position->position.x, newPosition.x) + radius.x;
	physics->possibleCollisionArea.max.y = Max(position->position.y, newPosition.y) + radius.y + 0.5f;
	physics->possibleCollisionArea.max.z = Max(position->position.z, newPosition.z) + radius.z;
}

Vector3 PhysicsSystem::ProcessMovement(PhysicsComponent *physics, PositionComponent *position, WorldComponent *world, float delta)
{
	STACK_TRACE;
	Vector3 resultingVelocity = ZERO_VECTOR;

	// don't attempt to process movement if the entity is not moving!
	if (Vector3::LengthSquared(physics->velocity) > 0.0f)
	{
		// convert position and velocity to ellipsoid space
		Vector3 esPosition = position->position / physics->collisionPacket.ellipsoidRadius;
		Vector3 esVelocity = physics->velocity / physics->collisionPacket.ellipsoidRadius;

		// check for and respond to any collisions along this velocity vector
		physics->collisionRecursionDepth = 0;
		physics->nearestIntersection = ZERO_VECTOR;
		physics->collisionPacket.nearestDistance = 0.0f;
		physics->collisionPacket.foundCollision = FALSE;
		physics->collisionPacket.esIntersectionPoint = ZERO_VECTOR;
		Vector3 newEsPosition = GetNewPositionForMovement(physics, world, esPosition, esVelocity, resultingVelocity, TRUE);

		// resulting velocity will have been calculated in ellipsoid space
		resultingVelocity *= physics->collisionPacket.ellipsoidRadius;

		if (physics->collisionPacket.foundCollision)
			physics->nearestIntersection = physics->collisionPacket.esIntersectionPoint * physics->collisionPacket.ellipsoidRadius;

		// convert the new position back to normal space and move the entity there
		position->position.Set(newEsPosition * physics->collisionPacket.ellipsoidRadius);
	}

	return resultingVelocity;
}

Vector3 PhysicsSystem::ProcessGravity(PhysicsComponent *physics, PositionComponent *position, WorldComponent *world, float delta)
{
	STACK_TRACE;
	Vector3 resultingVelocity = ZERO_VECTOR;

	// prepare a gravity velocity vector
	Vector3 gravity = GRAVITY * delta;

	// convert position and gravity velocity vector to ellipsoid space
	Vector3 esPosition = position->position / physics->collisionPacket.ellipsoidRadius;
	Vector3 esVelocity = gravity / physics->collisionPacket.ellipsoidRadius;

	// check for and respond to any collisions along this velocity vector
	physics->collisionRecursionDepth = 0;
	physics->nearestIntersection = ZERO_VECTOR;
	physics->collisionPacket.nearestDistance = 0.0f;
	physics->collisionPacket.foundCollision = FALSE;
	physics->collisionPacket.esIntersectionPoint = ZERO_VECTOR;
	Vector3 newEsPosition = GetNewPositionForMovement(physics, world, esPosition, esVelocity, resultingVelocity, TRUE, TRUE, SLOPE_STEEP_Y_ANGLE);

	// resulting velocity will have been calculated in ellipsoid space
	resultingVelocity *= physics->collisionPacket.ellipsoidRadius;

	if (physics->collisionPacket.foundCollision)
		physics->nearestIntersection = physics->collisionPacket.esIntersectionPoint * physics->collisionPacket.ellipsoidRadius;

	// convert the new position back to normal space and move the entity there
	position->position.Set(newEsPosition * physics->collisionPacket.ellipsoidRadius);

	return resultingVelocity;
}

Vector3 PhysicsSystem::GetNewPositionForMovement(PhysicsComponent *physics, WorldComponent *world, const Vector3 &position, const Vector3 &velocity, Vector3 &responseVelocity, BOOL canSlide, BOOL onlySlideIfTooSteep, float steepYAngle)
{
	STACK_TRACE;
	const float VERY_CLOSE_DISTANCE = 0.005f;
	const uint32_t MAX_RECURSION_DEPTH = 5;

	// don't recurse too much
	if (physics->collisionRecursionDepth > MAX_RECURSION_DEPTH)
		return position;

	responseVelocity = velocity;

	// set up the collision check information
	physics->collisionPacket.esVelocity = velocity;
	physics->collisionPacket.esNormalizedVelocity = Vector3::Normalize(velocity);
	physics->collisionPacket.esPosition = position;
	physics->collisionPacket.foundCollision = FALSE;

	// perform the check
	CheckForCollisionsWithWorld(physics, world);

	// if there was no collision, simply move along the velocity vector
	if (!physics->collisionPacket.foundCollision)
		return position + velocity;

	// a collision did occur

	Vector3 destination = position + velocity;
	Vector3 newPosition = position;

	if (physics->collisionPacket.nearestDistance >= VERY_CLOSE_DISTANCE)
	{
		// we haven't yet moved up too close to the nearest collision, so
		// let's inch forward a bit

		// figure out the new position that we need to move up to
		float moveUpLength = physics->collisionPacket.nearestDistance - VERY_CLOSE_DISTANCE;

		// HACK: if the above length ends up being 0, "v" calculated below will
		//       end up with "NaN" x/y/z components which will eventually cause 
		//       the resulting position from all this being "NaN" and the entity
		//       will seem to disappear entirely. If we catch this zero length
		//       condition and recalculate it so that the length is non-zero but
		//       still very small (below the VERY_CLOSE_DISTANCE threshold) then
		//       it appears to work fine.
		if (moveUpLength == 0.0f)
			moveUpLength = physics->collisionPacket.nearestDistance - (VERY_CLOSE_DISTANCE * 0.5f);

		Vector3 v = Vector3::SetLength(velocity, moveUpLength);
		newPosition = physics->collisionPacket.esPosition + v;

		// adjust the polygon intersection point, so the sliding plane will be
		// unaffected by the fact that we move slightly less than the collision
		// tells us
		v = Vector3::Normalize(v);
		physics->collisionPacket.esIntersectionPoint -= (v * VERY_CLOSE_DISTANCE);
	}

	if (!canSlide)
	{
		responseVelocity = ZERO_VECTOR;
		return newPosition;
	}

	// we can slide, so determine the sliding plane
	Vector3 slidePlaneOrigin = physics->collisionPacket.esIntersectionPoint;
	Vector3 slidePlaneNormal = Vector3::Normalize(newPosition - physics->collisionPacket.esIntersectionPoint);
	Plane slidingPlane(slidePlaneOrigin, slidePlaneNormal);

	// determine slide angle and then check if we need to bail out on sliding
	// depending on how steep the slide plane is
	physics->slidingPlaneNormal = Ray(slidePlaneOrigin * physics->collisionPacket.ellipsoidRadius, slidePlaneNormal);
	float slidingYAngle = acosf(Vector3::Dot(slidePlaneNormal, Y_AXIS));

	if (onlySlideIfTooSteep && slidingYAngle < steepYAngle)
	{
		responseVelocity = ZERO_VECTOR;
		return newPosition;
	}

	Vector3 newDestination = destination - (slidePlaneNormal * Plane::DistanceBetween(slidingPlane, destination));

	// generate the slide vector, which will become our new velocity vector
	// for the next iteration
	physics->isSliding = TRUE;
	Vector3 newVelocity = newDestination - physics->collisionPacket.esIntersectionPoint;
	responseVelocity = newVelocity;

	// don't recurse if the velocity is very small
	if (Vector3::Length(newVelocity) < VERY_CLOSE_DISTANCE)
		return newPosition;

	// recurse
	++physics->collisionRecursionDepth;
	return GetNewPositionForMovement(physics, world, newPosition, newVelocity, responseVelocity, canSlide, onlySlideIfTooSteep, steepYAngle);
}

void PhysicsSystem::CheckForCollisionsWithWorld(PhysicsComponent *physics, WorldComponent *world)
{
	STACK_TRACE;
	uint32_t x1 = 0;
	uint32_t y1 = 0;
	uint32_t z1 = 0;
	uint32_t x2 = 0;
	uint32_t y2 = 0;
	uint32_t z2 = 0;

	float lastCollisionDistance = FLT_MAX;

	// TODO: I don't think we even need to check if the area overlaps ... ?
	//       (it probably always will, right?)
	BOOL overlapping = world->world->GetOverlappedTiles(physics->possibleCollisionArea, x1, y1, z1, x2, y2, z2);
	if (overlapping)
	{
		for (uint32_t y = y1; y <= y2; ++y)
		{
			for (uint32_t z = z1; z < z2; ++z)
			{
				for (uint32_t x = x1; x < x2; ++x)
				{
					Tile *tile = world->world->Get(x, y, z);

					// only check solid tiles
					if (IsBitSet(TILE_COLLIDABLE, tile->flags))
					{
						// check each triangle in this tile's mesh
						const TileMesh *mesh = world->world->GetMeshes()->Get(tile);
						const Vector3 *vertices = mesh->GetCollisionVertices();

						// world position of this tile, will be used to move each
						// mesh triangle into world space
						Vector3 tileWorldPosition = Vector3((float)x, (float)y, (float)z);

						// also add the global TileMesh offset so the mesh is within
						// 0,0,0 to 1,1,1 and not -0.5,-0.5,-0.5 to 0.5,0.5,0.5
						tileWorldPosition += TILEMESH_OFFSET;

						const Matrix4x4 *transform = tile->GetTransformationMatrix();

						for (uint32_t i = 0; i < mesh->GetNumCollisionVertices(); i += 3)
						{
							// get the vertices making up this triangle
							Vector3 a = vertices[i];
							Vector3 b = vertices[i + 1];
							Vector3 c = vertices[i + 2];

							if (transform != NULL)
							{
								a = a * (*transform);
								b = b * (*transform);
								c = c * (*transform);
							}

							// move these vertices into world space
							a += tileWorldPosition;
							b += tileWorldPosition;
							c += tileWorldPosition;

							// test against it. we don't actually care about the
							// return value, but just want the CollisionPacket 
							// to be updated. at the end of all these tests, it 
							// will contain info for the closest intersection 
							// found
							BOOL collided = IntersectionTester::Test(physics->collisionPacket, a, b, c);
							if (collided && physics->collisionPacket.nearestDistance < lastCollisionDistance)
							{
								// this is the closest collision found so far
								// record the grid position ...
								physics->collisionTilePosition.x = x;
								physics->collisionTilePosition.y = y;
								physics->collisionTilePosition.z = z;

								// ... and the triangle
								physics->hitTriangle[0] = a;
								physics->hitTriangle[1] = b;
								physics->hitTriangle[2] = c;
							}
						}
					}
				}
			}
		}
	}
}

TerrainFrictionValues PhysicsSystem::GetTerrainFrictionFor(PhysicsComponent *physics, AffectedByTerrainFrictionComponent *affectedByTerrainFriction)
{
	STACK_TRACE;
	TerrainFrictionValues result;
	result.friction = physics->friction;
	result.walkAcceleration = physics->walkingAcceleration;
	result.maxWalkSpeed = physics->maxWalkSpeed;

	if (affectedByTerrainFriction != NULL && physics->isOnGround)
	{
		ASSERT(physics->standingOnTile != NULL);
		if (IsBitSet(TILE_FRICTION_SLIPPERY, physics->standingOnTile->flags))
		{
			result.friction = 0.95f;
			result.walkAcceleration = 0.5f;
			result.maxWalkSpeed = 2.0f;
		}
	}

	return result;
}

