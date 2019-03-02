#ifndef __GAME_ENTITIES_COMPONENTS_LIFECOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_LIFECOMPONENT_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../entities/component.h"
#include <stdlib.h>

class Entity;

class LifeComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "LifeComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }

	LifeComponent();
	void Reset();

	uint32_t health;
	uint32_t maxHealth;
	BOOL wasHurt;
	BOOL wasHealed;
	
	Entity *lastSourceOfDamage;
	Entity *lastSourceOfHealing;

	LifeComponent* AdjustHealth(int32_t amount);
	LifeComponent* AdjustMaxHealth(int32_t amount);
	LifeComponent* Hurt(uint32_t amount, float modifier = 1.0f);
	LifeComponent* Hurt(float percent, float modifier = 1.0f);
	LifeComponent* Heal(uint32_t amount, float modifier = 1.0f);
	LifeComponent* Heal(float percent, float modifier = 1.0f);

	float GetHealthPercent() const;
};

inline LifeComponent::LifeComponent()
{
	Reset();
}

inline void LifeComponent::Reset()
{
	health = 0;
	maxHealth = 0;
	wasHurt = FALSE;
	wasHealed = FALSE;
	lastSourceOfDamage = NULL;
	lastSourceOfHealing = NULL;
}

inline LifeComponent* LifeComponent::AdjustHealth(int32_t amount)
{
	if (amount < 0 && abs(amount) >= (int32_t)health)
		health = 0;
	else
	{
		health += amount;
		if (health > maxHealth)
			health = maxHealth;
	}

	return this;
}

inline LifeComponent* LifeComponent::AdjustMaxHealth(int32_t amount)
{
	if (amount < 0 && abs(amount) >= (int32_t)maxHealth)
		maxHealth = 0;
	else
	{
		maxHealth += amount;
		if (maxHealth < health)
			health = maxHealth;
	}

	return this;
}

inline LifeComponent* LifeComponent::Hurt(uint32_t amount, float modifier)
{
	amount *= modifier;
	
	if (amount >= health)
		health = 0;
	else
		health -= amount;

	wasHurt = TRUE;

	return this;
}

inline LifeComponent* LifeComponent::Hurt(float percent, float modifier)
{
	percent *= modifier;
	if (percent > 1.0f)
		percent = 1.0f;
	
	uint32_t amount = (uint32_t)(maxHealth * percent);
	return Hurt(amount);
}

inline LifeComponent* LifeComponent::Heal(uint32_t amount, float modifier)
{
	amount *= modifier;
	
	health += amount;
	if (health > maxHealth)
		health = maxHealth;

	wasHealed = TRUE;

	return this;
}

inline LifeComponent* LifeComponent::Heal(float percent, float modifier)
{
	percent *= modifier;
	if (percent > 1.0f)
		percent = 1.0f;
	
	uint32_t amount = (uint32_t)(maxHealth * percent);
	return Heal(amount);
}

inline float LifeComponent::GetHealthPercent() const
{
	return (float)health / (float)maxHealth;
}

#endif

