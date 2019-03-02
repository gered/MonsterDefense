#ifndef __GAME_ENTITIES_GLOBALCOMPONENTS_BUFFCOLLECTIONCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_GLOBALCOMPONENTS_BUFFCOLLECTIONCOMPONENT_H_INCLUDED__

#include "../../../entities/globalcomponent.h"

class BuffCollection;

class BuffCollectionComponent : public GlobalComponent
{
public:
	static GLOBAL_COMPONENT_TYPE GetType()
	{
		static GLOBAL_COMPONENT_TYPE typeName = "BuffCollectionComponent";
		return typeName;
	}
	GLOBAL_COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }
	
	BuffCollectionComponent();
	void Reset();
	
	BuffCollection *buffs;
};

inline BuffCollectionComponent::BuffCollectionComponent()
{
	Reset();
}

inline void BuffCollectionComponent::Reset()
{
	buffs = NULL;
}

#endif

