#ifndef __GAME_ENTITIES_PRESETS_BULLETS_SCATTERBULLETPRESET_H_INCLUDED__
#define __GAME_ENTITIES_PRESETS_BULLETS_SCATTERBULLETPRESET_H_INCLUDED__

#include "../../../../framework/common.h"
#include "../../../../entities/entitypreset.h"
#include "bulletpreset.h"

class Entity;
class EntityManager;

class ScatterBulletPreset : public BulletPreset
{
public:
	static ENTITYPRESET_TYPE GetType()
	{
		static ENTITYPRESET_TYPE typeName = "ScatterBulletPreset";
		return typeName;
	}
	ENTITYPRESET_TYPE GetTypeOf() const                    { return GetType(); }
	
	ScatterBulletPreset(EntityManager *entityManager);
	virtual ~ScatterBulletPreset();
	
	Entity* Create(EntityPresetArgs *args = NULL);
	
	void OnCollision(Entity *entity, Entity *victim);
};

#endif
