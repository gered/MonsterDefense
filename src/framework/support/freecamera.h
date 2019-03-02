#ifndef __FRAMEWORK_SUPPORT_FREECAMERA_H_INCLUDED__
#define __FRAMEWORK_SUPPORT_FREECAMERA_H_INCLUDED__

#include "../math/camera.h"

class BaseGameApp;
struct Vector3;

class FreeCamera : public Camera
{
public:
	FreeCamera(BaseGameApp *gameApp);
	virtual ~FreeCamera();

	void Move(float x, float y, float z);
	void Orient(float x, float y, float z);
	void OnUpdate(float delta);

private:
	BaseGameApp *m_gameApp;
	Vector3 m_movement;
	Vector3 m_forward;

};

#endif
