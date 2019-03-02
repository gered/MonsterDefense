#ifndef __GAME_ENTITIES_FORCES_FORCE_H_INCLUDED__
#define __GAME_ENTITIES_FORCES_FORCE_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../framework/math/common.h"
#include "../../../framework/math/vector3.h"
#include <stl/list.h>

class Force
{
public:
	Force(const Vector3 &direction, float strength, float friction, float zeroTolerance = EPSILON);
	Force(const Vector3 &direction, float strength, float friction, float minimumDuration, float zeroTolerance = EPSILON);
	virtual ~Force();

	BOOL IsActive() const                                  { return m_active; }
	void Kill();
	BOOL IsStrengthZero() const;
	BOOL HasMinDurationPassed() const                      { return (m_duration >= m_minDuration); }

	const Vector3& GetCurrentForce() const                 { return m_currentTicksForce; }
	virtual void Update(float delta);

private:
	BOOL m_active;
	Vector3 m_direction;
	float m_strength;
	float m_friction;
	Vector3 m_currentTicksForce;
	float m_duration;
	float m_minDuration;
	float m_zeroTolerance;
};

typedef stl::list<Force*> ForceList;

#endif

