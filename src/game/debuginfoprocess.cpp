#include "../framework/debug.h"

#include "debuginfoprocess.h"

#include "world.h"
#include "entities/components/animationcomponent.h"
#include "entities/components/playercomponent.h"
#include "entities/components/positioncomponent.h"
#include "entities/components/statecomponent.h"
#include "entities/components/weaponcomponent.h"
#include "gameplay/gameplaystate.h"
#include "../gameapp.h"
#include "../contexts/contentcache.h"
#include "../contexts/rendercontext.h"
#include "../entities/entity.h"
#include "../entities/entitymanager.h"
#include "../framework/content/contentmanager.h"
#include "../framework/content/spritefontparam.h"
#include "../framework/graphics/graphicsdevice.h"
#include "../framework/graphics/spritebatch.h"
#include "../framework/graphics/spritefont.h"
#include "../framework/math/camera.h"
#include "../states/gamestate.h"
#include <stl/string.h>

const Color TEXT_COLOR = COLOR_RED;

stl::string ConvertStateToString(ENTITYSTATES state);

DebugInfoProcess::DebugInfoProcess(GameState *gameState, ProcessManager *processManager)
	: GameProcess(gameState, processManager)
{
	STACK_TRACE;
}

DebugInfoProcess::~DebugInfoProcess()
{
	STACK_TRACE;
}

void DebugInfoProcess::OnRender(RenderContext *renderContext)
{
	STACK_TRACE;
	SpriteFont *font = GetGameApp()->GetContentCache()->GetUIFont();

	uint16_t y = 5;
	renderContext->GetSpriteBatch()->Printf(font, 5, y, TEXT_COLOR, "Loops: %d\nFT: %.6fs\nRPS: %d (%d)\nUPS: %d (%d)", 
		GetGameApp()->GetFPS(), 
		GetGameApp()->GetFrameTime(),
		GetGameApp()->GetRendersPerSecond(), GetGameApp()->GetRenderTime(),
		GetGameApp()->GetUpdatesPerSecond(), GetGameApp()->GetUpdateTime()
		);
	y += font->GetLetterHeight() * 3;

	renderContext->GetSpriteBatch()->Printf(font, 5, (y += font->GetLetterHeight()), TEXT_COLOR, "UI Scale: %d", GetGameApp()->GetScreenScale());
	renderContext->GetSpriteBatch()->Printf(font, 5, (y += font->GetLetterHeight()), TEXT_COLOR, "CP: %f, %f, %f", 
			renderContext->GetGraphicsDevice()->GetCamera()->GetPosition().x, 
			renderContext->GetGraphicsDevice()->GetCamera()->GetPosition().y, 
			renderContext->GetGraphicsDevice()->GetCamera()->GetPosition().z
			);
	
	if (GetGameState()->Is<GamePlayState>())
	{
		y += font->GetLetterHeight();
		
		GamePlayState *state = (GamePlayState*)GetGameState();
		EntityManager *entityManager = state->GetWorld()->GetEntityManager();
		
		Entity *player = entityManager->GetWith<PlayerComponent>();
		if (player != NULL)
			ShowEntityInfo(player, renderContext, y);
	}
}

void DebugInfoProcess::ShowEntityInfo(Entity *entity, RenderContext *renderContext, uint16_t &y)
{
	STACK_TRACE;
	SpriteFont *font = GetGameApp()->GetContentCache()->GetUIFont();
	
	PositionComponent *position = entity->Get<PositionComponent>();
	StateComponent *state = entity->Get<StateComponent>();
	WeaponComponent *weapon = entity->Get<WeaponComponent>();
	AnimationComponent *animation = entity->Get<AnimationComponent>();
	
	// position
	renderContext->GetSpriteBatch()->Printf(font, 5, (y += font->GetLetterHeight()), TEXT_COLOR, "EP: %f, %f, %f", 
											position->position.x, 
											position->position.y, 
											position->position.z
											);
	
	// current ENTITYSTATES value
	stl::string stateName = ConvertStateToString(state->state);
	stl::string tempStateName = ConvertStateToString(state->tempState);
	renderContext->GetSpriteBatch()->Printf(font, 5, (y += font->GetLetterHeight()), TEXT_COLOR, "ES: %s (%d - %s)", 
											stateName.c_str(), 
											state->isInTempState, 
											tempStateName.c_str()
											);
	
	// current animation
	renderContext->GetSpriteBatch()->Printf(font, 5, (y += font->GetLetterHeight()), TEXT_COLOR, "AS: %s (L: %d, CBO: %d), A: %d, F: %d)",
											animation->currentSequenceName,
											animation->isLooping,
											animation->currentSequenceCannotBeOverridden,
											animation->IsAnimating(),
											animation->IsAnimationFinished()
											);
	
	if (weapon != NULL)
		renderContext->GetSpriteBatch()->Printf(font, 5, (y += font->GetLetterHeight()), TEXT_COLOR, "EW: %s", weapon->weapon->GetTypeOf());
	
	y += font->GetLetterHeight();
}

stl::string ConvertStateToString(ENTITYSTATES state)
{
	switch (state)
	{
		case ENTITYSTATE_DEAD:     return "DEAD";
		case ENTITYSTATE_IDLE:     return "IDLE";
		case ENTITYSTATE_FALLING:  return "FALLING";
		case ENTITYSTATE_INACTIVE: return "INACTIVE";
		case ENTITYSTATE_JUMPING:  return "JUMPING";
		case ENTITYSTATE_NONE:     return "NONE";
		case ENTITYSTATE_PUNCHING: return "PUNCHING";
		case ENTITYSTATE_SHOOTING: return "SHOOTING";
		case ENTITYSTATE_WALKING:  return "WALKING";
		case ENTITYSTATE_OPEN:     return "OPEN";
		default:                   return "<unknown>";
	}
}
