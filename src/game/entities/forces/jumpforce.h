#ifndef __GAME_ENTITIES_FORCES_JUMPFORCE_H_INCLUDED__
#define __GAME_ENTITIES_FORCES_JUMPFORCE_H_INCLUDED__

#include "force.h"
#include "../../../framework/common.h"
#include "../../../framework/math/vector3.h"

class PhysicsComponent;

class JumpForce : public Force
{
public:
	JumpForce(const Vector3 &direction, float strength, float friction, const PhysicsComponent *physicsComponent);
	virtual ~JumpForce();

	void Update(float delta);

private:
	const PhysicsComponent *m_physicsComponent;
};

#endif

