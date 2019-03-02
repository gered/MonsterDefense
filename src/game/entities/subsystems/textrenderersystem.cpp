#include "../../../framework/debug.h"

#include "../../../framework/common.h"
#include "textrenderersystem.h"
#include "../components/boundingspherecomponent.h"
#include "../components/colorcomponent.h"
#include "../components/notvisiblecomponent.h"
#include "../components/positioncomponent.h"
#include "../components/positionoffsetcomponent.h"
#include "../components/renderoffsetcomponent.h"
#include "../components/textcomponent.h"
#include "../../../entities/entity.h"
#include "../../../entities/entitymanager.h"
#include "../../../contexts/rendercontext.h"
#include "../../../framework/graphics/billboardspritebatch.h"
#include "../../../framework/graphics/color.h"
#include "../../../framework/graphics/graphicsdevice.h"
#include "../../../framework/graphics/spritebatch.h"
#include "../../../framework/graphics/spritefont.h"
#include "../../../framework/math/camera.h"
#include "../../../framework/math/frustum.h"
#include "../../../framework/math/vector3.h"

TextRendererSystem::TextRendererSystem(EntityManager *entityManager, EventManager *eventManager)
: ComponentSystem(entityManager, eventManager)
{
	STACK_TRACE;
}

TextRendererSystem::~TextRendererSystem()
{
	STACK_TRACE;
}

void TextRendererSystem::OnRender(RenderContext *renderContext)
{
	STACK_TRACE;
	const Frustum *frustum = renderContext->GetGraphicsDevice()->GetCamera()->GetFrustum();
	BillboardSpriteBatch *billboardSpriteBatch = renderContext->GetBillboardSpriteBatch();
	SpriteBatch *spriteBatch = renderContext->GetSpriteBatch();
	
	EntityList list;
	GetEntityManager()->GetAllWith<TextComponent>(list);
	for (EntityList::iterator i = list.begin(); i != list.end(); ++i)
	{
		Entity *entity = *i;
		
		if (entity->Has<NotVisibleComponent>())
			continue;
		
		TextComponent *text = entity->Get<TextComponent>();
		PositionComponent *position = entity->Get<PositionComponent>();
		BoundingSphereComponent *bounds = entity->Get<BoundingSphereComponent>();
		PositionOffsetComponent *positionOffset = entity->Get<PositionOffsetComponent>();
		RenderOffsetComponent *renderOffset = entity->Get<RenderOffsetComponent>();
		ColorComponent *color = entity->Get<ColorComponent>();
		
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
		
		renderPosition += text->offset;

		// render color (default to white)
		Color renderColor;
		if (color != NULL)
			renderColor = color->color;
		else
			renderColor = COLOR_WHITE;
		
		if (text->useBillboardRendering)
		{
			//// adjust y coordinate based on the height of the tile specified
			//float difference = (billboard->height / 2.0f) - 0.5f;
			//if (difference > 0.0f)
			//	renderPosition.y += difference;
			
			billboardSpriteBatch->Render(text->font, renderPosition, BILLBOARDSPRITE_SCREENALIGNED, renderColor, text->billboardSizePerPixel, text->text);
		}
		else
			spriteBatch->Render(text->font, renderPosition, renderColor, (float)renderContext->GetScreenScale(), text->text);
	}
}

