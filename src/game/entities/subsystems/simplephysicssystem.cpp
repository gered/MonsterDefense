#include "../../../framework/debug.h"

#include "simplephysicssystem.h"
#include "../components/orientationxzcomponent.h"
#include "../components/simplephysicscomponent.h"
#include "../components/positioncomponent.h"
#include "../components/removephysicswhenstillcomponent.h"
#include "../events/entitystatechangeevent.h"
#include "../events/moveforwardevent.h"
#include "../events/moveindirectionevent.h"
#include "../events/stopmovementevent.h"
#include "../globalcomponents/worldcomponent.h"
#include "../../../entities/entity.h"
#include "../../../entities/entitymanager.h"
#include "../../../events/event.h"
#include "../../../events/eventmanager.h"
#include "../../../framework/math/boundingbox.h"
#include "../../../framework/math/intersectiontester.h"
#include "../../../framework/math/mathhelpers.h"
#include "../../../framework/math/matrix4x4.h"
#include "../../../framework/math/vector3.h"
#include "../../../tilemap/tile.h"
#include "../../../tilemap/tilemap.h"
#include "../../../tilemap/tilemesh.h"
#include "../../../tilemap/tilemeshcollection.h"

SimplePhysicsSystem::SimplePhysicsSystem(EntityManager *entityManager, EventManager *eventManager)
	: ComponentSystem(entityManager, eventManager)
{
	STACK_TRACE;
	ListenFor<MoveForwardEvent>();
	ListenFor<MoveInDirectionEvent>();
	ListenFor<StopMovementEvent>();
}

SimplePhysicsSystem::~SimplePhysicsSystem()
{
	STACK_TRACE;
	StopListeningFor<MoveForwardEvent>();
	StopListeningFor<MoveInDirectionEvent>();
	StopListeningFor<StopMovementEvent>();
}

void SimplePhysicsSystem::OnUpdate(float delta)
{
	STACK_TRACE;
	WorldComponent *world = GetEntityManager()->GetGlobalComponent<WorldComponent>();
	ASSERT(world != NULL);

	EntityList list;
	GetEntityManager()->GetAllWith<SimplePhysicsComponent>(list);
	for (EntityList::iterator i = list.begin(); i != list.end(); ++i)
	{
		Entity *entity = *i;
		SimplePhysicsComponent *physics = entity->Get<SimplePhysicsComponent>();

		PositionComponent *position = entity->Get<PositionComponent>();
		ASSERT(position != NULL);

		// amount we will try to move this tick
		physics->velocity = physics->nextMovement * delta;

		// figure out what section of the world we could possible collide with 
		// based on the entity's current position and velocity
		UpdatePossibleCollisionArea(physics, position);

		// handle movement very simply (just move up along velocity and check
		// if a collision happened... no sliding, gravity, etc.)
		physics->velocity = ProcessMovement(physics, position, world, delta);

		// FINAL thing to do -- check if we should remove the physics component
		float velocityLength = Vector3::Length(physics->velocity);
		if (entity->Has<RemovePhysicsWhenStillComponent>() && IsCloseEnough(velocityLength, 0.0f))
			entity->Remove<SimplePhysicsComponent>();
	}
}

BOOL SimplePhysicsSystem::Handle(const Event *event)
{
	STACK_TRACE;
	if (event->Is<MoveInDirectionEvent>())
	{
		const MoveInDirectionEvent *e = event->As<MoveInDirectionEvent>();
		if (e->GetEntity()->Has<SimplePhysicsComponent>())
		{
			Move(e->GetEntity(), e->direction);
			return TRUE;
		}
	}
	if (event->Is<MoveForwardEvent>())
	{
		const MoveForwardEvent *e = event->As<MoveForwardEvent>();
		if (e->GetEntity()->Has<SimplePhysicsComponent>())
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
		if (e->GetEntity()->Has<SimplePhysicsComponent>())
		{
			Stop(e->GetEntity());
			return TRUE;
		}
	}

	return FALSE;
}

void SimplePhysicsSystem::Move(Entity *entity, const Vector3 &direction)
{
	STACK_TRACE;
	SimplePhysicsComponent *physics = entity->Get<SimplePhysicsComponent>();
	ASSERT(physics != NULL);
	physics->nextMovement = direction * physics->speed;
}

void SimplePhysicsSystem::Stop(Entity *entity)
{
	STACK_TRACE;
	SimplePhysicsComponent *physics = entity->Get<SimplePhysicsComponent>();
	ASSERT(physics != NULL);
	physics->velocity = ZERO_VECTOR;
	physics->nextMovement = ZERO_VECTOR;
}

void SimplePhysicsSystem::UpdatePossibleCollisionArea(SimplePhysicsComponent *physics, PositionComponent *position)
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

Vector3 SimplePhysicsSystem::ProcessMovement(SimplePhysicsComponent *physics, PositionComponent *position, WorldComponent *world, float delta)
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
		physics->nearestIntersection = ZERO_VECTOR;
		physics->collisionPacket.nearestDistance = 0.0f;
		physics->collisionPacket.foundCollision = FALSE;
		physics->collisionPacket.esIntersectionPoint = ZERO_VECTOR;
		physics->collisionPacket.esVelocity = esVelocity;
		physics->collisionPacket.esNormalizedVelocity = Vector3::Normalize(esVelocity);
		physics->collisionPacket.esPosition = esPosition;
		physics->collisionPacket.foundCollision = FALSE;

		// perform the check
		CheckForCollisionsWithWorld(physics, world);

		Vector3 newEsPosition;

		// if there was no collision, simply move along the velocity vector
		if (!physics->collisionPacket.foundCollision)
		{
			newEsPosition = esPosition + esVelocity;
			resultingVelocity = physics->velocity;
		}
		else
		{
			newEsPosition = esPosition;
			resultingVelocity = ZERO_VECTOR;
		}

		if (physics->collisionPacket.foundCollision)
			physics->nearestIntersection = physics->collisionPacket.esIntersectionPoint * physics->collisionPacket.ellipsoidRadius;

		// convert the new position back to normal space and move the entity there
		position->position.Set(newEsPosition * physics->collisionPacket.ellipsoidRadius);
	}

	return resultingVelocity;
}

void SimplePhysicsSystem::CheckForCollisionsWithWorld(SimplePhysicsComponent *physics, WorldComponent *world)
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
