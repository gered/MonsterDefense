#ifndef __GAME_ENTITIES_COMPONENTS_TEXTCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_TEXTCOMPONENT_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../entities/component.h"
#include "../../../framework/graphics/spritefont.h"
#include "../../../framework/math/vector3.h"

const int TEXTCOMPONENT_MAX_LENGTH = 32;

class TextComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "TextComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }
	
	TextComponent();
	void Reset();

	char text[TEXTCOMPONENT_MAX_LENGTH];
	SpriteFont *font;
	Vector3 offset;
	BOOL useBillboardRendering;
	float billboardSizePerPixel;
};

inline TextComponent::TextComponent()
{
	Reset();
}

inline void TextComponent::Reset()
{
	text[0] = '\0';
	font = NULL;
	offset = ZERO_VECTOR;
	useBillboardRendering = FALSE;
	billboardSizePerPixel = 0.0f;
}

#endif

