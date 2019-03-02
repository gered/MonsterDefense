#ifndef __CONTEXTS_RENDERCONTEXT_H_INCLUDED__
#define __CONTEXTS_RENDERCONTEXT_H_INCLUDED__

#include "../framework/common.h"

#include "../framework/graphics/color.h"
#include "../framework/math/vector3.h"

class BillboardSpriteBatch;
class ContentManager;
class GraphicsDevice;
class KeyframeMeshRenderer;
class SpriteBatch;
class StaticMeshRenderer;
class StandardShader;
class TileMapRenderer;
class VertexLerpShader;

class RenderContext
{
public:
	RenderContext(GraphicsDevice *graphicsDevice, ContentManager *contentManager);
	virtual ~RenderContext();

	void OnLoadGame();
	void OnPreRender();
	void OnPostRender();
	void OnResize();

	GraphicsDevice* GetGraphicsDevice() const              { return m_graphicsDevice; }
	SpriteBatch* GetSpriteBatch() const                    { return m_spriteBatch; }
	BillboardSpriteBatch* GetBillboardSpriteBatch() const  { return m_billboardSpriteBatch; }
	KeyframeMeshRenderer* GetKeyframeMeshRenderer() const  { return m_keyframeMeshRenderer; }
	StaticMeshRenderer* GetStaticMeshRenderer() const      { return m_staticMeshRenderer; }
	TileMapRenderer* GetTileMapRenderer() const            { return m_tileMapRenderer; }

	StandardShader* GetPerPixelLightingShader() const      { return m_perPixelLightingShader; }
	StandardShader* GetNormalTerrainShader() const         { return m_normalTerrainShader; }
	StandardShader* GetStaticObjectLitShader() const       { return m_staticObjectLitShader; }
	StandardShader* GetStaticObjectUnlitShader() const     { return m_staticObjectUnlitShader; }
	VertexLerpShader* GetAnimatedObjectShader() const      { return m_animatedObjectShader; }

	uint32_t GetScreenScale() const                        { return m_screenScale; }

	Color GetAmbientLight() const                          { return m_ambientLight; }
	Vector3 GetCommonLightDir() const                      { return m_commonLightDir; }

private:
	void CalculateScreenScale();

	ContentManager *m_contentManager;
	GraphicsDevice *m_graphicsDevice;
	SpriteBatch *m_spriteBatch;
	BillboardSpriteBatch *m_billboardSpriteBatch;
	KeyframeMeshRenderer *m_keyframeMeshRenderer;
	StaticMeshRenderer *m_staticMeshRenderer;
	TileMapRenderer *m_tileMapRenderer;

	StandardShader *m_perPixelLightingShader;
	StandardShader *m_normalTerrainShader;
	StandardShader *m_staticObjectLitShader;
	StandardShader *m_staticObjectUnlitShader;
	VertexLerpShader *m_animatedObjectShader;

	uint32_t m_screenScale;

	Color m_ambientLight;
	Vector3 m_commonLightDir;
};

#endif

