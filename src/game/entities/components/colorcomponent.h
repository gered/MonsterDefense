#ifndef __GAME_ENTITIES_COMPONENTS_COLORCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_COLORCOMPONENT_H_INCLUDED__

#include "../../../entities/component.h"
#include "../../../framework/graphics/color.h"

class ColorComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "ColorComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }

	ColorComponent();
	void Reset();

	Color color;
};

inline ColorComponent::ColorComponent()
{
	Reset();
}

inline void ColorComponent::Reset()
{
	color = COLOR_WHITE;
}

#endif

