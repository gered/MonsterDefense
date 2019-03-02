#ifndef __GAME_ENTITIES_SUBSYSTEMS_KEYFRAMEMESHRENDERERSYSTEM_H_INCLUDED__
#define __GAME_ENTITIES_SUBSYSTEMS_KEYFRAMEMESHRENDERERSYSTEM_H_INCLUDED__

#include "../../../entities/componentsystem.h"

class BlendState;
class EntityManager;
class EventManager;
class RenderContext;
class RenderState;

class KeyframeMeshRendererSystem : public ComponentSystem
{
public:
	static COMPONENTSYSTEM_TYPE GetType()
	{
		static COMPONENTSYSTEM_TYPE typeName = "KeyframeMeshRendererSystem";
		return typeName;
	}
	COMPONENTSYSTEM_TYPE GetTypeOf() const                 { return GetType(); }

	KeyframeMeshRendererSystem(EntityManager *entityManager, EventManager *eventManager);
	virtual ~KeyframeMeshRendererSystem();

	void OnRender(RenderContext *context);

private:
	RenderState *m_renderState;
	BlendState *m_defaultBlendState;
	BlendState *m_alphaBlendState;
};

#endif

