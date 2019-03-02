#include "../../../framework/debug.h"

#include "../../../framework/common.h"
#include "minorvisualeffectssystem.h"
#include "../components/blobshadowcomponent.h"
#include "../components/boundingspherecomponent.h"
#include "../components/colorcomponent.h"
#include "../components/deadcomponent.h"
#include "../components/flickercomponent.h"
#include "../components/notvisiblecomponent.h"
#include "../components/physicscomponent.h"
#include "../components/positioncomponent.h"
#include "../components/positionoffsetcomponent.h"
#include "../components/particlesondeathcomponent.h"
#include "../components/particlesonpowerupcollectioncomponent.h"
#include "../components/particlesonspawncomponent.h"
#include "../events/despawnedevent.h"
#include "../events/powerupcollectedevent.h"
#include "../events/spawnedevent.h"
#include "../events/spawnparticlesevent.h"
#include "../events/stopflickeringevent.h"
#include "../globalcomponents/contentcachecomponent.h"
#include "../globalcomponents/graphicsdevicecomponent.h"
#include "../globalcomponents/worldcomponent.h"
#include "../../../entities/entity.h"
#include "../../../entities/entitymanager.h"
#include "../../../events/event.h"
#include "../../../events/eventmanager.h"
#include "../../../contexts/contentcache.h"
#include "../../../contexts/rendercontext.h"
#include "../../../framework/graphics/blendstate.h"
#include "../../../framework/graphics/color.h"
#include "../../../framework/graphics/graphicsdevice.h"
#include "../../../framework/graphics/renderstate.h"
#include "../../../framework/graphics/simplecolortextureshader.h"
#include "../../../framework/graphics/standardshader.h"
#include "../../../framework/graphics/texture.h"
#include "../../../framework/graphics/textureatlas.h"
#include "../../../framework/graphics/vertexbuffer.h"
#include "../../../framework/math/boundingsphere.h"
#include "../../../framework/math/camera.h"
#include "../../../framework/math/frustum.h"
#include "../../../framework/math/matrix4x4.h"
#include "../../../framework/math/ray.h"
#include "../../../framework/math/vector3.h"
#include "../../../tilemap/tile.h"
#include "../../../tilemap/tilemap.h"

const unsigned int BLOB_SHADOW_TRIANGLES = 2;
const unsigned int BLOB_SHADOW_VERTICES = BLOB_SHADOW_TRIANGLES * 3;
const float BLOB_SHADOW_ALPHA = 0.4f;
const Color BLOB_SHADOW_COLOR = Color(1.0f, 1.0f, 1.0f, BLOB_SHADOW_ALPHA);

MinorVisualEffectsSystem::MinorVisualEffectsSystem(EntityManager *entityManager, EventManager *eventManager)
	: ComponentSystem(entityManager, eventManager)
{
	STACK_TRACE;
	GraphicsDeviceComponent *graphicsDevice = GetEntityManager()->GetGlobalComponent<GraphicsDeviceComponent>();
	ASSERT(graphicsDevice != NULL);

	m_shadowBlendState = new BLENDSTATE_ALPHABLEND;
	m_shadowRenderState = new RENDERSTATE_DEFAULT;
	m_shadowVertices = new VertexBuffer(BUFFEROBJECT_USAGE_STREAM);
	m_shadowVertices->AddAttribute(VERTEX_POS_3D);
	m_shadowVertices->AddAttribute(VERTEX_TEXCOORD);
	m_shadowVertices->AddAttribute(VERTEX_COLOR);
	m_shadowVertices->Create(BLOB_SHADOW_VERTICES * 30);
	m_shadowVertices->CreateInVRAM();

	graphicsDevice->graphicsDevice->RegisterManagedResource(m_shadowVertices);
	
	ListenFor<SpawnedEvent>();
	ListenFor<DespawnedEvent>();
	ListenFor<PowerUpCollectedEvent>();
	ListenFor<StopFlickeringEvent>();
}

MinorVisualEffectsSystem::~MinorVisualEffectsSystem()
{
	STACK_TRACE;
	GraphicsDeviceComponent *graphicsDevice = GetEntityManager()->GetGlobalComponent<GraphicsDeviceComponent>();
	ASSERT(graphicsDevice != NULL);

	graphicsDevice->graphicsDevice->UnregisterManagedResource(m_shadowVertices);
	
	SAFE_DELETE(m_shadowBlendState);
	SAFE_DELETE(m_shadowRenderState);
	SAFE_DELETE(m_shadowVertices);
	
	StopListeningFor<SpawnedEvent>();
	StopListeningFor<DespawnedEvent>();
	StopListeningFor<PowerUpCollectedEvent>();
	StopListeningFor<StopFlickeringEvent>();
}

void MinorVisualEffectsSystem::OnRender(RenderContext *renderContext)
{
	STACK_TRACE;
	WorldComponent *world = GetEntityManager()->GetGlobalComponent<WorldComponent>();
	ASSERT(world != NULL);
	ContentCacheComponent *contentCache = GetEntityManager()->GetGlobalComponent<ContentCacheComponent>();
	ASSERT(contentCache != NULL);

	EntityList list;

	// draw all blob shadows
	uint32_t numShadows = 0;
	m_shadowVertices->MoveToStart();
	GetEntityManager()->GetAllWith<BlobShadowComponent>(list);
	for (EntityList::iterator i = list.begin(); i != list.end(); ++i)
	{
		Entity *entity = *i;
		
		if (entity->Has<NotVisibleComponent>())
			continue;

		PhysicsComponent *physics = entity->Get<PhysicsComponent>();
		BoundingSphereComponent *bounds = entity->Get<BoundingSphereComponent>();
		PositionComponent *position = entity->Get<PositionComponent>();
		
		ASSERT(position != NULL);
		ASSERT(bounds != NULL);

		// find out where the shadow needs to be drawn at
		Vector3 entityFeetPosition = position->position;
		entityFeetPosition.y -= bounds->bounds.radius;

		Vector3 shadowCenterPoint;
		if (physics == NULL || physics->isOnGround)
		{
			// entity is standing on the ground, so the shadow center point is
			// directly at the entity's feet
			// NOTE: we also assume the entity is on the ground if they don't
			//       even have a physics component at all
			shadowCenterPoint = entityFeetPosition;
		}
		else
		{
			// entity is off the ground, so we need to cast a ray downward
			// and set the shadow center point to the first collision found
			// with the tilemap
			Ray ray;
			ray.position = entityFeetPosition;
			ray.direction = DOWN;

			uint32_t collisionTileX = 0;
			uint32_t collisionTileY = 0;
			uint32_t collisionTileZ = 0;
			if (!world->world->CheckForCollision(ray, collisionTileX, collisionTileY, collisionTileZ))
				continue;        // apparently nothing is below the entity to draw the shadow on...

			shadowCenterPoint.x = entityFeetPosition.x;
			shadowCenterPoint.y = (float)collisionTileY + 1.0f; // shadow is on *top* of the tile the ray collided with
			shadowCenterPoint.z = entityFeetPosition.z;
		}

		// put together a quad for this shadow centered on the XZ plane around
		// the shadow center point
		float halfWidth = bounds->bounds.radius * 0.7f;
		float x1 = shadowCenterPoint.x - halfWidth;
		float x2 = shadowCenterPoint.x + halfWidth;
		float z1 = shadowCenterPoint.z - halfWidth;
		float z2 = shadowCenterPoint.z + halfWidth;
		float y = shadowCenterPoint.y + 0.03f;

		// ensure there are enough vertices in the VBO for this blob shadow
		if (m_shadowVertices->GetRemainingSpace() < BLOB_SHADOW_VERTICES)
			m_shadowVertices->Extend(BLOB_SHADOW_VERTICES);

		uint32_t pos = m_shadowVertices->GetCurrentPosition();

		m_shadowVertices->SetPosition3(pos, x1, y, z2);
		m_shadowVertices->SetTexCoord(pos,0.0f, 1.0f);
		m_shadowVertices->SetColor(pos, BLOB_SHADOW_COLOR);

		m_shadowVertices->SetPosition3(pos + 1, x2, y, z2);
		m_shadowVertices->SetTexCoord(pos + 1,1.0f, 1.0f);
 		m_shadowVertices->SetColor(pos + 1, BLOB_SHADOW_COLOR);

		m_shadowVertices->SetPosition3(pos + 2, x1, y, z1);
		m_shadowVertices->SetTexCoord(pos + 2, 0.0f, 0.0f);
		m_shadowVertices->SetColor(pos + 2, BLOB_SHADOW_COLOR);

		m_shadowVertices->SetPosition3(pos + 3, x2, y, z2);
		m_shadowVertices->SetTexCoord(pos + 3, 1.0f, 1.0f);
		m_shadowVertices->SetColor(pos + 3, BLOB_SHADOW_COLOR);

		m_shadowVertices->SetPosition3(pos + 4, x2, y, z1);
		m_shadowVertices->SetTexCoord(pos + 4, 1.0f, 0.0f);
		m_shadowVertices->SetColor(pos + 4, BLOB_SHADOW_COLOR);

		m_shadowVertices->SetPosition3(pos + 5, x1, y, z1);
		m_shadowVertices->SetTexCoord(pos + 5, 0.0f, 0.0f);
		m_shadowVertices->SetColor(pos + 5, BLOB_SHADOW_COLOR);

		// move position ahead ready for the next blob shadow
		m_shadowVertices->Move(BLOB_SHADOW_VERTICES);
		++numShadows;
	}

	// render the blob shadows
	if (numShadows > 0)
	{
		m_shadowRenderState->Apply();
		m_shadowBlendState->Apply();
		renderContext->GetGraphicsDevice()->BindTexture(contentCache->cache->GetShadow());

		StandardShader *shader = renderContext->GetGraphicsDevice()->GetSimpleColorTextureShader();
		renderContext->GetGraphicsDevice()->BindShader(shader);
		shader->SetModelViewMatrix(renderContext->GetGraphicsDevice()->GetModelViewMatrix());
		shader->SetProjectionMatrix(renderContext->GetGraphicsDevice()->GetProjectionMatrix());

		renderContext->GetGraphicsDevice()->BindVertexBuffer(m_shadowVertices);
		renderContext->GetGraphicsDevice()->RenderTriangles(0, numShadows * BLOB_SHADOW_TRIANGLES);

		renderContext->GetGraphicsDevice()->UnbindShader();
	}
}

void MinorVisualEffectsSystem::OnUpdate(float delta)
{
	STACK_TRACE;
	// flickering
	EntityList list;
	GetEntityManager()->GetAllWith<FlickerComponent>(list);
	for (EntityList::iterator i = list.begin(); i != list.end(); ++i)
	{
		Entity *entity = *i;
		FlickerComponent *flicker = entity->Get<FlickerComponent>();
		ColorComponent *color = entity->Get<ColorComponent>();
		ASSERT(color != NULL);

		if (!flicker->isFlickerTimeInfinite)
		{
			// timer... when it reaches zero, stop flickering
			flicker->flickerTime -= delta;
			if (flicker->flickerTime <= 0.0f)
			{
				color->color = flicker->originalColor;
				entity->Remove<FlickerComponent>();
				continue;
			}
		}

		// on/off flicker color swap. each time the timer reaches zero we
		// swap the colorcomponent color and reset the timer
		flicker->onOffSwapTime -= delta;
		if (flicker->onOffSwapTime < 0.0f)
		{
			color->color = flicker->flickerColorCurrentlyOn ? flicker->originalColor : flicker->color;

			// reset for next
			flicker->flickerColorCurrentlyOn = !flicker->flickerColorCurrentlyOn;
			flicker->onOffSwapTime = flicker->swapTime;
		}
	}
}

BOOL MinorVisualEffectsSystem::Handle(const Event *event)
{
	STACK_TRACE;
	if (event->Is<StopFlickeringEvent>())
	{
		const StopFlickeringEvent *e = event->As<StopFlickeringEvent>();
		FlickerComponent *flicker = e->GetEntity()->Get<FlickerComponent>();
		ASSERT(flicker != NULL);
		ColorComponent *color = e->GetEntity()->Get<ColorComponent>();
		ASSERT(color != NULL);
		
		color->color = flicker->originalColor;
		e->GetEntity()->Remove<FlickerComponent>();
	}
	else if (event->Is<SpawnedEvent>())
	{
		const SpawnedEvent *e = event->As<SpawnedEvent>();
		ParticlesOnSpawnComponent *particlesOnSpawn = e->GetEntity()->Get<ParticlesOnSpawnComponent>();
		if (particlesOnSpawn != NULL)
		{
			PositionComponent *position = e->GetEntity()->Get<PositionComponent>();
			BoundingSphereComponent *bounds = e->GetEntity()->Get<BoundingSphereComponent>();
			
			SpawnParticlesEvent particleEvent;
			particleEvent.particleInfo = particlesOnSpawn->particleInfo;
			
			// automatically set a few things from this entity's own components
			particleEvent.particleInfo.position = position->position;
			if (particleEvent.particleInfo.spawnInRadiusAroundPosition && bounds != NULL)
				particleEvent.particleInfo.radius = bounds->bounds.radius;
			
			GetEventManager()->Trigger(&particleEvent);
		}
	}
	else if (event->Is<DespawnedEvent>())
	{
		const DespawnedEvent *e = event->As<DespawnedEvent>();
		ParticlesOnDeathComponent *particlesOnDeath = e->GetEntity()->Get<ParticlesOnDeathComponent>();
		if (particlesOnDeath != NULL)
		{
			PositionComponent *position = e->GetEntity()->Get<PositionComponent>();
			BoundingSphereComponent *bounds = e->GetEntity()->Get<BoundingSphereComponent>();
			
			SpawnParticlesEvent particleEvent;
			particleEvent.particleInfo = particlesOnDeath->particleInfo;
			
			// automatically set a few things from this entity's own components
			particleEvent.particleInfo.position = position->position;
			if (particleEvent.particleInfo.spawnInRadiusAroundPosition && bounds != NULL)
				particleEvent.particleInfo.radius = bounds->bounds.radius;
			
			GetEventManager()->Trigger(&particleEvent);
		}
	}
	else if (event->Is<PowerUpCollectedEvent>())
	{
		const PowerUpCollectedEvent *e = event->As<PowerUpCollectedEvent>();
		ParticlesOnPowerUpCollectionComponent *particlesOnCollection = e->powerUpEntity->Get<ParticlesOnPowerUpCollectionComponent>();
		if (particlesOnCollection != NULL)
		{
			PositionComponent *position = e->powerUpEntity->Get<PositionComponent>();
			BoundingSphereComponent *bounds = e->powerUpEntity->Get<BoundingSphereComponent>();
			
			SpawnParticlesEvent particleEvent;
			particleEvent.particleInfo = particlesOnCollection->particleInfo;
			
			// automatically set a few things from this entity's own components
			particleEvent.particleInfo.position = position->position;
			if (particleEvent.particleInfo.spawnInRadiusAroundPosition && bounds != NULL)
				particleEvent.particleInfo.radius = bounds->bounds.radius;
			
			GetEventManager()->Trigger(&particleEvent);
		}
	}
	
	return FALSE;
}
