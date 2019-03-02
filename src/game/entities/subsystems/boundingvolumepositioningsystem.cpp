#include "../../../framework/debug.h"

#include "boundingvolumepositioningsystem.h"
#include "../components/boundingspherecomponent.h"
#include "../components/positioncomponent.h"
#include "../components/positionoffsetcomponent.h"
#include "../../../entities/entity.h"
#include "../../../entities/entitymanager.h"
#include "../../../framework/math/boundingbox.h"
#include "../../../framework/math/boundingsphere.h"
#include "../../../framework/math/vector3.h"

BoundingVolumePositioningSystem::BoundingVolumePositioningSystem(EntityManager *entityManager, EventManager *eventManager)
	: ComponentSystem(entityManager, eventManager)
{
	STACK_TRACE;
}

BoundingVolumePositioningSystem::~BoundingVolumePositioningSystem()
{
	STACK_TRACE;
}

void BoundingVolumePositioningSystem::OnUpdate(float delta)
{
	STACK_TRACE;
	EntityList list;
	GetEntityManager()->GetAllWith<BoundingSphereComponent>(list);
	for (EntityList::iterator i = list.begin(); i != list.end(); ++i)
	{
		Entity *entity = *i;
		BoundingSphereComponent *bounds = entity->Get<BoundingSphereComponent>();

		PositionComponent *position = entity->Get<PositionComponent>();
		ASSERT(position != NULL);

		PositionOffsetComponent *positionOffset = entity->Get<PositionOffsetComponent>();

		Vector3 boundsCenter = position->position;
		if (positionOffset != NULL)
			boundsCenter += positionOffset->offset;

		bounds->bounds.center = boundsCenter;
	}

	// TODO: add similar code for other types of bounding volume components here
}

