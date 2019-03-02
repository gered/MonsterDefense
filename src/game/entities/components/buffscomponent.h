#ifndef __GAME_ENTITIES_COMPONENTS_BUFFSCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_BUFFSCOMPONENT_H_INCLUDED__

#include "../../../entities/component.h"
#include "../buffs/appliedbuff.h"
#include "../buffs/buff.h"
#include <stl/list.h>

typedef stl::list<AppliedBuff> AppliedBuffsList;

class BuffsComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "BuffsComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }
	
	BuffsComponent();
	void Reset();
	
	AppliedBuffsList buffs;
};

inline BuffsComponent::BuffsComponent()
{
	Reset();
}

inline void BuffsComponent::Reset()
{
	buffs.clear();
}

#endif

