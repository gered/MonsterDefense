#ifndef __GAME_ENTITIES_SUBSYSTEMS_MINORVISUALEFFECTSSYSTEM_H_INCLUDED__
#define __GAME_ENTITIES_SUBSYSTEMS_MINORVISUALEFFECTSSYSTEM_H_INCLUDED__

#include "../../../entities/componentsystem.h"

class BlendState;
class EntityManager;
class EventManager;
class RenderContext;
class RenderState;
class VertexBuffer;
struct Event;

class MinorVisualEffectsSystem : public ComponentSystem
{
public:
	static COMPONENTSYSTEM_TYPE GetType()
	{
		static COMPONENTSYSTEM_TYPE typeName = "MinorVisualEffectsSystem";
		return typeName;
	}
	COMPONENTSYSTEM_TYPE GetTypeOf() const                 { return GetType(); }

	MinorVisualEffectsSystem(EntityManager *entityManager, EventManager *eventManager);
	virtual ~MinorVisualEffectsSystem();

	void OnRender(RenderContext *context);
	void OnUpdate(float delta);

	BOOL Handle(const Event *event);

private:
	BlendState *m_shadowBlendState;
	RenderState *m_shadowRenderState;
	VertexBuffer *m_shadowVertices;
};

#endif

