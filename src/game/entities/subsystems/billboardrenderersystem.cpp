#include "../../../framework/debug.h"

#include "../../../framework/common.h"
#include "billboardrenderersystem.h"
#include "../components/affectedbylightingcomponent.h"
#include "../components/billboardcomponent.h"
#include "../components/boundingspherecomponent.h"
#include "../components/colorcomponent.h"
#include "../components/notvisiblecomponent.h"
#include "../components/positioncomponent.h"
#include "../components/positionoffsetcomponent.h"
#include "../components/renderoffsetcomponent.h"
#include "../globalcomponents/worldcomponent.h"
#include "../../../entities/entity.h"
#include "../../../entities/entitymanager.h"
#include "../../../contexts/rendercontext.h"
#include "../../../framework/graphics/billboardspritebatch.h"
#include "../../../framework/graphics/color.h"
#include "../../../framework/graphics/graphicsdevice.h"
#include "../../../framework/graphics/texture.h"
#include "../../../framework/graphics/textureatlas.h"
#include "../../../framework/math/camera.h"
#include "../../../framework/math/frustum.h"
#include "../../../framework/math/vector3.h"
#include "../../../tilemap/tile.h"
#include "../../../tilemap/tilemap.h"

BillboardRendererSystem::BillboardRendererSystem(EntityManager *entityManager, EventManager *eventManager)
	: ComponentSystem(entityManager, eventManager)
{
	STACK_TRACE;
}

BillboardRendererSystem::~BillboardRendererSystem()
{
	STACK_TRACE;
}

void BillboardRendererSystem::OnRender(RenderContext *renderContext)
{
	STACK_TRACE;
	const Frustum *frustum = renderContext->GetGraphicsDevice()->GetCamera()->GetFrustum();
	BillboardSpriteBatch *billboardSpriteBatch = renderContext->GetBillboardSpriteBatch();

	WorldComponent *world = GetEntityManager()->GetGlobalComponent<WorldComponent>();
	ASSERT(world != NULL);

	EntityList list;
	GetEntityManager()->GetAllWith<BillboardComponent>(list);
	for (EntityList::iterator i = list.begin(); i != list.end(); ++i)
	{
		Entity *entity = *i;
		
		if (entity->Has<NotVisibleComponent>())
			continue;
		
		BillboardComponent *billboard = entity->Get<BillboardComponent>();
		PositionComponent *position = entity->Get<PositionComponent>();
		BoundingSphereComponent *bounds = entity->Get<BoundingSphereComponent>();
		PositionOffsetComponent *positionOffset = entity->Get<PositionOffsetComponent>();
		RenderOffsetComponent *renderOffset = entity->Get<RenderOffsetComponent>();
		ColorComponent *color = entity->Get<ColorComponent>();
		BOOL isAffectedByLighting = entity->Has<AffectedByLightingComponent>();

		ASSERT(position != NULL);

		if (bounds != NULL)
		{
			// don't do anything more if this entity isn't currently visible to the camera
			if (!frustum->Test(bounds->bounds))
				continue;
		}

		Vector3 renderPosition = position->position;
		if (positionOffset != NULL)
			renderPosition += positionOffset->offset;
		if (renderOffset != NULL)
			renderPosition += renderOffset->offset;

		// adjust y coordinate based on the height of the tile specified
		float difference = (billboard->height / 2.0f) - 0.5f;
		if (difference > 0.0f)
			renderPosition.y += difference;

		BILLBOARDSPRITE_TYPE type = (billboard->isAxisAligned ? BILLBOARDSPRITE_SCREENANDAXISALIGNED : BILLBOARDSPRITE_SCREENALIGNED);

		// render color (default to white)
		Color renderColor;
		if (color != NULL)
			renderColor = color->color;
		else
			renderColor = COLOR_WHITE;

		// apply world lighting adjustments to the render color (if applicable)
		if (isAffectedByLighting)
			renderColor = GetLightingColor(position, world, renderColor);

		if (billboard->texture != NULL)
			billboardSpriteBatch->Render(billboard->texture, renderPosition, billboard->width, billboard->height, type, renderColor);
		else
			billboardSpriteBatch->Render(billboard->textureAtlas, billboard->textureAtlasTileIndex, renderPosition, billboard->width, billboard->height, type, renderColor);
	}
}

Color BillboardRendererSystem::GetLightingColor(PositionComponent *position, WorldComponent *world, const Color &entityColor)
{
	float brightness = 0.0f;

	if (world->world->GetLighter() == NULL)
		brightness = Tile::GetBrightness(world->world->GetAmbientLightValue());
	else
	{
		const Tile *tile = world->world->GetTileAt(position);
		brightness = tile->GetBrightness();
	}

	Color result;
	result.r = entityColor.r * brightness;
	result.g = entityColor.g * brightness;
	result.b = entityColor.b * brightness;
	result.a = entityColor.a;
	
	return result;
}

