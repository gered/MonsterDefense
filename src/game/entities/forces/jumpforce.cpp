#include "../../../framework/debug.h"

#include "jumpforce.h"
#include "../components/physicscomponent.h"
#include "../../physicsconstants.h"
#include "../../../framework/math/common.h"
#include "../../../framework/math/mathhelpers.h"

const float JUMP_MINIMUM_DURATION = 0.025f;

JumpForce::JumpForce(const Vector3 &direction, float strength, float friction, const PhysicsComponent *physicsComponent)
	: Force(direction, strength, friction, JUMP_MINIMUM_DURATION, ON_GROUND_ZERO_TOLERANCE)
{
	STACK_TRACE;
	m_physicsComponent = physicsComponent;
}

JumpForce::~JumpForce()
{
	STACK_TRACE;
}

void JumpForce::Update(float delta)
{
	STACK_TRACE;
	Force::Update(delta);

	// Force::Update() call could potentially kill this force and then return 
	// here, so we should check for that first
	if (!IsActive())
		return;

	BOOL cancelled = FALSE;

	// jumping should be cancelled when the top of the entity hits a surface
	// that is more or less perpendicular to the jumping direction (positive Y).
	// it should also be stopped if the entity has landed on some ground early
	// along in the jump (e.g. they jumped up to a ledge)
	if (m_physicsComponent->collisionPacket.foundCollision || m_physicsComponent->isSliding)
	{
		if (m_physicsComponent->isSliding)
		{
			// if we're sliding, then check the angle
			float slideYAngle = acosf(Vector3::Dot(m_physicsComponent->slidingPlaneNormal.direction, Y_AXIS));

			// Y axis angle's from 135 to 225 means we hit something overhead.
			// 180 degrees = hit something perfectly perpendicular to the Y axis
			// HACK: also we check for a Y angle of zero as that will handle an 
			// edge case where the entity jumped and immediately overhead there 
			// is an obstacle (in this case, most of the time, the slide Y angle
			// is actually for the collision below their feet...)
			if ((slideYAngle > RADIANS_135 && slideYAngle < RADIANS_225) || IsCloseEnough(slideYAngle, 0.0f))
				cancelled = TRUE;
		}
		else
			// not sliding, just a full-on collision with something
			// (collision but not sliding means it's usually a flat area of the 
			// ground), so this is really IsOnGround() == TRUE (??)
			cancelled = TRUE;
	}

	// don't kill it, even if there was a collision found above, if the force
	// hasn't been running for the minimum duration yet
	if (cancelled && HasMinDurationPassed())
		Kill();
}

