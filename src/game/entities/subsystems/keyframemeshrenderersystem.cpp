#include "../../../framework/debug.h"

#include "../../../framework/common.h"
#include "keyframemeshrenderersystem.h"
#include "../components/affectedbylightingcomponent.h"
#include "../components/boundingspherecomponent.h"
#include "../components/colorcomponent.h"
#include "../components/keyframemeshcomponent.h"
#include "../components/notvisiblecomponent.h"
#include "../components/orientationxzcomponent.h"
#include "../components/positioncomponent.h"
#include "../components/positionoffsetcomponent.h"
#include "../components/renderoffsetcomponent.h"
#include "../../../entities/entity.h"
#include "../../../entities/entitymanager.h"
#include "../../../contexts/rendercontext.h"
#include "../../../framework/assets/animation/keyframemesh.h"
#include "../../../framework/assets/animation/keyframemeshrenderer.h"
#include "../../../framework/graphics/blendstate.h"
#include "../../../framework/graphics/color.h"
#include "../../../framework/graphics/graphicsdevice.h"
#include "../../../framework/graphics/renderstate.h"
#include "../../../framework/graphics/standardshader.h"
#include "../../../framework/graphics/texture.h"
#include "../../../framework/graphics/vertexlerpshader.h"
#include "../../../framework/math/boundingsphere.h"
#include "../../../framework/math/camera.h"
#include "../../../framework/math/frustum.h"
#include "../../../framework/math/matrix3x3.h"
#include "../../../framework/math/matrix4x4.h"
#include "../../../framework/math/vector3.h"

KeyframeMeshRendererSystem::KeyframeMeshRendererSystem(EntityManager *entityManager, EventManager *eventManager)
	: ComponentSystem(entityManager, eventManager)
{
	STACK_TRACE;
	m_renderState = new RENDERSTATE_DEFAULT;
	m_defaultBlendState = new BLENDSTATE_DEFAULT;
	m_alphaBlendState = new BLENDSTATE_ALPHABLEND;
}

KeyframeMeshRendererSystem::~KeyframeMeshRendererSystem()
{
	STACK_TRACE;
	SAFE_DELETE(m_renderState);
	SAFE_DELETE(m_defaultBlendState);
	SAFE_DELETE(m_alphaBlendState);
}

void KeyframeMeshRendererSystem::OnRender(RenderContext *context)
{
	STACK_TRACE;
	const Frustum *frustum = context->GetGraphicsDevice()->GetCamera()->GetFrustum();
	KeyframeMeshRenderer *meshRenderer = context->GetKeyframeMeshRenderer();

	BOOL isAlphaEnabled = FALSE;

	EntityList list;
	GetEntityManager()->GetAllWith<KeyframeMeshComponent>(list);

	VertexLerpShader *shader = context->GetAnimatedObjectShader();
	if (list.size() > 0)
	{
		m_renderState->Apply();
		m_defaultBlendState->Apply();
		context->GetGraphicsDevice()->BindShader(shader);
	}

	for (EntityList::iterator i = list.begin(); i != list.end(); ++i)
	{
		Entity *entity = *i;
		if (entity->Has<NotVisibleComponent>())
			continue;
		
		KeyframeMeshComponent *keyframeMesh = entity->Get<KeyframeMeshComponent>();
		PositionComponent *position = entity->Get<PositionComponent>();
		OrientationXZComponent *orientation = entity->Get<OrientationXZComponent>();
		BoundingSphereComponent *bounds = entity->Get<BoundingSphereComponent>();
		PositionOffsetComponent *positionOffset = entity->Get<PositionOffsetComponent>();
		RenderOffsetComponent *renderOffset = entity->Get<RenderOffsetComponent>();
		ColorComponent *color = entity->Get<ColorComponent>();
		//BOOL isAffectedByLighting = entity->Has<AffectedByLightingComponent>();

		ASSERT(position != NULL);
		ASSERT(orientation != NULL);

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

		float angle = orientation->angle;
		Vector3 forward = FORWARD * Matrix4x4::CreateRotationY(-angle);
		
		Matrix4x4 world = Matrix4x4::CreateWorld(renderPosition, forward, UP);

		shader->SetModelViewMatrix(context->GetGraphicsDevice()->GetModelViewMatrix() * world);
		shader->SetProjectionMatrix(context->GetGraphicsDevice()->GetProjectionMatrix());
		shader->SetUniform("u_ambient", context->GetAmbientLight());
		shader->SetUniform("u_lightDir", context->GetCommonLightDir());
		shader->SetUniform("u_lightColor", COLOR_WHITE);
		shader->SetUniform("u_modelViewNormalMatrix", Matrix3x3::Transpose(Matrix3x3::Inverse(Matrix3x3(world))));

		if (color != NULL)
		{
			shader->SetUniform("u_color", color->color);

			// check if we need to switch alpha blending state on/off
			// (we don't want to do this any more then is necessary)
			if (color->color.a < COLOR_ALPHA_OPAQUE && !isAlphaEnabled)
			{
				m_alphaBlendState->Apply();
				isAlphaEnabled = TRUE;
			}
			else if (color->color.a == COLOR_ALPHA_OPAQUE && isAlphaEnabled)
			{
				m_defaultBlendState->Apply();
				isAlphaEnabled = FALSE;
			}
		}
		else
			shader->SetUniform("u_color", COLOR_WHITE);

		if (keyframeMesh->startFrame == keyframeMesh->endFrame)
			meshRenderer->Render(context->GetGraphicsDevice(), keyframeMesh->mesh, keyframeMesh->texture, keyframeMesh->startFrame, shader);
		else
			meshRenderer->Render(context->GetGraphicsDevice(), keyframeMesh->mesh, keyframeMesh->texture, keyframeMesh->startFrame, keyframeMesh->endFrame, keyframeMesh->interpolation, shader);
	}

	if (list.size() > 0)
	{
		context->GetGraphicsDevice()->UnbindShader();
	}
}

