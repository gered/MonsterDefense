#ifndef __GAME_ENTITIES_COMPONENTS_BLOBSHADOWCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_BLOBSHADOWCOMPONENT_H_INCLUDED__

#include "../../../entities/component.h"

class BlobShadowComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "BlobShadowComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }
};

#endif
