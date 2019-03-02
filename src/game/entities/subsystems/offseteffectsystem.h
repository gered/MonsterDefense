#ifndef __GAME_ENTITIES_SUBSYSTEMS_OFFSETEFFECTSYSTEM_H_INCLUDED__
#define __GAME_ENTITIES_SUBSYSTEMS_OFFSETEFFECTSYSTEM_H_INCLUDED__

#include "../../../entities/componentsystem.h"

class CollectedPowerUpComponent;
class Entity;
class EntityManager;
class EventManager;
class FloatBounceComponent;
class PositionOffsetComponent;

class OffsetEffectSystem : public ComponentSystem
{
public:
	static COMPONENTSYSTEM_TYPE GetType()
	{
		static COMPONENTSYSTEM_TYPE typeName = "OffsetEffectSystem";
		return typeName;
	}
	COMPONENTSYSTEM_TYPE GetTypeOf() const                 { return GetType(); }

	OffsetEffectSystem(EntityManager *entityManager, EventManager *eventManager);
	virtual ~OffsetEffectSystem();

	void OnUpdate(float delta);

private:
	void HandleFloatBounce(PositionOffsetComponent *positionOffset, FloatBounceComponent *floatBounce, float delta);
	void HandleCollectedPowerUp(PositionOffsetComponent *positionOffset, CollectedPowerUpComponent *collectedPowerUp, Entity *entity, float delta);
};

#endif

