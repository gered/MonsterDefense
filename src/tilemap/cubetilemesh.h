#ifndef __TILEMAP_CUBETILEMESH_H_INCLUDED__
#define __TILEMAP_CUBETILEMESH_H_INCLUDED__

#include "../framework/common.h"
#include "tilelightdefs.h"
#include "tilemesh.h"
#include "tilemeshdefs.h"
#include "../framework/graphics/color.h"

class VertexBuffer;
struct RectF;
struct Vector2;
struct Vector3;

class CubeTileMesh : public TileMesh
{
public:
	CubeTileMesh(CUBE_FACES faces, const RectF *textureAtlasTileBoundaries, MESH_SIDES opaqueSides, TILE_LIGHT_VALUE lightValue, BOOL alpha, float translucency = 1.0f, const Color &color = COLOR_WHITE);
	virtual ~CubeTileMesh();

	VertexBuffer* GetBuffer() const                        { return m_vertices; }
	uint32_t GetTopFaceVertexOffset() const                { return m_topFaceVertexOffset; }
	uint32_t GetBottomFaceVertexOffset() const             { return m_bottomFaceVertexOffset; }
	uint32_t GetFrontFaceVertexOffset() const              { return m_frontFaceVertexOffset; }
	uint32_t GetBackFaceVertexOffset() const               { return m_backFaceVertexOffset; }
	uint32_t GetLeftFaceVertexOffset() const               { return m_leftFaceVertexOffset; }
	uint32_t GetRightFaceVertexOffset() const              { return m_rightFaceVertexOffset; }

	uint32_t GetNumCollisionVertices() const               { return m_numCollisionVertices; }
	const Vector3* GetCollisionVertices() const            { return m_collisionVertices; }

	CUBE_FACES GetFaces() const                            { return m_faces; }
	BOOL HasFace(CUBE_FACES face) const                    { return IsBitSet(face, m_faces); }

	TILEMESH_TYPE GetType() const                          { return TILEMESH_CUBE; }

private:
	void SetupFaceVertices(const RectF *textureAtlasTileBoundaries);
	void SetupCollisionVertices();

	Vector2 ScaleTexCoord(const Vector2 &texCoord, const RectF &tileBoundaries) const;

	VertexBuffer *m_vertices;
	uint32_t m_topFaceVertexOffset;
	uint32_t m_bottomFaceVertexOffset;
	uint32_t m_frontFaceVertexOffset;
	uint32_t m_backFaceVertexOffset;
	uint32_t m_leftFaceVertexOffset;
	uint32_t m_rightFaceVertexOffset;
	uint32_t m_numCollisionVertices;
	Vector3 *m_collisionVertices;
	CUBE_FACES m_faces;
};


#endif

