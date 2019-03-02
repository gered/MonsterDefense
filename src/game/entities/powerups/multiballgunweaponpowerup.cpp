#include "../../../framework/debug.h"

#include "multiballgunweaponpowerup.h"
#include "../components/billboardcomponent.h"
#include "../components/particlesonpowerupcollectioncomponent.h"
#include "../events/weaponaddedevent.h"
#include "../globalcomponents/contentcachecomponent.h"
#include "../globalcomponents/weaponcollectioncomponent.h"
#include "../presets/particles/sparkleparticlepreset.h"
#include "../weapons/multiballgunweapon.h"
#include "../weapons/weaponcollection.h"
#include "../../../contexts/contentcache.h"
#include "../../../entities/entity.h"
#include "../../../entities/entitymanager.h"
#include "../../../events/eventmanager.h"

MultiBallGunWeaponPowerUp::MultiBallGunWeaponPowerUp(EntityManager *entityManager)
	: PowerUp(entityManager)
{
	STACK_TRACE;
}

MultiBallGunWeaponPowerUp::~MultiBallGunWeaponPowerUp()
{
	STACK_TRACE;
}

void MultiBallGunWeaponPowerUp::OnEntityCreation(Entity *powerUpEntity)
{
	STACK_TRACE;
	ASSERT(powerUpEntity != NULL);
	
	ContentCacheComponent *content = GetEntityManager()->GetGlobalComponent<ContentCacheComponent>();
	ASSERT(content != NULL);
	WeaponCollectionComponent *weapons = GetEntityManager()->GetGlobalComponent<WeaponCollectionComponent>();
	ASSERT(weapons != NULL);
	
	Weapon *pickedUpWeapon = weapons->weapons->Get<MultiBallGunWeapon>();
	ASSERT(pickedUpWeapon != NULL);
	
	BillboardComponent *billboard = powerUpEntity->Add<BillboardComponent>();
	billboard->textureAtlas = content->cache->GetItems();
	billboard->textureAtlasTileIndex = pickedUpWeapon->GetImageIndex();
	billboard->width = 0.5f;
	billboard->height = 0.5f;
	
	ParticlesOnPowerUpCollectionComponent *particlesOnCollection = powerUpEntity->Add<ParticlesOnPowerUpCollectionComponent>();
	particlesOnCollection->particleInfo.particlePreset = SparkleParticlePreset::GetType();
	particlesOnCollection->particleInfo.spawnInRadiusAroundPosition = TRUE;
	particlesOnCollection->particleInfo.minimumAmount = 5;
	particlesOnCollection->particleInfo.maximumAmount = 10;
	particlesOnCollection->particleInfo.lowColor = COLOR_YELLOW;
	particlesOnCollection->particleInfo.highColor = COLOR_YELLOW;
}

void MultiBallGunWeaponPowerUp::OnPickup(Entity *powerUpEntity, Entity *collector)
{
	STACK_TRACE;
	WeaponAddedEvent *e = new WeaponAddedEvent(collector);
	e->type = MultiBallGunWeapon::GetType();
	GetEntityManager()->GetEventManager()->Queue(e);
}
