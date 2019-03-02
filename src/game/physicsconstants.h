#ifndef __GAME_PHYSICSCONSTANTS_H_INCLUDED__
#define __GAME_PHYSICSCONSTANTS_H_INCLUDED__

#include "../framework/math/mathhelpers.h"
#include "../framework/math/vector3.h"

const float UNITS_PER_METRE = 1.0f;

const float GRAVITY_SPEED = 9.8f * UNITS_PER_METRE;
const Vector3 GRAVITY(0.0f, -GRAVITY_SPEED, 0.0f);

const float FRICTION_NORMAL = 0.4f;

const float SLOPE_STEEP_Y_ANGLE = DegreesToRadians(46.0f);
const float ON_GROUND_ZERO_TOLERANCE = 0.1f;

const float FALL_DAMAGE_PERCENT_PER_HEIGHT = 0.10f;
const float MINIMUM_HIGH_FALL_DISTANCE = 5.0f * UNITS_PER_METRE;
const float HIGHER_FALL_DISTANCE_STEPS = 2.0f * UNITS_PER_METRE;

const float STANDARD_PUSH_STRENGTH = 1.5f;
const float STANDARD_PUSH_RESISTANCE = 1.0f;

#endif
