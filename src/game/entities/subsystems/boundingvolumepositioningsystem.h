#ifndef __GAME_ENTITIES_SUBSYSTEMS_BOUNDINGVOLUMEPOSITIONINGSYSTEM_H_INCLUDED__
#define __GAME_ENTITIES_SUBSYSTEMS_BOUNDINGVOLUMEPOSITIONINGSYSTEM_H_INCLUDED__

#include "../../../entities/componentsystem.h"

class EntityManager;
class EventManager;

class BoundingVolumePositioningSystem : public ComponentSystem
{
public:
	static COMPONENTSYSTEM_TYPE GetType()
	{
		static COMPONENTSYSTEM_TYPE typeName = "BoundingVolumePositioningSystem";
		return typeName;
	}
	COMPONENTSYSTEM_TYPE GetTypeOf() const                 { return GetType(); }

	BoundingVolumePositioningSystem(EntityManager *entityManager, EventManager *eventManager);
	virtual ~BoundingVolumePositioningSystem();

	void OnUpdate(float delta);
};

#endif

