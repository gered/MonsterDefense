#ifndef __GAME_ENTITIES_COMPONENTS_BULLETCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_BULLETCOMPONENT_H_INCLUDED__

#include "../../../framework/common.h"
#include "../../../entities/component.h"

class Entity;

class BulletComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "BulletComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }

	BulletComponent();
	void Reset();

	Entity *source;    // NOTE: this must *always* be validated before using!
	                   //       via EntityManager::IsValid()
	BOOL usePercentDamageAmount;
	float percentDamageAmount;
	uint32_t flatDamageAmount;
	
	// these 2 mainly are here to provide a slight performance benefit in
	// BulletSystem so it doesn't need to constantly query the source entity
	// for what player/npc components it has each time the bullet is updated
	BOOL isOwnedByPlayer;
	BOOL isOwnedByNPC;
};

inline BulletComponent::BulletComponent()
{
	Reset();
}

inline void BulletComponent::Reset()
{
	source = NULL;
	usePercentDamageAmount = FALSE;
	percentDamageAmount = 0.0f;
	flatDamageAmount = 0;
	isOwnedByPlayer = FALSE;
	isOwnedByNPC = FALSE;
}

#endif

