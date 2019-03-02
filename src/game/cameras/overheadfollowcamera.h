#ifndef __GAME_CAMERAS_OVERHEADFOLLOWCAMERA_H_INCLUDED__
#define __GAME_CAMERAS_OVERHEADFOLLOWCAMERA_H_INCLUDED__

#include "../../game/direction.h"
#include "../../framework/common.h"
#include "../../framework/math/camera.h"
#include "../../framework/math/vector3.h"

class Entity;
class GraphicsDevice;

class OverheadFollowCamera : public Camera
{
public:
	OverheadFollowCamera(GraphicsDevice *graphicsDevice);
	virtual ~OverheadFollowCamera();

	void SetTarget(Entity *entity)                         { m_entity = entity; }
	void SetElevationDistance(float distance)              { m_elevationDistance = distance; }
	void SetElevationAngle(float angle)                    { m_elevationAngle = angle; }
	
	float GetFacingAngle() const                           { return m_facingAngle; }
	void SetFacingAngle(float angle);
	void RotateBy(float radians);

	void OnUpdate(float delta);

private:
	float GetCurrentYAngle(float delta);
	void UpdatePosition(float yAngle);

	Entity *m_entity;
	Vector3 m_lastEntityPosition;
	float m_elevationDistance;
	float m_elevationAngle;
	
	float m_facingAngle;
	float m_newFacingAngle;
	BOOL m_isShiftingToNewDirection;
	float m_shiftProgress;
	float m_startYAngle;
	float m_endYAngle;
};

#endif
