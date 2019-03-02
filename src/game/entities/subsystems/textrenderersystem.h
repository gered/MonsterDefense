#ifndef __GAME_ENTITIES_SUBSYSTEMS_TEXTRENDERERSYSTEM_H_INCLUDED__
#define __GAME_ENTITIES_SUBSYSTEMS_TEXTRENDERERSYSTEM_H_INCLUDED__

#include "../../../entities/componentsystem.h"

class EntityManager;
class EventManager;
class RenderContext;

class TextRendererSystem : public ComponentSystem
{
public:
	static COMPONENTSYSTEM_TYPE GetType()
	{
		static COMPONENTSYSTEM_TYPE typeName = "TextRendererSystem";
		return typeName;
	}
	COMPONENTSYSTEM_TYPE GetTypeOf() const                 { return GetType(); }
	
	TextRendererSystem(EntityManager *entityManager, EventManager *eventManager);
	virtual ~TextRendererSystem();
	
	void OnRender(RenderContext *context);
};

#endif

