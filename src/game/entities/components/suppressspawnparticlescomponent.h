#ifndef __GAME_ENTITIES_COMPONENTS_SUPPRESSSPAWNPARTICLESCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_SUPPRESSSPAWNPARTICLESCOMPONENT_H_INCLUDED__

#include "../../../entities/component.h"

class SuppressSpawnParticlesComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "SuppressSpawnParticlesComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }
};

#endif

