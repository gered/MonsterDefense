#ifndef __GAME_ENTITIES_PRESETS_BULLETS_BALLBULLETPRESET_H_INCLUDED__
#define __GAME_ENTITIES_PRESETS_BULLETS_BALLBULLETPRESET_H_INCLUDED__

#include "../../../../framework/common.h"
#include "../../../../entities/entitypreset.h"
#include "bulletpreset.h"

class Entity;
class EntityManager;

class BallBulletPreset : public BulletPreset
{
public:
	static ENTITYPRESET_TYPE GetType()
	{
		static ENTITYPRESET_TYPE typeName = "BallBulletPreset";
		return typeName;
	}
	ENTITYPRESET_TYPE GetTypeOf() const                    { return GetType(); }

	BallBulletPreset(EntityManager *entityManager);
	virtual ~BallBulletPreset();
	
	Entity* Create(EntityPresetArgs *args = NULL);
	
	void OnCollision(Entity *entity, Entity *victim);
};

#endif
