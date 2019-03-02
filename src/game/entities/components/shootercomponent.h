#ifndef __GAME_ENTITIES_COMPONENTS_SHOOTERCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_SHOOTERCOMPONENT_H_INCLUDED__

#include "../../../entities/component.h"
#include "../../../entities/entitypreset.h"

class ShooterComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "ShooterComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }

	ShooterComponent();
	void Reset();

	ENTITYPRESET_TYPE bulletPreset;
};

inline ShooterComponent::ShooterComponent()
{
	Reset();
}

inline void ShooterComponent::Reset()
{
	bulletPreset = NULL;
}

#endif

