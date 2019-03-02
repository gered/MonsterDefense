#ifndef __GAME_ENTITIES_GLOBALCOMPONENTS_GRAPHICSDEVICECOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_GLOBALCOMPONENTS_GRAPHICSDEVICECOMPONENT_H_INCLUDED__

#include "../../../entities/globalcomponent.h"

class GraphicsDevice;

class GraphicsDeviceComponent : public GlobalComponent
{
public:
	static GLOBAL_COMPONENT_TYPE GetType()
	{
		static GLOBAL_COMPONENT_TYPE typeName = "GraphicsDeviceComponent";
		return typeName;
	}
	GLOBAL_COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }

	GraphicsDeviceComponent();
	void Reset();

	GraphicsDevice *graphicsDevice;
};

inline GraphicsDeviceComponent::GraphicsDeviceComponent()
{
	Reset();
}

inline void GraphicsDeviceComponent::Reset()
{
	graphicsDevice = NULL;
}

#endif

