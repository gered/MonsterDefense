#ifndef __GAME_ENTITIES_SUBSYSTEMS_BILLBOARDRENDERERSYSTEM_H_INCLUDED__
#define __GAME_ENTITIES_SUBSYSTEMS_BILLBOARDRENDERERSYSTEM_H_INCLUDED__

#include "../../../entities/componentsystem.h"

class EntityManager;
class EventManager;
class PositionComponent;
class RenderContext;
class WorldComponent;
struct Color;

class BillboardRendererSystem : public ComponentSystem
{
public:
	static COMPONENTSYSTEM_TYPE GetType()
	{
		static COMPONENTSYSTEM_TYPE typeName = "BillboardRendererSystem";
		return typeName;
	}
	COMPONENTSYSTEM_TYPE GetTypeOf() const                 { return GetType(); }

	BillboardRendererSystem(EntityManager *entityManager, EventManager *eventManager);
	virtual ~BillboardRendererSystem();

	void OnRender(RenderContext *context);

private:
	Color GetLightingColor(PositionComponent *position, WorldComponent *world, const Color &entityColor);
};

#endif

