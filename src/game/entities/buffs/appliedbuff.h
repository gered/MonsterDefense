#ifndef __GAME_ENTITIES_BUFFS_APPLIEDBUFF_H_INCLUDED__
#define __GAME_ENTITIES_BUFFS_APPLIEDBUFF_H_INCLUDED__

#include "../../../framework/common.h"
#include "buff.h"

struct AppliedBuff
{
	float remainingTime;
	Buff *buff;
	
	AppliedBuff();
};

inline AppliedBuff::AppliedBuff()
{
	remainingTime = 0.0f;
	buff = NULL;
}

#endif
