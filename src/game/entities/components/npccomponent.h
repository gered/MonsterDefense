#ifndef __GAME_ENTITIES_COMPONENTS_NPCCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_NPCCOMPONENT_H_INCLUDED__

#include "../../../entities/component.h"

class NPCComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "NPCComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }
};

#endif

