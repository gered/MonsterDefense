#include "../framework/debug.h"

#include "world.h"

#include "entities/buffs/buffcollection.h"
#include "entities/buffs/attackbuff.h"
#include "entities/buffs/attackspeedbuff.h"
#include "entities/buffs/defensebuff.h"

#include "entities/globalcomponents/buffcollectioncomponent.h"
#include "entities/globalcomponents/contentcachecomponent.h"
#include "entities/globalcomponents/contentmanagercomponent.h"
#include "entities/globalcomponents/graphicsdevicecomponent.h"
#include "entities/globalcomponents/monsterpowercomponent.h"
#include "entities/globalcomponents/powerupcollectioncomponent.h"
#include "entities/globalcomponents/weaponcollectioncomponent.h"
#include "entities/globalcomponents/worldcomponent.h"

#include "entities/presets/playerpreset.h"

#include "entities/presets/bullets/ballbulletpreset.h"
#include "entities/presets/bullets/scatterbulletpreset.h"

#include "entities/presets/monsters/bluezombiepreset.h"
#include "entities/presets/monsters/greenzombiepreset.h"
#include "entities/presets/monsters/purplezombiepreset.h"
#include "entities/presets/monsters/redzombiepreset.h"

#include "entities/presets/objects/chestpreset.h"

#include "entities/presets/particles/heartparticlepreset.h"
#include "entities/presets/particles/smokeparticlepreset.h"
#include "entities/presets/particles/sparkparticlepreset.h"
#include "entities/presets/particles/sparkleparticlepreset.h"
#include "entities/presets/particles/starparticlepreset.h"
#include "entities/presets/particles/textparticlepreset.h"

#include "entities/presets/powerups/pickeduppoweruppreset.h"
#include "entities/presets/powerups/poweruppreset.h"

#include "entities/powerups/powerupcollection.h"
#include "entities/powerups/attackbuffpowerup.h"
#include "entities/powerups/attackspeedbuffpowerup.h"
#include "entities/powerups/ballgunweaponpowerup.h"
#include "entities/powerups/bluegempowerup.h"
#include "entities/powerups/defensebuffpowerup.h"
#include "entities/powerups/greengempowerup.h"
#include "entities/powerups/heartpowerup.h"
#include "entities/powerups/multiballgunweaponpowerup.h"
#include "entities/powerups/redgempowerup.h"
#include "entities/powerups/scattergunweaponpowerup.h"
#include "entities/powerups/yellowgempowerup.h"

#include "entities/subsystems/animationsystem.h"
#include "entities/subsystems/attacksystem.h"
#include "entities/subsystems/billboardrenderersystem.h"
#include "entities/subsystems/boundingvolumepositioningsystem.h"
#include "entities/subsystems/buffssystem.h"
#include "entities/subsystems/bulletsystem.h"
#include "entities/subsystems/entitystatesystem.h"
#include "entities/subsystems/keyframemeshrenderersystem.h"
#include "entities/subsystems/lifeanddeathsystem.h"
#include "entities/subsystems/minorvisualeffectssystem.h"
#include "entities/subsystems/npcsystem.h"
#include "entities/subsystems/offseteffectsystem.h"
#include "entities/subsystems/particlesystem.h"
#include "entities/subsystems/physicssystem.h"
#include "entities/subsystems/powerupsystem.h"
#include "entities/subsystems/scoresystem.h"
#include "entities/subsystems/simplephysicssystem.h"
#include "entities/subsystems/textrenderersystem.h"
#include "entities/subsystems/weaponsystem.h"

#include "entities/weapons/weaponcollection.h"
#include "entities/weapons/ballgunweapon.h"
#include "entities/weapons/fistweapon.h"
#include "entities/weapons/fistwithsmallknockbackweapon.h"
#include "entities/weapons/multiballgunweapon.h"
#include "entities/weapons/scattergunweapon.h"

#include "../gameapp.h"
#include "../contexts/contentcache.h"
#include "../entities/entitymanager.h"

void World::RegisterEntityDataClasses()
{
	STACK_TRACE;
	ASSERT(m_entityManager != NULL);
	
	m_powerUps = new PowerUpCollection(m_entityManager);
	m_powerUps->Add<BlueGemPowerUp>();
	m_powerUps->Add<GreenGemPowerUp>();
	m_powerUps->Add<RedGemPowerUp>();
	m_powerUps->Add<YellowGemPowerUp>();
	m_powerUps->Add<HeartPowerUp>();
	m_powerUps->Add<AttackBuffPowerUp>();
	m_powerUps->Add<AttackSpeedBuffPowerUp>();
	m_powerUps->Add<DefenseBuffPowerUp>();
	m_powerUps->Add<BallGunWeaponPowerUp>();
	m_powerUps->Add<MultiBallGunWeaponPowerUp>();
	m_powerUps->Add<ScatterGunWeaponPowerUp>();
	
	m_buffs = new BuffCollection(m_entityManager);
	m_buffs->Add<AttackBuff>();
	m_buffs->Add<AttackSpeedBuff>();
	m_buffs->Add<DefenseBuff>();
	
	m_weapons = new WeaponCollection(m_entityManager);
	m_weapons->Add<BallGunWeapon>();
	m_weapons->Add<FistWeapon>();
	m_weapons->Add<FistWithSmallKnockbackWeapon>();
	m_weapons->Add<MultiBallGunWeapon>();
	m_weapons->Add<ScatterGunWeapon>();
	
	m_entityManager->AddGlobalComponent<ContentCacheComponent>()->cache = m_gameApp->GetContentCache();
	m_entityManager->AddGlobalComponent<ContentManagerComponent>()->content = m_gameApp->GetContentManager();
	m_entityManager->AddGlobalComponent<GraphicsDeviceComponent>()->graphicsDevice = m_gameApp->GetGraphicsDevice();
	m_entityManager->AddGlobalComponent<MonsterPowerComponent>();
	m_entityManager->AddGlobalComponent<PowerUpCollectionComponent>()->powerUps = m_powerUps;
	m_entityManager->AddGlobalComponent<BuffCollectionComponent>()->buffs = m_buffs;
	m_entityManager->AddGlobalComponent<WeaponCollectionComponent>()->weapons = m_weapons;
	m_entityManager->AddGlobalComponent<WorldComponent>()->world = m_tileMap;
	
	// order matters!
	// generally we try to do stuff that changes entity position first
	// and rendering last
	m_entityManager->AddSubsystem<PhysicsSystem>();
    m_entityManager->AddSubsystem<SimplePhysicsSystem>();
	m_entityManager->AddSubsystem<EntityStateSystem>();
	m_entityManager->AddSubsystem<BuffsSystem>();
	m_entityManager->AddSubsystem<WeaponSystem>();
	m_entityManager->AddSubsystem<AttackSystem>();
	m_entityManager->AddSubsystem<NPCSystem>();
	m_entityManager->AddSubsystem<BulletSystem>();
	m_entityManager->AddSubsystem<PowerUpSystem>();
	m_entityManager->AddSubsystem<LifeAndDeathSystem>();
	m_entityManager->AddSubsystem<ScoreSystem>();
	m_entityManager->AddSubsystem<AnimationSystem>();
	m_entityManager->AddSubsystem<ParticleSystem>();
	m_entityManager->AddSubsystem<MinorVisualEffectsSystem>();
	m_entityManager->AddSubsystem<OffsetEffectSystem>();
	m_entityManager->AddSubsystem<BoundingVolumePositioningSystem>();
	m_entityManager->AddSubsystem<BillboardRendererSystem>();
	m_entityManager->AddSubsystem<TextRendererSystem>();
	m_entityManager->AddSubsystem<KeyframeMeshRendererSystem>();
	
	
	m_entityManager->AddPreset<PlayerPreset>();

	// objects
	m_entityManager->AddPreset<ChestPreset>();
	
	// monsters
	m_entityManager->AddPreset<BlueZombiePreset>();
	m_entityManager->AddPreset<GreenZombiePreset>();
	m_entityManager->AddPreset<PurpleZombiePreset>();
	m_entityManager->AddPreset<RedZombiePreset>();
	
	// bullets
	m_entityManager->AddPreset<BallBulletPreset>();
	m_entityManager->AddPreset<ScatterBulletPreset>();	
	
	// particles
	m_entityManager->AddPreset<SparkParticlePreset>();
	m_entityManager->AddPreset<SparkleParticlePreset>();
	m_entityManager->AddPreset<SmokeParticlePreset>();
	m_entityManager->AddPreset<StarParticlePreset>();
	m_entityManager->AddPreset<HeartParticlePreset>();
	
	// powerups
	m_entityManager->AddPreset<PowerUpPreset>();
	m_entityManager->AddPreset<PickedUpPowerUpPreset>();
	
	// misc
	m_entityManager->AddPreset<TextParticlePreset>();
}
