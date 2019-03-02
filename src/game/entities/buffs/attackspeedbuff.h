#ifndef __GAME_ENTITIES_BUFFS_ATTACKSPEEDBUFF_H_INCLUDED__
#define __GAME_ENTITIES_BUFFS_ATTACKSPEEDBUFF_H_INCLUDED__

#include "../../../framework/common.h"
#include "buff.h"

class Entity;
class EntityManager;

class AttackSpeedBuff : public Buff
{
public:
	static BUFF_TYPE GetType()
	{
		static BUFF_TYPE typeName = "AttackSpeedBuff";
		return typeName;
	}
	BUFF_TYPE GetTypeOf() const                          { return GetType(); }
	
	AttackSpeedBuff(EntityManager *entityManager);
	virtual ~AttackSpeedBuff();

	void OnApply(Entity *entity);
	void OnRemove(Entity *entity);
	
	float GetDuration() const                            { return 30.0f; }
	uint32_t GetImageIndex() const                       { return 18; }
	
};

#endif
