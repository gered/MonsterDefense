#ifndef __GAME_ENTITIES_COMPONENTS_BILLBOARDCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_BILLBOARDCOMPONENT_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../entities/component.h"

class Texture;
class TextureAtlas;

class BillboardComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "BillboardComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }

	BillboardComponent();

	void Reset();

	uint32_t textureAtlasTileIndex;
	float width;
	float height;
	BOOL isAxisAligned;
	Texture *texture;
	TextureAtlas *textureAtlas;
};

inline BillboardComponent::BillboardComponent()
{
	Reset();
}

inline void BillboardComponent::Reset()
{
	textureAtlasTileIndex = 0;
	width = 0.0f;
	height = 0.0f;
	isAxisAligned = FALSE;
	texture = NULL;
	textureAtlas = NULL;
}

#endif

