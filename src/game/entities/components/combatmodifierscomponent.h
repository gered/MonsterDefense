#ifndef __GAME_ENTITIES_COMPONENTS_COMBATMODIFIERSCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_COMBATMODIFIERSCOMPONENT_H_INCLUDED__

#include "../../../entities/component.h"

class CombatModifiersComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "CombatModifiersComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }
	
	CombatModifiersComponent();
	void Reset();
	
	float damageModifier;
	float defenseModifier;
	float attackCooldownModifier;
};


inline CombatModifiersComponent::CombatModifiersComponent()
{
	Reset();
}

inline void CombatModifiersComponent::Reset()
{
	damageModifier = 1.0f;
	defenseModifier = 1.0f;
	attackCooldownModifier = 1.0f;
}

#endif

