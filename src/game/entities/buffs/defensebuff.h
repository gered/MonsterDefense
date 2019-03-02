#ifndef __GAME_ENTITIES_BUFFS_DEFENSEBUFF_H_INCLUDED__
#define __GAME_ENTITIES_BUFFS_DEFENSEBUFF_H_INCLUDED__

#include "../../../framework/common.h"
#include "buff.h"

class Entity;
class EntityManager;

class DefenseBuff : public Buff
{
public:
	static BUFF_TYPE GetType()
	{
		static BUFF_TYPE typeName = "DefenseBuff";
		return typeName;
	}
	BUFF_TYPE GetTypeOf() const                          { return GetType(); }
	
	DefenseBuff(EntityManager *entityManager);
	virtual ~DefenseBuff();

	void OnApply(Entity *entity);
	void OnRemove(Entity *entity);
	
	float GetDuration() const                            { return 30.0f; }
	uint32_t GetImageIndex() const                       { return 17; }
	
};

#endif
