#include "../../../../framework/debug.h"

#include "bulletpreset.h"
#include "../../components/bulletcomponent.h"
#include "../../components/boundingspherecomponent.h"
#include "../../components/orientationxzcomponent.h"
#include "../../components/simplephysicscomponent.h"
#include "../../components/positioncomponent.h"
#include "../../events/bulletcollisionevent.h"
#include "../../../../entities/entity.h"
#include "../../../../entities/entitymanager.h"
#include "../../../../events/event.h"
#include "../../../../events/eventlistenerex.h"
#include "../../../../events/eventmanager.h"

BulletPreset::BulletPreset(EntityManager *entityManager)
	: EntityPreset(entityManager),
	  EventListenerEx(entityManager->GetEventManager())
{
	STACK_TRACE;
	ListenFor<BulletCollisionEvent>();
}

BulletPreset::~BulletPreset()
{
	STACK_TRACE;
	StopListeningFor<BulletCollisionEvent>();
}

Entity* BulletPreset::Create(EntityPresetArgs *args)
{
	STACK_TRACE;
	Entity *entity = GetEntityManager()->Add();

	entity->Add<BulletComponent>();

	entity->Add<BoundingSphereComponent>()
		->bounds.radius = 0.25f;

	entity->Add<OrientationXZComponent>();

	SimplePhysicsComponent *physics = entity->Add<SimplePhysicsComponent>();
	physics->speed = 10.0f;
	physics->SetBoundsRadius(0.25f);

	entity->Add<PositionComponent>();

	return entity;
}

BOOL BulletPreset::Handle(const Event *event)
{
	STACK_TRACE;
	if (event->Is<BulletCollisionEvent>())
	{
		const BulletCollisionEvent *e = event->As<BulletCollisionEvent>();
		if (e->GetEntity()->WasCreatedUsingPreset(GetTypeOf()))
			OnCollision(e->GetEntity(), e->collisionEntity);
	}

	return FALSE;
}
