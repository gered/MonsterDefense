#include "../../../framework/debug.h"
#include "../../../framework/common.h"

#include "bulletsystem.h"
#include "../components/boundingspherecomponent.h"
#include "../components/bulletcomponent.h"
#include "../components/canbeattackedcomponent.h"
#include "../components/hurtcooldowncomponent.h"
#include "../components/positioncomponent.h"
#include "../components/simplephysicscomponent.h"
#include "../events/bulletcollisionevent.h"
#include "../events/entitystatechangeevent.h"
#include "../events/hurtevent.h"
#include "../events/moveforwardevent.h"
#include "../presets/particles/particlepresetproperties.h"
#include "../presets/particles/sparkparticlepreset.h"
#include "../../../entities/entity.h"
#include "../../../entities/entitymanager.h"
#include "../../../entities/components/inactivecomponent.h"
#include "../../../events/event.h"
#include "../../../events/eventmanager.h"
#include "../../../framework/math/intersectiontester.h"

BulletSystem::BulletSystem(EntityManager *entityManager, EventManager *eventManager)
	: ComponentSystem(entityManager, eventManager)
{
	STACK_TRACE;
	ListenFor<BulletCollisionEvent>();
}

BulletSystem::~BulletSystem()
{
	STACK_TRACE;
	StopListeningFor<BulletCollisionEvent>();
}

void BulletSystem::OnUpdate(float delta)
{
	STACK_TRACE;
	EntityList list;
	
	// update bullets
	list.clear();
	GetEntityManager()->GetAllWith<BulletComponent>(list);
	for (EntityList::iterator i = list.begin(); i != list.end(); ++i)
	{
		Entity *bulletEntity = *i;

		// check if bullet has collided with the environment
		SimplePhysicsComponent *physics = bulletEntity->Get<SimplePhysicsComponent>();
		ASSERT(physics != NULL);
		if (physics->collisionPacket.foundCollision)
		{
			BulletCollisionEvent *collisionEvent = new BulletCollisionEvent(bulletEntity);
			GetEventManager()->Queue(collisionEvent);

			// no more processing for this bullet now
			continue;
		}

		// check if bullet has collided with another entity
		BulletComponent *bullet = bulletEntity->Get<BulletComponent>();
		BoundingSphereComponent *bulletBounds = bulletEntity->Get<BoundingSphereComponent>();
		ASSERT(bulletBounds != NULL);
		
		EntityList potentialVictims;
		GetEntityManager()->GetAllWith<CanBeAttackedComponent>(potentialVictims);

		BOOL collided = FALSE;

		for (EntityList::iterator j = potentialVictims.begin(); j != potentialVictims.end(); ++j)
		{
			Entity *potentialVictim = *j;
			
			// can't hurt them if they have this type of cooldown, so we can
			// skip an intersection test by testing for this component first...
			HurtCooldownComponent *hurtCooldown = potentialVictim->Get<HurtCooldownComponent>();
			if (hurtCooldown != NULL)
				continue;
			
			CanBeAttackedComponent *canBeAttacked = potentialVictim->Get<CanBeAttackedComponent>();
			if (canBeAttacked->byPlayer == FALSE && bullet->isOwnedByPlayer == TRUE)
				continue;
			if (canBeAttacked->byNPC == FALSE && bullet->isOwnedByNPC == TRUE)
				continue;

			// now we need to do an intersection test
			BoundingSphereComponent *potentialVictimBounds = potentialVictim->Get<BoundingSphereComponent>();
			ASSERT(potentialVictimBounds != NULL);
			if (IntersectionTester::Test(bulletBounds->bounds, potentialVictimBounds->bounds))
			{
				// bullet collided with this victim
				// hurt them + raise bullet collision event

				HurtEvent *hurtEvent = new HurtEvent(potentialVictim);
				hurtEvent->usePercent = bullet->usePercentDamageAmount;
				hurtEvent->amount = bullet->flatDamageAmount;
				hurtEvent->percent = bullet->percentDamageAmount;
				hurtEvent->source = bullet->source;
				GetEventManager()->Queue(hurtEvent);

				BulletCollisionEvent *collisionEvent = new BulletCollisionEvent(bulletEntity);
				collisionEvent->collisionEntity = potentialVictim;
				GetEventManager()->Queue(collisionEvent);

				collided = TRUE;
				break;
			}
		}

		// if this bullet had a collision, don't do any more updates with it
		if (collided)
			continue;

		// nothing has gotten in the bullet's way, so continue moving it
		MoveForwardEvent *moveEvent = new MoveForwardEvent(bulletEntity);
		GetEventManager()->Queue(moveEvent);
	}
}

BOOL BulletSystem::Handle(const Event *event)
{
	STACK_TRACE;
	if (event->Is<BulletCollisionEvent>())
	{
		const BulletCollisionEvent *e = event->As<BulletCollisionEvent>();

		// destroy the bullet
		e->GetEntity()->Add<InactiveComponent>();
	}
	
	return FALSE;
}
