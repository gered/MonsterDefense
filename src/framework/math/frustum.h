#ifndef __FRAMEWORK_MATH_FRUSTUM_H_INCLUDED__
#define __FRAMEWORK_MATH_FRUSTUM_H_INCLUDED__

#include "../common.h"
#include "plane.h"

class GraphicsDevice;
struct BoundingBox;
struct BoundingSphere;

enum FRUSTUM_SIDES
{
	FRUSTUM_RIGHT = 0,
	FRUSTUM_LEFT = 1,
	FRUSTUM_BOTTOM = 2,
	FRUSTUM_TOP = 3,
	FRUSTUM_BACK = 4,
	FRUSTUM_FRONT = 5
};

const int32_t NUM_FRUSTUM_SIDES = 6;

/**
 * A viewing frustum that can be used to test for geometry visibility
 * given the current camera modelview and projection matrices.
 */
class Frustum
{
public:
	/**
	 * Creates a frustum object.
	 * @param graphicsDevice the graphics device this frustum is for
	 */
	Frustum(GraphicsDevice *graphicsDevice);

	virtual ~Frustum();

	/**
	 * Recalculates the viewing frustum from the current camera matrices.
	 */
	void Calculate();

	/**
	 * Tests a point for visibility.
	 * @param point the point to be tested
	 * @return BOOL TRUE if visible, FALSE if not
	 */
	BOOL Test(const Vector3 &point) const;

	/**
	 * Tests a box for visibility.
	 * @param box the box to be tested
	 * @return BOOL TRUE if at least partially visible, FALSE if entirely
	 *              outside the viewing frustum
	 */
	BOOL Test(const BoundingBox &box) const;

	/**
	 * Tests a sphere for visibility.
	 * @param sphere the sphere to be tested
	 * @return BOOL TRUE if at least partially visible, FALSE if entirely
	 *              outside the viewing frustum
	 */
	BOOL Test(const BoundingSphere &sphere) const;

private:
	BOOL TestPlaneAgainstBox(const Plane &plane, float minX, float minY, float minZ, float width, float height, float depth) const;
	BOOL TestPlaneAgainstSphere(const Plane &plane, const Vector3 &center, float radius) const;

	GraphicsDevice *m_graphicsDevice;
	Plane m_planes[NUM_FRUSTUM_SIDES];
};

#endif
