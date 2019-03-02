#ifndef __GAME_ENTITIES_COMPONENTS_WEAPONCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_WEAPONCOMPONENT_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../entities/component.h"
#include "../weapons/weapon.h"
#include <stl/string.h>

class WeaponComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "WeaponComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }
	
	WeaponComponent();
	void Reset();
	
	Weapon *weapon;
};

inline WeaponComponent::WeaponComponent()
{
	Reset();
}

inline void WeaponComponent::Reset()
{
	weapon = NULL;
}

#endif

