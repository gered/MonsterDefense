#ifndef __GAME_DIRECTION_H_INCLUDED__
#define __GAME_DIRECTION_H_INCLUDED__

#include "../framework/math/common.h"
#include "../framework/math/vector3.h"
#include "../framework/math/mathhelpers.h"

enum FacingDirection
{
	FACING_NORTH,
	FACING_EAST,
	FACING_SOUTH,
	FACING_WEST
};

// this has been set up such that:
// east = +x direction
// west = -x direction
// north = -z direction
// south = +z direction

const float FACING_NORTH_ANGLE = RADIANS_0;
const float FACING_NORTHEAST_ANGLE = RADIANS_45;
const float FACING_EAST_ANGLE = RADIANS_90;
const float FACING_SOUTHEAST_ANGLE = RADIANS_135;
const float FACING_SOUTH_ANGLE = RADIANS_180;
const float FACING_SOUTHWEST_ANGLE = RADIANS_225;
const float FACING_WEST_ANGLE = RADIANS_270;
const float FACING_NORTHWEST_ANGLE = RADIANS_315;

inline FacingDirection GetFacingDirection(float orientationY)
{
	// add 45 degrees initially to kind of "rotate" the 360 degree circle
	// clockwise because 315 -> 360 is also part of the "east" quadrant along
	// with 0 -> 45.
	float adjusted = orientationY + RADIANS_45;
	if (adjusted >= RADIANS_360)
		adjusted -= RADIANS_360;

	int quadrant = (int)(adjusted / RADIANS_90);
	return (FacingDirection)quadrant;
}

inline FacingDirection GetFacingDirection(const Vector3 &orientation)
{
	return GetFacingDirection(orientation.y);
}

inline FacingDirection GetFacingDirectionAdjustedForCamera(float objectOrientationY, const Vector3 &cameraOrientation)
{
	float difference = RolloverClamp(objectOrientationY - cameraOrientation.y, RADIANS_0, RADIANS_360);
	return GetFacingDirection(difference);
}

inline float GetFacingAngle(FacingDirection direction)
{
	return ((float)direction) * RADIANS_90;
}

#endif

