#ifndef __GAME_ENTITIES_COMPONENTS_KEYFRAMEMESHCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_KEYFRAMEMESHCOMPONENT_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../entities/component.h"

class KeyframeMesh;
class Texture;

class KeyframeMeshComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "KeyframeMeshComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }

	KeyframeMeshComponent();
	void Reset();

	uint32_t startFrame;
	uint32_t endFrame;
	float interpolation;
	KeyframeMesh *mesh;
	Texture *texture;
};

inline KeyframeMeshComponent::KeyframeMeshComponent()
{
	Reset();
}

inline void KeyframeMeshComponent::Reset()
{
	startFrame = 0;
	endFrame = 0;
	interpolation = 0.0f;
	mesh = NULL;
	texture = NULL;
}

#endif

