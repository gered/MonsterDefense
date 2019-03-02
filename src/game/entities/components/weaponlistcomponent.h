#ifndef __GAME_ENTITIES_COMPONENTS_WEAPONLISTCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_WEAPONLISTCOMPONENT_H_INCLUDED__

#include "../../../entities/component.h"
#include "../weapons/weapon.h"
#include <stl/vector.h>

class WeaponListComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "WeaponListComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }
	
	WeaponListComponent();
	void Reset();
	
	stl::vector<Weapon*> weapons;
};

inline WeaponListComponent::WeaponListComponent()
{
	Reset();
}

inline void WeaponListComponent::Reset()
{
	weapons.clear();
}

#endif

