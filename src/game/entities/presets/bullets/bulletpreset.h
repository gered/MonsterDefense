#ifndef __GAME_ENTITIES_PRESETS_BULLETS_BULLETPRESET_H_INCLUDED__
#define __GAME_ENTITIES_PRESETS_BULLETS_BULLETPRESET_H_INCLUDED__

#include "../../../../framework/common.h"
#include "../../../../entities/entitypreset.h"
#include "../../../../events/eventlistenerex.h"

class Entity;
class EntityManager;
struct Event;

class BulletPreset : public EntityPreset, public EventListenerEx
{
public:
	virtual ~BulletPreset();

	Entity* Create(EntityPresetArgs *args = NULL);
	
	virtual void OnCollision(Entity *entity, Entity *victim) {}
	
	BOOL Handle(const Event *event);

protected:
	BulletPreset(EntityManager *entityManager);
};

#endif
