#ifndef __GAME_ENTITIES_SUBSYSTEMS_SCORESYSTEM_H_INCLUDED__
#define __GAME_ENTITIES_SUBSYSTEMS_SCORESYSTEM_H_INCLUDED__

#include "../../../entities/componentsystem.h"

class EntityManager;
class EventManager;
struct Event;

class ScoreSystem : public ComponentSystem
{
public:
	static COMPONENTSYSTEM_TYPE GetType()
	{
		static COMPONENTSYSTEM_TYPE typeName = "ScoreSystem";
		return typeName;
	}
	COMPONENTSYSTEM_TYPE GetTypeOf() const                 { return GetType(); }
	
	ScoreSystem(EntityManager *entityManager, EventManager *eventManager);
	virtual ~ScoreSystem();
	
	void OnUpdate(float delta);
	
	BOOL Handle(const Event *event);
};

#endif

