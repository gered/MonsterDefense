#include "../framework/debug.h"

#include "rendercontext.h"

#include "../framework/assets/animation/keyframemeshrenderer.h"
#include "../framework/assets/static/staticmeshrenderer.h"
#include "../framework/content/contentmanager.h"
#include "../framework/graphics/billboardspritebatch.h"
#include "../framework/graphics/customstandardshader.h"
#include "../framework/graphics/customvertexlerpshader.h"
#include "../framework/graphics/graphicsdevice.h"
#include "../framework/graphics/renderstate.h"
#include "../framework/graphics/spritebatch.h"
#include "../framework/support/text.h"
#include "../tilemap/tilemaprenderer.h"

RenderContext::RenderContext(GraphicsDevice *graphicsDevice, ContentManager *contentManager)
{
	STACK_TRACE;
	ASSERT(graphicsDevice != NULL);
	ASSERT(contentManager != NULL);

	m_graphicsDevice = graphicsDevice;
	m_contentManager = contentManager;

	m_spriteBatch = new SpriteBatch(m_graphicsDevice);
	ASSERT(m_spriteBatch != NULL);

	m_billboardSpriteBatch = new BillboardSpriteBatch(m_graphicsDevice);
	ASSERT(m_billboardSpriteBatch != NULL);

	m_keyframeMeshRenderer = new KeyframeMeshRenderer();
	ASSERT(m_keyframeMeshRenderer != NULL);

	m_staticMeshRenderer = new StaticMeshRenderer();
	ASSERT(m_staticMeshRenderer != NULL);

	m_tileMapRenderer = new TileMapRenderer(m_graphicsDevice);
	ASSERT(m_tileMapRenderer != NULL);

	m_perPixelLightingShader = NULL;
	m_normalTerrainShader = NULL;
	m_staticObjectLitShader = NULL;
	m_staticObjectUnlitShader = NULL;
	m_animatedObjectShader = NULL;

	CalculateScreenScale();

	m_ambientLight = Color(0.5f, 0.5f, 0.5f, 1.0f);
	m_commonLightDir = Vector3::Normalize(Vector3(-1.0f, 1.0f, 0.5f));
}

RenderContext::~RenderContext()
{
	STACK_TRACE;
	m_graphicsDevice->UnregisterManagedResource(m_perPixelLightingShader);
	m_graphicsDevice->UnregisterManagedResource(m_normalTerrainShader);
	m_graphicsDevice->UnregisterManagedResource(m_staticObjectLitShader);
	m_graphicsDevice->UnregisterManagedResource(m_staticObjectUnlitShader);
	m_graphicsDevice->UnregisterManagedResource(m_animatedObjectShader);

	SAFE_DELETE(m_spriteBatch);
	SAFE_DELETE(m_billboardSpriteBatch);
	SAFE_DELETE(m_keyframeMeshRenderer);
	SAFE_DELETE(m_staticMeshRenderer);
	SAFE_DELETE(m_tileMapRenderer);
	SAFE_DELETE(m_perPixelLightingShader);
	SAFE_DELETE(m_normalTerrainShader);
	SAFE_DELETE(m_staticObjectLitShader);
	SAFE_DELETE(m_staticObjectUnlitShader);
	SAFE_DELETE(m_animatedObjectShader);
}

void RenderContext::OnLoadGame()
{
	STACK_TRACE;
	Text *vertSrc;
	Text *fragSrc;

	vertSrc = m_contentManager->Get<Text>("assets://shaders/perpixellighting.glsl.vert");
	fragSrc = m_contentManager->Get<Text>("assets://shaders/perpixellighting.glsl.frag");
	m_perPixelLightingShader = new CustomStandardShader(vertSrc, fragSrc);
	m_perPixelLightingShader->MapAttributeToStandardAttribType("a_position", VERTEX_POS_3D);
	m_perPixelLightingShader->MapAttributeToStandardAttribType("a_normal", VERTEX_NORMAL);
	m_perPixelLightingShader->MapAttributeToStandardAttribType("a_texcoord0", VERTEX_TEXCOORD);

	vertSrc = m_contentManager->Get<Text>("assets://shaders/normal_terrain.glsl.vert");
	fragSrc = m_contentManager->Get<Text>("assets://shaders/normal_terrain.glsl.frag");
	m_normalTerrainShader = new CustomStandardShader(vertSrc, fragSrc);
	m_normalTerrainShader->MapAttributeToStandardAttribType("a_position", VERTEX_POS_3D);
	m_normalTerrainShader->MapAttributeToStandardAttribType("a_normal", VERTEX_NORMAL);
	m_normalTerrainShader->MapAttributeToStandardAttribType("a_texcoord0", VERTEX_TEXCOORD);

	vertSrc = m_contentManager->Get<Text>("assets://shaders/static_object_lit.glsl.vert");
	fragSrc = m_contentManager->Get<Text>("assets://shaders/static_object_lit.glsl.frag");
	m_staticObjectLitShader = new CustomStandardShader(vertSrc, fragSrc);
	m_staticObjectLitShader->MapAttributeToStandardAttribType("a_position", VERTEX_POS_3D);
	m_staticObjectLitShader->MapAttributeToStandardAttribType("a_normal", VERTEX_NORMAL);
	m_staticObjectLitShader->MapAttributeToStandardAttribType("a_texcoord0", VERTEX_TEXCOORD);

	vertSrc = m_contentManager->Get<Text>("assets://shaders/static_object_unlit.glsl.vert");
	fragSrc = m_contentManager->Get<Text>("assets://shaders/static_object_unlit.glsl.frag");
	m_staticObjectUnlitShader = new CustomStandardShader(vertSrc, fragSrc);
	m_staticObjectUnlitShader->MapAttributeToStandardAttribType("a_position", VERTEX_POS_3D);
	m_staticObjectUnlitShader->MapAttributeToStandardAttribType("a_texcoord0", VERTEX_TEXCOORD);

	vertSrc = m_contentManager->Get<Text>("assets://shaders/animated_object.glsl.vert");
	fragSrc = m_contentManager->Get<Text>("assets://shaders/animated_object.glsl.frag");
	m_animatedObjectShader = new CustomVertexLerpShader(vertSrc, fragSrc);
	m_animatedObjectShader->MapAttributeToVboAttribIndex("a_position1", 0);
	m_animatedObjectShader->MapAttributeToVboAttribIndex("a_position2", 1);
	m_animatedObjectShader->MapAttributeToVboAttribIndex("a_normal1", 2);
	m_animatedObjectShader->MapAttributeToVboAttribIndex("a_normal2", 3);
	m_animatedObjectShader->MapAttributeToStandardAttribType("a_texcoord0", VERTEX_TEXCOORD);

	m_graphicsDevice->RegisterManagedResource(m_perPixelLightingShader);
	m_graphicsDevice->RegisterManagedResource(m_normalTerrainShader);
	m_graphicsDevice->RegisterManagedResource(m_staticObjectLitShader);
	m_graphicsDevice->RegisterManagedResource(m_staticObjectUnlitShader);
	m_graphicsDevice->RegisterManagedResource(m_animatedObjectShader);
}

void RenderContext::OnPreRender()
{
	STACK_TRACE;
	RENDERSTATE_DEFAULT.Apply();
	BLENDSTATE_DEFAULT.Apply();

	m_spriteBatch->Begin();
	m_billboardSpriteBatch->Begin();
}

void RenderContext::OnPostRender()
{
	STACK_TRACE;
	m_billboardSpriteBatch->End();
	m_spriteBatch->End();
}

void RenderContext::OnResize()
{
	STACK_TRACE;
	CalculateScreenScale();
}

void RenderContext::CalculateScreenScale()
{
	STACK_TRACE;
	uint16_t width = m_graphicsDevice->GetViewportWidth();
	uint16_t height = m_graphicsDevice->GetViewportHeight();

	m_screenScale = 1;

	if (width < 640 || height < 480)
		return;

	m_screenScale = 2;

	if (width < 960 || height < 720)
		return;

	m_screenScale = 3;

	if (width < 1280 || height < 960)
		return;
}

