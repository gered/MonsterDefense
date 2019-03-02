#include "../../../framework/debug.h"

#include "powerupsystem.h"
#include "../components/boundingspherecomponent.h"
#include "../components/chestcomponent.h"
#include "../components/chestopenercomponent.h"
#include "../components/collectedpowerupcomponent.h"
#include "../components/playercomponent.h"
#include "../components/positioncomponent.h"
#include "../components/powerupcollectorcomponent.h"
#include "../components/powerupcomponent.h"
#include "../events/entitystatechangeevent.h"
#include "../events/powerupcollectedevent.h"
#include "../events/resetdespawntimerevent.h"
#include "../events/spawndroppedpowerupevent.h"
#include "../globalcomponents/powerupcollectioncomponent.h"
#include "../powerups/powerupcollection.h"
#include "../presets/powerups/poweruppresetproperties.h"
#include "../presets/powerups/poweruppreset.h"
#include "../presets/powerups/pickeduppoweruppreset.h"
#include "../../../entities/entity.h"
#include "../../../entities/entitymanager.h"
#include "../../../entities/components/inactivecomponent.h"
#include "../../../events/eventmanager.h"
#include "../../../framework/math/intersectiontester.h"

PowerUpSystem::PowerUpSystem(EntityManager *entityManager, EventManager *eventManager)
	: ComponentSystem(entityManager, eventManager)
{
	STACK_TRACE;
	ListenFor<SpawnDroppedPowerUpEvent>();
}

PowerUpSystem::~PowerUpSystem()
{
	STACK_TRACE;
	StopListeningFor<SpawnDroppedPowerUpEvent>();
}

void PowerUpSystem::OnUpdate(float delta)
{
	STACK_TRACE;
	EntityList list;
	EntityList powerUpEntities;
	EntityList chestOpenerEntities;
	
	// power up collection by power up collector entities
	GetEntityManager()->GetAllWith<PowerUpComponent>(powerUpEntities);

	if (!powerUpEntities.empty())
	{
		GetEntityManager()->GetAllWith<PowerUpCollectorComponent>(list);
		for (EntityList::iterator i = list.begin(); i != list.end(); ++i)
		{
			Entity *collector = *i;
			BoundingSphereComponent *entityBounds = collector->Get<BoundingSphereComponent>();
			ASSERT(entityBounds != NULL);

			for (EntityList::iterator j = powerUpEntities.begin(); j != powerUpEntities.end(); ++j)
			{
				Entity *powerUpEntity = *j;
				
				// TODO: this check was originally added due to a bug, might not be needed in reality??
				if (powerUpEntity->Has<InactiveComponent>())
					continue;
				
				BoundingSphereComponent *powerUpBounds = powerUpEntity->Get<BoundingSphereComponent>();
				ASSERT(powerUpBounds != NULL);

				// if the collector entity is touching the powerup entity ...
				if (IntersectionTester::Test(entityBounds->bounds, powerUpBounds->bounds))
				{
					// it is, allow them to collect the powerup
					PowerUpComponent *collectedPowerUp = powerUpEntity->Get<PowerUpComponent>();

					// signal powerup collection
					PowerUpCollectedEvent collectionEvent(collector, powerUpEntity);
					GetEntityManager()->GetEventManager()->Trigger(&collectionEvent);

					// powerup-specific handling
					collectedPowerUp->powerUp->OnPickup(powerUpEntity, collector);

					if (collectedPowerUp->powerUp->ShouldCreatedCollectedEntityOnPickup() || collectedPowerUp->shouldRaisePowerUpOnCollect)
					{
						// spawn collected power up entity (the one that raises up to 
						// emphasize power up collection to the player)
						PositionComponent *collectedPowerUpPosition = powerUpEntity->Get<PositionComponent>();
						
						PowerUpPresetProperties collectedEntityArgs;
						collectedEntityArgs.powerUp = collectedPowerUp->powerUp;
						Entity *collected = GetEntityManager()->AddUsingPreset<PickedUpPowerUpPreset>(&collectedEntityArgs);
						
						collected->Get<PositionComponent>()->position = collectedPowerUpPosition->position;
					}
					
					// finally, remove the power up entity that was just collected
					powerUpEntity->Add<InactiveComponent>();
				}
			}
		}
	}
	
	// check if the player is near enough to a closed chest to open it
	list.clear();
	GetEntityManager()->GetAllWith<ChestComponent>(list);
	if (!list.empty())
	{
		GetEntityManager()->GetAllWith<ChestOpenerComponent>(chestOpenerEntities);
		for (EntityList::iterator i = chestOpenerEntities.begin(); i != chestOpenerEntities.end(); ++i)
		{
			Entity *chestOpener = *i;
			BOOL hidePowerUpsOnOpen = chestOpener->Get<ChestOpenerComponent>()->hidePowerUpsOnOpen;
			
			BoundingSphereComponent *chestOpenerBounds = chestOpener->Get<BoundingSphereComponent>();
			ASSERT(chestOpenerBounds != NULL);

			for (EntityList::iterator j = list.begin(); j != list.end(); ++j)
			{
				Entity *chestEntity = *j;
				ChestComponent *chest = chestEntity->Get<ChestComponent>();
				
				// if the chest is currently closed ...
				if (!chest->isOpen)
				{
					BoundingSphereComponent *bounds = chestEntity->Get<BoundingSphereComponent>();
					ASSERT(bounds != NULL);
					
					// ... and the player is touching it ...
					if (IntersectionTester::Test(bounds->bounds, chestOpenerBounds->bounds))
					{
						// open it
						chest->isOpen = TRUE;
						
						// chest state change (for animation purposes)
						EntityStateChangeEvent stateChangeEvent(chestEntity);
						stateChangeEvent.state = ENTITYSTATE_OPEN;
						GetEventManager()->Trigger(&stateChangeEvent);
						
						// reset despawn timer (mainly so the "open" animation is
						// able to completely finish before the chest disappears)
						ResetDespawnTimerEvent resetDespawnTime(chestEntity);
						resetDespawnTime.newTimeTillDespawn = 5.0f;
						resetDespawnTime.alsoResetFlickerTime = TRUE;
						resetDespawnTime.newTimeToFlickerAt = 2.0f;
						GetEventManager()->Trigger(&resetDespawnTime);

						// if the chest has a powerup inside, then give it to the
						// entity that opened the chest
						if (chest->powerUp != NULL)
						{
							// signal powerup collection
							PowerUpCollectedEvent collectionEvent(chestOpener, chestEntity);
							collectionEvent.powerUpEntityIsChestEntity = TRUE;
							GetEntityManager()->GetEventManager()->Trigger(&collectionEvent);
							
							// powerup-specific handling
							chest->powerUp->OnPickup(chestEntity, chestOpener);
							
							if (!hidePowerUpsOnOpen)
							{
								// spawn collected power up entity (the one that raises up to 
								// emphasize power up collection to the player)
								PositionComponent *collectedPowerUpPosition = chestEntity->Get<PositionComponent>();
								
								PowerUpPresetProperties collectedEntityArgs;
								collectedEntityArgs.powerUp = chest->powerUp;
								Entity *collected = GetEntityManager()->AddUsingPreset<PickedUpPowerUpPreset>(&collectedEntityArgs);
								
								collected->Get<PositionComponent>()->position = collectedPowerUpPosition->position;
							}
						}
					}
				}
			}
		}
	}
}

BOOL PowerUpSystem::Handle(const Event *event)
{
	STACK_TRACE;
	if (event->Is<SpawnDroppedPowerUpEvent>())
	{
		const SpawnDroppedPowerUpEvent *e = event->As<SpawnDroppedPowerUpEvent>();
		
		PowerUpCollectionComponent *powerUps = GetEntityManager()->GetGlobalComponent<PowerUpCollectionComponent>();
		
		PowerUpPresetProperties spawnArgs;
		spawnArgs.addPushForce = TRUE;
		spawnArgs.powerUp = powerUps->powerUps->Get(e->powerUpName);
		Entity *newPowerUp = GetEntityManager()->AddUsingPreset<PowerUpPreset>(&spawnArgs);
		
		newPowerUp->Get<PositionComponent>()->position = e->position;
	}
	
	return FALSE;
}

