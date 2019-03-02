#include "../../../framework/debug.h"

#include "offseteffectsystem.h"
#include "../components/collectedpowerupcomponent.h"
#include "../components/colorcomponent.h"
#include "../components/floatbouncecomponent.h"
#include "../components/positioncomponent.h"
#include "../components/positionoffsetcomponent.h"
#include "../components/renderoffsetcomponent.h"
#include "../../../entities/entity.h"
#include "../../../entities/entitymanager.h"
#include "../../../entities/components/inactivecomponent.h"
#include "../../../framework/graphics/color.h"
#include "../../../framework/math/mathhelpers.h"
#include "../../../framework/math/vector3.h"

OffsetEffectSystem::OffsetEffectSystem(EntityManager *entityManager, EventManager *eventManager)
	: ComponentSystem(entityManager, eventManager)
{
	STACK_TRACE;
}

OffsetEffectSystem::~OffsetEffectSystem()
{
	STACK_TRACE;
}

void OffsetEffectSystem::OnUpdate(float delta)
{
	STACK_TRACE;
	EntityList list;

	GetEntityManager()->GetAllWith<PositionOffsetComponent>(list);
	for (EntityList::iterator i = list.begin(); i != list.end(); ++i)
	{
		Entity *entity = *i;
		PositionOffsetComponent *positionOffset = entity->Get<PositionOffsetComponent>();

		// reset
		positionOffset->offset = ZERO_VECTOR;

		// check for and add offsets from any components that manipulate offsets
		// TODO: add code for newer offset-manipulating components here

		FloatBounceComponent *floatBounce = entity->Get<FloatBounceComponent>();
		if (floatBounce != NULL)
			HandleFloatBounce(positionOffset, floatBounce, delta);

		CollectedPowerUpComponent *collectedPowerUp = entity->Get<CollectedPowerUpComponent>();
		if (collectedPowerUp != NULL)
			HandleCollectedPowerUp(positionOffset, collectedPowerUp, entity, delta);
	}

	// Why were we doing this? Commented out for now to make KeyframeMesh rendering
	// work with models which require a constant render offset (due to the model's
	// origin being placed at the model's feet instead of the center)
	/*
	list.clear();
	GetEntityManager()->GetAllWith<RenderOffsetComponent>(list);
	for (EntityList::iterator i = list.begin(); i != list.end(); ++i)
	{
		Entity *entity = *i;
		RenderOffsetComponent *renderOffset = entity->Get<RenderOffsetComponent>();

		// reset
		renderOffset->offset = ZERO_VECTOR;

		// TODO: currently there aren't any components doing anything with this
	}
	*/
}

void OffsetEffectSystem::HandleFloatBounce(PositionOffsetComponent *positionOffset, FloatBounceComponent *floatBounce, float delta)
{
	STACK_TRACE;
	if (floatBounce->isMovingUp)
	{
		floatBounce->position += delta;
		if (floatBounce->position >= 1.0f)
		{
			floatBounce->position = 1.0f;
			floatBounce->isMovingUp = FALSE;
		}
	}
	else
	{
		floatBounce->position -= delta;
		if (floatBounce->position <= 0.0f)
		{
			floatBounce->position = 0.0f;
			floatBounce->isMovingUp = TRUE;
		}
	}

	// convert into a Y distance, and add to offset
	float bounceAmountY = SmoothStep(floatBounce->minY, floatBounce->maxY, floatBounce->position);
	positionOffset->offset.y += bounceAmountY;
}

void OffsetEffectSystem::HandleCollectedPowerUp(PositionOffsetComponent *positionOffset, CollectedPowerUpComponent *collectedPowerUp, Entity *entity, float delta)
{
	STACK_TRACE;
	const float FADEOUT_SPEED = 3.0f;
	const float TIME_TO_RISE_UP = 1.0f;
	const float TIME_TO_FLOAT_BEFORE_FADING = 1.0f;

	if (collectedPowerUp->riseTime < TIME_TO_RISE_UP)
	{
		collectedPowerUp->riseTime += delta;
	}
	else
	{
		if (collectedPowerUp->postRiseFloatTime >= TIME_TO_FLOAT_BEFORE_FADING)
		{
			if (collectedPowerUp->alpha > 0.0f)
			{
				collectedPowerUp->alpha -= delta * FADEOUT_SPEED;
				ColorComponent *color = entity->Get<ColorComponent>();
				if (color != NULL)
					color->color.a = collectedPowerUp->alpha;
			}
			else
				entity->Add<InactiveComponent>();
		}
		else
			collectedPowerUp->postRiseFloatTime += delta;
	}

	float t = collectedPowerUp->riseTime / TIME_TO_RISE_UP;
	float floatY = SmoothStep(collectedPowerUp->minFloatY, collectedPowerUp->maxFloatY, t);
	positionOffset->offset.y += floatY;
}

