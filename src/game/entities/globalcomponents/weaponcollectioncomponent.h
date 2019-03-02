#ifndef __GAME_ENTITIES_GLOBALCOMPONENTS_WEAPONCOLLECTIONCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_GLOBALCOMPONENTS_WEAPONCOLLECTIONCOMPONENT_H_INCLUDED__

#include "../../../entities/globalcomponent.h"

class WeaponCollection;

class WeaponCollectionComponent : public GlobalComponent
{
public:
	static GLOBAL_COMPONENT_TYPE GetType()
	{
		static GLOBAL_COMPONENT_TYPE typeName = "WeaponCollectionComponent";
		return typeName;
	}
	GLOBAL_COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }
	
	WeaponCollectionComponent();
	void Reset();
	
	WeaponCollection *weapons;
};

inline WeaponCollectionComponent::WeaponCollectionComponent()
{
	Reset();
}

inline void WeaponCollectionComponent::Reset()
{
	weapons = NULL;
}

#endif

