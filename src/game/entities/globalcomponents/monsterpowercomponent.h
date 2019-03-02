#ifndef __GAME_ENTITIES_GLOBALCOMPONENTS_MONSTERPOWERCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_GLOBALCOMPONENTS_MONSTERPOWERCOMPONENT_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../entities/globalcomponent.h"

class MonsterPowerComponent : public GlobalComponent
{
public:
	static GLOBAL_COMPONENT_TYPE GetType()
	{
		static GLOBAL_COMPONENT_TYPE typeName = "MonsterPowerComponent";
		return typeName;
	}
	GLOBAL_COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }
	
	MonsterPowerComponent();
	void Reset();

	float attackModifier;
	float defenseModifier;
	float spawnRateModifier;
	
	float attackPercentAboveBase;
	float defensePercentAboveBase;
	float spawnRatePercentAboveBase;
};

inline MonsterPowerComponent::MonsterPowerComponent()
{
	Reset();
}

inline void MonsterPowerComponent::Reset()
{
	attackModifier = 1.0f;
	defenseModifier = 1.0f;
	spawnRateModifier = 1.0f;
	
	attackPercentAboveBase = 0.0f;
	defensePercentAboveBase = 0.0f;
	spawnRatePercentAboveBase = 0.0f;
}

#endif

