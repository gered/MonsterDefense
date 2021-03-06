#include "../framework/debug.h"

#include "chunkrenderer.h"

#include "../framework/graphics/blendstate.h"
#include "../framework/graphics/color.h"
#include "../framework/graphics/graphicsdevice.h"
#include "../framework/graphics/renderstate.h"
#include "../framework/graphics/textureatlas.h"
#include "../framework/graphics/vertexbuffer.h"
#include "../framework/math/matrix4x4.h"
#include "tilechunk.h"
#include "tilemap.h"
#include "tilemeshcollection.h"

ChunkRenderer::ChunkRenderer(GraphicsDevice *graphicsDevice)
{
	STACK_TRACE;
	m_graphicsDevice = graphicsDevice;

	m_renderState = new RENDERSTATE_DEFAULT;
	ASSERT(m_renderState != NULL);

	m_defaultBlendState = new BLENDSTATE_DEFAULT;
	ASSERT(m_defaultBlendState != NULL);

	m_alphaBlendState = new BLENDSTATE_ALPHABLEND;
	ASSERT(m_alphaBlendState != NULL);
}

ChunkRenderer::~ChunkRenderer()
{
	STACK_TRACE;
	SAFE_DELETE(m_renderState);
	SAFE_DELETE(m_defaultBlendState);
	SAFE_DELETE(m_alphaBlendState);
}

uint32_t ChunkRenderer::Render(const TileChunk *chunk)
{
	STACK_TRACE;
	const Texture *texture = chunk->GetTileMap()->GetMeshes()->GetTextureAtlas()->GetTexture();

	uint32_t numVertices = chunk->GetNumVertices();

	m_renderState->Apply();
	m_defaultBlendState->Apply();
	m_graphicsDevice->BindTexture(texture);
	m_graphicsDevice->BindVertexBuffer(chunk->GetVertices());
	m_graphicsDevice->RenderTriangles(0, numVertices / 3);
	m_graphicsDevice->UnbindVertexBuffer();

	return numVertices;
}

uint32_t ChunkRenderer::RenderAlpha(const TileChunk *chunk)
{
	STACK_TRACE;
	uint32_t numVertices = 0;

	if (chunk->IsAlphaEnabled())
	{
		const Texture *texture = chunk->GetTileMap()->GetMeshes()->GetTextureAtlas()->GetTexture();

		numVertices = chunk->GetNumAlphaVertices();

		m_renderState->Apply();
		m_alphaBlendState->Apply();
		m_graphicsDevice->BindTexture(texture);
		m_graphicsDevice->BindVertexBuffer(chunk->GetAlphaVertices());
		m_graphicsDevice->RenderTriangles(0, numVertices / 3);
		m_graphicsDevice->UnbindVertexBuffer();
	}

	return numVertices;
}

