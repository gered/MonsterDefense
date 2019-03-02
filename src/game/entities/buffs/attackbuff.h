#ifndef __GAME_ENTITIES_BUFFS_ATTACKBUFF_H_INCLUDED__
#define __GAME_ENTITIES_BUFFS_ATTACKBUFF_H_INCLUDED__

#include "../../../framework/common.h"
#include "buff.h"

class Entity;
class EntityManager;

class AttackBuff : public Buff
{
public:
	static BUFF_TYPE GetType()
	{
		static BUFF_TYPE typeName = "AttackBuff";
		return typeName;
	}
	BUFF_TYPE GetTypeOf() const                          { return GetType(); }
	
	AttackBuff(EntityManager *entityManager);
	virtual ~AttackBuff();

	void OnApply(Entity *entity);
	void OnRemove(Entity *entity);
	
	float GetDuration() const                            { return 30.0f; }
	uint32_t GetImageIndex() const                       { return 16; }
	
};

#endif
