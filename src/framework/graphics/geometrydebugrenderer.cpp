#include "../debug.h"

#include "geometrydebugrenderer.h"

#include <math.h>

#include "graphicsdevice.h"
#include "debugshader.h"
#include "renderstate.h"
#include "vertexbuffer.h"
#include "../math/vector3.h"
#include "../math/boundingbox.h"
#include "../math/boundingsphere.h"
#include "../math/linesegment.h"
#include "../math/point3.h"
#include "../math/ray.h"

GeometryDebugRenderer::GeometryDebugRenderer(GraphicsDevice *graphicsDevice, BOOL depthTesting)
{
	STACK_TRACE;
	m_graphicsDevice = graphicsDevice;
	m_color1 = Color(1.0f, 1.0f, 0.0f);
	m_color2 = Color(1.0f, 0.0f, 0.0f);

	m_renderState = new RENDERSTATE_DEFAULT;
	ASSERT(m_renderState != NULL);
	m_renderState->SetDepthTesting(depthTesting);
	m_renderState->SetLineWidth(2.0f);

	m_vertices = new VertexBuffer(BUFFEROBJECT_USAGE_STREAM);
	ASSERT(m_vertices != NULL);
	m_vertices->AddAttribute(VERTEX_POS_3D);
	m_vertices->AddAttribute(VERTEX_COLOR);
	m_vertices->Create(16384);
	m_currentVertex = 0;

	m_begunRendering = FALSE;
}

GeometryDebugRenderer::~GeometryDebugRenderer()
{
	STACK_TRACE;
	SAFE_DELETE(m_vertices);
	SAFE_DELETE(m_renderState);
}

void GeometryDebugRenderer::Begin()
{
	STACK_TRACE;
	m_currentVertex = 0;
	m_begunRendering = TRUE;
}

void GeometryDebugRenderer::Render(const BoundingBox &box, const Color &color)
{
	STACK_TRACE;
	ASSERT(m_begunRendering == TRUE);
	ASSERT(m_vertices->GetNumElements() > (m_currentVertex + 24));

	uint32_t i = m_currentVertex;

	// removed lines which are duplicated by more then one face
	// left and right faces don't need to be drawn at all (entirely duplicated lines)

	// top
	m_vertices->SetPosition3(i++, box.min.x, box.max.y, box.min.z);
	m_vertices->SetPosition3(i++, box.max.x, box.max.y, box.min.z);

	m_vertices->SetPosition3(i++, box.max.x, box.max.y, box.min.z);
	m_vertices->SetPosition3(i++, box.max.x, box.max.y, box.max.z);

	m_vertices->SetPosition3(i++, box.max.x, box.max.y, box.max.z);
	m_vertices->SetPosition3(i++, box.min.x, box.max.y, box.max.z);

	m_vertices->SetPosition3(i++, box.min.x, box.max.y, box.max.z);
	m_vertices->SetPosition3(i++, box.min.x, box.max.y, box.min.z);

	// back
	m_vertices->SetPosition3(i++, box.min.x, box.min.y, box.min.z);
	m_vertices->SetPosition3(i++, box.max.x, box.min.y, box.min.z);

	m_vertices->SetPosition3(i++, box.max.x, box.min.y, box.min.z);
	m_vertices->SetPosition3(i++, box.max.x, box.max.y, box.min.z);

	m_vertices->SetPosition3(i++, box.min.x, box.max.y, box.min.z);
	m_vertices->SetPosition3(i++, box.min.x, box.min.y, box.min.z);

	// front
	m_vertices->SetPosition3(i++, box.min.x, box.min.y, box.max.z);
	m_vertices->SetPosition3(i++, box.max.x, box.min.y, box.max.z);

	m_vertices->SetPosition3(i++, box.max.x, box.min.y, box.max.z);
	m_vertices->SetPosition3(i++, box.max.x, box.max.y, box.max.z);

	m_vertices->SetPosition3(i++, box.min.x, box.max.y, box.max.z);
	m_vertices->SetPosition3(i++, box.min.x, box.min.y, box.max.z);

	// bottom
	m_vertices->SetPosition3(i++, box.max.x, box.min.y, box.min.z);
	m_vertices->SetPosition3(i++, box.max.x, box.min.y, box.max.z);

	m_vertices->SetPosition3(i++, box.min.x, box.min.y, box.max.z);
	m_vertices->SetPosition3(i++, box.min.x, box.min.y, box.min.z);

	// fill in all the colours
	for (uint32_t j = m_currentVertex; j < i; ++j)
		m_vertices->SetColor(j, color);

	m_currentVertex = i;
}

void GeometryDebugRenderer::Render(const Point3 &boxMin, const Point3 &boxMax, const Color &color)
{
	STACK_TRACE;
	BoundingBox b((float)boxMin.x, (float)boxMin.y, (float)boxMin.z, (float)boxMax.x, (float)boxMax.y, (float)boxMax.z);
	Render(b, color);
}

void GeometryDebugRenderer::Render(const BoundingSphere &sphere, const Color &color)
{
	STACK_TRACE;
	ASSERT(m_begunRendering == TRUE);
	ASSERT(m_vertices->GetNumElements() > (m_currentVertex + 615));

	uint32_t p = m_currentVertex;

	float ax, ay, az;
	float bx, by, bz;
	float cx = 0.0f, cy = 0.0f, cz = 0.0f;
	float dx = 0.0f, dy = 0.0f, dz = 0.0f;
	float theta1, theta2, theta3;

	int n = 12;
	for (int j = 0; j < n / 2; ++j)
	{
		theta1 = j * PI * 2 / n - PI / 2;
		theta2 = (j + 1) * PI * 2 / n - PI / 2;

		for (int i = 0; i <= n; ++i)
		{
			theta3 = i * PI * 2 / n;
			ax = sphere.center.x + sphere.radius * cosf(theta2) * cosf(theta3);
			ay = sphere.center.y + sphere.radius * sinf(theta2);
			az = sphere.center.z + sphere.radius * cosf(theta2) * sinf(theta3);

			bx = sphere.center.x + sphere.radius * cosf(theta1) * cosf(theta3);
			by = sphere.center.y + sphere.radius * sinf(theta1);
			bz = sphere.center.z + sphere.radius * cosf(theta1) * sinf(theta3);

			if (j > 0 || i > 0)
			{
				m_vertices->SetPosition3(p++, ax, ay, az);
				m_vertices->SetPosition3(p++, bx, by, bz);

				m_vertices->SetPosition3(p++, bx, by, bz);
				m_vertices->SetPosition3(p++, dx, dy, dz);

				m_vertices->SetPosition3(p++, dx, dy, dz);
				m_vertices->SetPosition3(p++, cx, cy, cz);

				m_vertices->SetPosition3(p++, cx, cy, cz);
				m_vertices->SetPosition3(p++, ax, ay, az);
			}

			cx = ax;
			cy = ay;
			cz = az;
			dx = bx;
			dy = by;
			dz = bz;
		}
	}

	// fill in all the colours
	for (uint32_t i = m_currentVertex; i < p; ++i)
		m_vertices->SetColor(i, color);

	m_currentVertex = p;
}

void GeometryDebugRenderer::Render(const Ray &ray, float length, const Color &color1, const Color &color2)
{
	STACK_TRACE;
	ASSERT(m_begunRendering == TRUE);
	ASSERT(m_vertices->GetNumElements() > (m_currentVertex + 2));

	Vector3 temp = ray.GetPositionAt(length);

	m_vertices->SetPosition3(m_currentVertex, ray.position);
	m_vertices->SetColor(m_currentVertex, color1);
	m_vertices->SetPosition3(m_currentVertex + 1, temp);
	m_vertices->SetColor(m_currentVertex + 1, color2);

	m_currentVertex += 2;
}

void GeometryDebugRenderer::Render(const LineSegment &line, const Color &color)
{
	STACK_TRACE;
	ASSERT(m_begunRendering == TRUE);
	ASSERT(m_vertices->GetNumElements() > (m_currentVertex + 2));

	m_vertices->SetPosition3(m_currentVertex, line.a);
	m_vertices->SetColor(m_currentVertex, color);
	m_vertices->SetPosition3(m_currentVertex + 1, line.b);
	m_vertices->SetColor(m_currentVertex + 1, color);

	m_currentVertex += 2;
}

void GeometryDebugRenderer::Render(const Vector3 &a, const Vector3 &b, const Vector3 &c, const Color &color)
{
	STACK_TRACE;
	ASSERT(m_begunRendering == TRUE);
	ASSERT(m_vertices->GetNumElements() > (m_currentVertex + 6));

	m_vertices->SetPosition3(m_currentVertex, a);
	m_vertices->SetColor(m_currentVertex, color);
	m_vertices->SetPosition3(m_currentVertex + 1, b);
	m_vertices->SetColor(m_currentVertex + 1, color);
	m_vertices->SetPosition3(m_currentVertex + 2, a);
	m_vertices->SetColor(m_currentVertex + 2, color);
	m_vertices->SetPosition3(m_currentVertex + 3, c);
	m_vertices->SetColor(m_currentVertex + 3, color);
	m_vertices->SetPosition3(m_currentVertex + 4, b);
	m_vertices->SetColor(m_currentVertex + 4, color);
	m_vertices->SetPosition3(m_currentVertex + 5, c);
	m_vertices->SetColor(m_currentVertex + 5, color);

	m_currentVertex += 6;
}

void GeometryDebugRenderer::End()
{
	STACK_TRACE;
	ASSERT(m_begunRendering == TRUE);

	if (m_currentVertex > 0)
	{
		m_graphicsDevice->BindShader(m_graphicsDevice->GetDebugShader());
		m_graphicsDevice->GetDebugShader()->SetModelViewMatrix(m_graphicsDevice->GetModelViewMatrix());
		m_graphicsDevice->GetDebugShader()->SetProjectionMatrix(m_graphicsDevice->GetProjectionMatrix());

		m_renderState->Apply();
		m_graphicsDevice->BindVertexBuffer(m_vertices);
		m_graphicsDevice->RenderLines(0, m_currentVertex / 2);
		m_graphicsDevice->RenderPoints(0, m_currentVertex);
		m_graphicsDevice->UnbindVertexBuffer();

		m_graphicsDevice->UnbindShader();
	}

	m_begunRendering = FALSE;
}
