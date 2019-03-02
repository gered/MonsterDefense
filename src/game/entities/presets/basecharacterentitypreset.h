#ifndef __GAME_ENTITIES_PRESETS_BASECHARACTERENTITYPRESET_H_INCLUDED__
#define __GAME_ENTITIES_PRESETS_BASECHARACTERENTITYPRESET_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../entities/entitypreset.h"
#include "../../../entities/entitypresetargs.h"
#include "../../../events/eventlistenerex.h"

class Entity;
class EntityManager;
struct Event;

class BaseCharacterEntityPreset : public EntityPreset, public EventListenerEx
{
public:
	virtual ~BaseCharacterEntityPreset();
	
	virtual Entity* Create(EntityPresetArgs *args = NULL) = 0;
	
	BOOL Handle(const Event *event);
	
	virtual void OnSpawn(Entity *entity)                   {}
	virtual void OnDespawn(Entity *entity)                 {}
	virtual void OnHeal(Entity *entity, Entity *source)    {}
	virtual void OnHurt(Entity *entity, Entity *source)    {}
	virtual void OnKilled(Entity *entity, Entity *killer)  {}
	
protected:
	BaseCharacterEntityPreset(EntityManager *entityManager);

	void SpawnGreenSmokeAt(Entity *entity);
	void SpawnPuffOfSmokeAt(Entity *entity);
};


#endif
