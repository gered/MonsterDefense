#include "../../../framework/debug.h"

#include "force.h"
#include "../../../framework/math/mathhelpers.h"

Force::Force(const Vector3 &direction, float strength, float friction, float zeroTolerance)
{
	STACK_TRACE;
	m_active = TRUE;
	m_direction = direction;
	m_strength = strength;
	m_friction = friction;
	m_currentTicksForce = ZERO_VECTOR;
	m_duration = 0.0f;
	m_minDuration = 0.0f;
	m_zeroTolerance = zeroTolerance;
}

Force::Force(const Vector3 &direction, float strength, float friction, float minimumDuration, float zeroTolerance)
{
	STACK_TRACE;
	m_active = TRUE;
	m_direction = direction;
	m_strength = strength;
	m_friction = friction;
	m_currentTicksForce = ZERO_VECTOR;
	m_duration = 0.0f;
	m_minDuration = minimumDuration;
	m_zeroTolerance = zeroTolerance;
}

Force::~Force()
{
	STACK_TRACE;
}

void Force::Kill()
{
	STACK_TRACE;
	m_active = FALSE;
	m_currentTicksForce = ZERO_VECTOR;
}

BOOL Force::IsStrengthZero() const
{
	STACK_TRACE;
	if (IsCloseEnough(m_strength, 0.0f, m_zeroTolerance))
		return TRUE;
	else
		return FALSE;
}

void Force::Update(float delta)
{
	STACK_TRACE;
	if (!m_active)
		return;

	if (IsStrengthZero())
	{
		Kill();
		return;
	}

	m_duration += delta;

	m_currentTicksForce = (m_direction * m_strength) * delta;
	m_strength *= m_friction;
}

