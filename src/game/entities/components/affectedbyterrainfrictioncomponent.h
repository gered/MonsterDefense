#ifndef __GAME_ENTITIES_COMPONENTS_AFFECTEDBYTERRAINFRICTION_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_AFFECTEDBYTERRAINFRICTION_H_INCLUDED__

#include "../../../entities/component.h"

class AffectedByTerrainFrictionComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "AffectedByTerrainFrictionComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }
};

#endif

