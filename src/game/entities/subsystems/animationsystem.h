#ifndef __GAME_ENTITIES_SUBSYSTEMS_ANIMATIONSYSTEM_H_INCLUDED__
#define __GAME_ENTITIES_SUBSYSTEMS_ANIMATIONSYSTEM_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../entities/componentsystem.h"

class AnimationComponent;
class BillboardComponent;
class Camera;
class EntityManager;
class EventManager;
class KeyframeMeshComponent;
class OrientationXZComponent;
class RenderContext;
struct Event;

class AnimationSystem : public ComponentSystem
{
public:
	static COMPONENTSYSTEM_TYPE GetType()
	{
		static COMPONENTSYSTEM_TYPE typeName = "AnimationSystem";
		return typeName;
	}
	COMPONENTSYSTEM_TYPE GetTypeOf() const                 { return GetType(); }

	AnimationSystem(EntityManager *entityManager, EventManager *eventManager);
	virtual ~AnimationSystem();

	void OnUpdate(float delta);
	void OnRender(RenderContext *context);

	BOOL Handle(const Event *event);

private:
	void UpdateBillboardFrame(BillboardComponent *billboard, AnimationComponent *animation, OrientationXZComponent *orientation, const Camera *camera);
	void UpdateKeyframeMeshFrame(KeyframeMeshComponent *keyframeMesh, AnimationComponent *animation);
};

#endif

