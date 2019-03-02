#ifndef __GAME_ENTITIES_GLOBALCOMPONENTS_POWERUPCOLLECTIONCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_GLOBALCOMPONENTS_POWERUPCOLLECTIONCOMPONENT_H_INCLUDED__

#include "../../../entities/globalcomponent.h"

class PowerUpCollection;

class PowerUpCollectionComponent : public GlobalComponent
{
public:
	static GLOBAL_COMPONENT_TYPE GetType()
	{
		static GLOBAL_COMPONENT_TYPE typeName = "PowerUpCollectionComponent";
		return typeName;
	}
	GLOBAL_COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }
	
	PowerUpCollectionComponent();
	void Reset();
	
	PowerUpCollection *powerUps;
};

inline PowerUpCollectionComponent::PowerUpCollectionComponent()
{
	Reset();
}

inline void PowerUpCollectionComponent::Reset()
{
	powerUps = NULL;
}

#endif

