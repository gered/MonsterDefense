#include "../../../../framework/debug.h"

#include "zombiepreset.h"
#include "../../components/animationcomponent.h"
#include "../../components/autoanimationcomponent.h"
#include "../../components/boundingspherecomponent.h"
#include "../../components/canbeattackedcomponent.h"
#include "../../components/keyframemeshcomponent.h"
#include "../../components/particlesondeathcomponent.h"
#include "../../components/particlesonspawncomponent.h"
#include "../../components/physicscomponent.h"
#include "../../components/positioncomponent.h"
#include "../../components/renderoffsetcomponent.h"
#include "../../events/spawndroppedpowerupevent.h"
#include "../../powerups/powerup.h"
#include "../../powerups/bluegempowerup.h"
#include "../../powerups/greengempowerup.h"
#include "../../powerups/heartpowerup.h"
#include "../../powerups/redgempowerup.h"
#include "../../powerups/yellowgempowerup.h"
#include "../../powerups/ballgunweaponpowerup.h"
#include "../../powerups/multiballgunweaponpowerup.h"
#include "../../powerups/scattergunweaponpowerup.h"
#include "../../globalcomponents/contentmanagercomponent.h"
#include "../../../physicsconstants.h"
#include "../../../../contexts/contentcache.h"
#include "../../../../entities/entity.h"
#include "../../../../entities/entitymanager.h"
#include "../../../../events/eventmanager.h"
#include "../../../../framework/content/contentmanager.h"
#include "../../../../framework/assets/animation/keyframemesh.h"

ZombiePreset::ZombiePreset(EntityManager *entityManager)
	: MonsterPreset(entityManager)
{
	STACK_TRACE;
}

ZombiePreset::~ZombiePreset()
{
	STACK_TRACE;
}

Entity* ZombiePreset::Create(EntityPresetArgs *args)
{
	STACK_TRACE;
	Entity *entity = MonsterPreset::Create(args);
	
	ContentManagerComponent *content = GetEntityManager()->GetGlobalComponent<ContentManagerComponent>();
	ASSERT(content != NULL);

	KeyframeMesh *mesh = content->content->Get<KeyframeMesh>("assets://characters/zombie.mesh");

	entity->Get<AnimationComponent>()
		->AddSequence("idle", *mesh->GetAnimation("IDLE"), 0.05f)
		->AddSequence("walk", *mesh->GetAnimation("WALK"), 0.02f)
		->AddSequence("dead", *mesh->GetAnimation("KILLED"), 0.10f)
		->AddSequence("punch", *mesh->GetAnimation("BASH_LEFT_2"), 0.1f);

	entity->Get<AutoAnimationComponent>()
		->Set(ENTITYSTATE_IDLE, "idle")
		->Set(ENTITYSTATE_WALKING, "walk")
		->Set(ENTITYSTATE_DEAD, "dead", FALSE, TRUE)
		->Set(ENTITYSTATE_PUNCHING, "punch", FALSE, TRUE);
	
	entity->Get<BoundingSphereComponent>()
		->bounds.radius = 0.49f;

	PhysicsComponent *physics = entity->Get<PhysicsComponent>();
	physics->friction = FRICTION_NORMAL;
	physics->maxWalkSpeed = 6.0f;
	physics->walkingAcceleration = 4.0f;
	physics->SetBoundsRadius(0.49f);

	entity->Add<RenderOffsetComponent>()
		->offset = Vector3(0.0f, -0.5f, 0.0f);

	KeyframeMeshComponent *keyframeMesh = entity->Add<KeyframeMeshComponent>();
	keyframeMesh->mesh = mesh;

	CanBeAttackedComponent *canBeAttacked = entity->Add<CanBeAttackedComponent>();
	canBeAttacked->byPlayer = TRUE;
	canBeAttacked->byNPC = TRUE;
	
	return entity;
}

void ZombiePreset::OnKilled(Entity *entity, Entity *killer)
{
	STACK_TRACE;
	POWERUP_TYPE normalDrops[] = {
		BlueGemPowerUp::GetType(),
		GreenGemPowerUp::GetType(),
		RedGemPowerUp::GetType(),
		YellowGemPowerUp::GetType(),
		HeartPowerUp::GetType()
	};
	
	POWERUP_TYPE weaponDrops[] = {
		BallGunWeaponPowerUp::GetType(),
		MultiBallGunWeaponPowerUp::GetType(),
		ScatterGunWeaponPowerUp::GetType()
	};
	
	// 50% chance to drop something or not
	float chanceToDropSomethingRoll = Rnd(0.0f, 1.0f);
	if (chanceToDropSomethingRoll < 0.5f)
	{
		SpawnDroppedPowerUpEvent powerUpDrop;
		powerUpDrop.position = entity->Get<PositionComponent>()->position;

		float chanceToDropWeaponRoll = Rnd(0.0f, 1.0f);
		if (chanceToDropWeaponRoll < 0.1f)
		{
			// weapon drop
			powerUpDrop.powerUpName = weaponDrops[Rnd(0, 2)];
			GetEventManager()->Trigger(&powerUpDrop);
		}
		else
		{
			// normal drops
			int numDrops = Rnd(1, 3);
			for (int i = 0; i < numDrops; ++i)
			{
				powerUpDrop.powerUpName = normalDrops[Rnd(0, 4)];
				GetEventManager()->Trigger(&powerUpDrop);
			}
		}
		
	}
}
