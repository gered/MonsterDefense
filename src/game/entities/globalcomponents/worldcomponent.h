#ifndef __GAME_ENTITIES_GLOBALCOMPONENTS_WORLDCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_GLOBALCOMPONENTS_WORLDCOMPONENT_H_INCLUDED__

#include "../../../entities/globalcomponent.h"

class TileMap;

class WorldComponent : public GlobalComponent
{
public:
	static GLOBAL_COMPONENT_TYPE GetType()
	{
		static GLOBAL_COMPONENT_TYPE typeName = "WorldComponent";
		return typeName;
	}
	GLOBAL_COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }

	WorldComponent();
	void Reset();

	TileMap *world;
};

inline WorldComponent::WorldComponent()
{
	Reset();
}

inline void WorldComponent::Reset()
{
	world = NULL;
}

#endif

