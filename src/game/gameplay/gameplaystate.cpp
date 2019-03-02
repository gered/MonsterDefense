#include "../../framework/debug.h"
#include "../../framework/log.h"
#include "../../framework/common.h"

#include "gameplaystate.h"

#include <stl/string.h>

#include "../debuginfoprocess.h"
#include "introcountdownstate.h"
#include "missionfailurestate.h"
#include "missionsuccessstate.h"
#include "pausemenuui.h"
#include "statusui.h"
#include "../gamemodesettings.h"
#include "../gameoptions.h"
#include "../world.h"
#include "../cameras/overheadfollowcamera.h"
#include "../entities/components/deadcomponent.h"
#include "../entities/components/physicscomponent.h"
#include "../entities/components/playercomponent.h"
#include "../entities/components/positioncomponent.h"
#include "../entities/components/orientationxzcomponent.h"
#include "../entities/components/scorecomponent.h"
#include "../entities/events/despawnedevent.h"
#include "../entities/events/hurtevent.h"
#include "../entities/events/moveforwardevent.h"
#include "../entities/events/moveindirectionevent.h"
#include "../entities/events/jumpevent.h"
#include "../entities/events/attackevent.h"
#include "../entities/events/weaponswitchevent.h"
#include "../entities/forces/force.h"
#include "../events/introfinishedevent.h"
#include "../events/optionsupdatedevent.h"
#include "../events/quitgameevent.h"
#include "../events/timerpauseevent.h"
#include "../events/timerresumeevent.h"
#include "../events/timerranoutevent.h"
#include "../events/timersetevent.h"
#include "../levels/testlevel.h"
#include "../mainmenu/mainmenustate.h"
#include "../../gameapp.h"
#include "../../contexts/rendercontext.h"
#include "../../effects/dimeffect.h"
#include "../../effects/effectmanager.h"
#include "../../effects/fadeeffect.h"
#include "../../entities/entity.h"
#include "../../entities/entitymanager.h"
#include "../../events/event.h"
#include "../../events/eventmanager.h"
#include "../../framework/content/contentmanager.h"
#include "../../framework/graphics/graphicsdevice.h"
#include "../../framework/input/keyboard.h"
#include "../../framework/math/camera.h"
#include "../../framework/math/common.h"
#include "../../framework/math/mathhelpers.h"
#include "../../framework/math/vector3.h"
#include "../../input/gamepad.h"
#include "../../input/gamepadbuttons.h"
#include "../../input/virtualgamepad.h"
#include "../../processes/gwengameprocess.h"
#include "../../processes/processmanager.h"
#include "../../states/gamestate.h"
#include "../../states/gwengamestate.h"
#include "../../states/statemanager.h"

GamePlayState::GamePlayState(GameApp *gameApp, StateManager *stateManager)
	: GameState(gameApp, stateManager)
{
	STACK_TRACE;
	m_world = NULL;
	m_camera = NULL;
	m_missionFailure = FALSE;
	m_missionSuccess = FALSE;
	m_timeLeftBeforeShowMissionResultScreen = 0.0f;
	m_playerScore = 0;
	m_introIsRunning = TRUE;
	
	ListenFor<IntroFinishedEvent>();
	ListenFor<OptionsUpdatedEvent>();
	ListenFor<QuitGameEvent>();
	ListenFor<DespawnedEvent>();
	ListenFor<TimerRanOutEvent>();
}

GamePlayState::~GamePlayState()
{
	STACK_TRACE;
	StopListeningFor<IntroFinishedEvent>();
	StopListeningFor<OptionsUpdatedEvent>();
	StopListeningFor<QuitGameEvent>();
	StopListeningFor<DespawnedEvent>();
	StopListeningFor<TimerRanOutEvent>();
}

void GamePlayState::OnPush()
{
	STACK_TRACE;
	if (!GetGameApp()->GetKeyboard()->HasPhysicalKeysForGameControls())
		GetGameApp()->GetVirtualGamePad()->SetEnabled(TRUE);

	m_world = new World(GetGameApp());
	m_world->Initialize();
	m_world->SetLevel<TestLevel>();

	m_camera = new OverheadFollowCamera(GetGameApp()->GetGraphicsDevice());
	m_camera->SetTarget(m_world->GetEntityManager()->GetWith<PlayerComponent>());
	GetGameApp()->GetGraphicsDevice()->SetCamera(m_camera);

	GetProcessManager()->Add<GwenGameProcess>()->SetGwenHandler<StatusUI>();
	
	if (GetGameApp()->GetOptions()->showDebugInfo)
		GetProcessManager()->Add<DebugInfoProcess>("debug");
	
	GameModeSettings *settings = GetGameApp()->GetModeSettings();
	if (settings->type == GAME_TYPE_TIMED)
	{
		ASSERT(settings->durationMs > 0);
		TimerSetEvent e;
		e.isCountingDown = TRUE;
		e.milliseconds = settings->durationMs;
		GetEventManager()->Trigger(&e);
	}
	else
	{
		TimerSetEvent e;
		e.isCountingDown = FALSE;
		GetEventManager()->Trigger(&e);
	}
}

void GamePlayState::OnPop()
{
	STACK_TRACE;
	GetGameApp()->GetGraphicsDevice()->SetCamera(NULL);

	if (!GetGameApp()->GetKeyboard()->HasPhysicalKeysForGameControls())
		GetGameApp()->GetVirtualGamePad()->SetEnabled(FALSE);

	SAFE_DELETE(m_world);
	SAFE_DELETE(m_camera);
}

void GamePlayState::OnPause(BOOL dueToOverlay)
{
	STACK_TRACE;
	GameState::OnPause(dueToOverlay);
	if (!GetGameApp()->GetKeyboard()->HasPhysicalKeysForGameControls())
		GetGameApp()->GetVirtualGamePad()->SetEnabled(FALSE);
	
	if (dueToOverlay)
		GetEffectManager()->Add<DimEffect>(FALSE);
	
	TimerPauseEvent timerPause;
	GetEventManager()->Trigger(&timerPause);
}

void GamePlayState::OnResume(BOOL fromOverlay)
{
	STACK_TRACE;
	GameState::OnResume(fromOverlay);
	if (!GetGameApp()->GetKeyboard()->HasPhysicalKeysForGameControls())
		GetGameApp()->GetVirtualGamePad()->SetEnabled(TRUE);
	
	if (fromOverlay)
		GetEffectManager()->Remove<DimEffect>();
	
	TimerResumeEvent timerResume;
	GetEventManager()->Trigger(&timerResume);
}

void GamePlayState::OnAppGainFocus()
{
	STACK_TRACE;
	GameState::OnAppGainFocus();
}

void GamePlayState::OnAppLostFocus()
{
	STACK_TRACE;
	GameState::OnAppLostFocus();
	if (!m_introIsRunning && IsTopState())
		GetStateManager()->Overlay<GwenGameState>()->SetGwenHandler<PauseMenuUI>();
}

void GamePlayState::OnLostContext()
{
	STACK_TRACE;
	GameState::OnLostContext();
	m_world->OnLostContext();
}

void GamePlayState::OnNewContext()
{
	STACK_TRACE;
	GameState::OnNewContext();
	m_world->OnNewContext();
}

void GamePlayState::OnRender(RenderContext *renderContext)
{
	STACK_TRACE;
	renderContext->GetGraphicsDevice()->Clear(0.25f, 0.5f, 1.0f, 1.0f);
	m_world->OnRender(renderContext);
	GameState::OnRender(renderContext);
}

void GamePlayState::OnResize()
{
	STACK_TRACE;
	GameState::OnResize();
	m_world->OnResize();
}

void GamePlayState::OnUpdate(float delta)
{
	STACK_TRACE;
	GameState::OnUpdate(delta);

	if (!m_introIsRunning && !IsTransitioning() && IsTopState())
	{
		if (GetGameApp()->GetGamePad()->IsPressed(BUTTON_BACK))
		{
			GetStateManager()->Overlay<GwenGameState>()->SetGwenHandler<PauseMenuUI>();
			return;
		}
		
		if (m_missionFailure || m_missionSuccess)
		{
			m_timeLeftBeforeShowMissionResultScreen -= delta;
			if (m_timeLeftBeforeShowMissionResultScreen < 0.0f)
			{
				m_timeLeftBeforeShowMissionResultScreen = 0.0f;
				
				if (m_missionSuccess)
					GetStateManager()->Overlay<GwenGameState>()->SetGwenHandler<MissionSuccessState>();
				else if (m_missionFailure)
					GetStateManager()->Overlay<GwenGameState>()->SetGwenHandler<MissionFailureState>();
				
				// so we don't keep counting down, or re-overlaying...
				m_missionSuccess = FALSE;
				m_missionFailure = FALSE;
			}
		}

		BOOL playerCanBeControlled = TRUE;
		
		Entity *player = m_world->GetEntityManager()->GetWith<PlayerComponent>();
		if (player == NULL)
			playerCanBeControlled = FALSE;
		else if (player->Has<DeadComponent>())
			playerCanBeControlled = FALSE;
		
		if (playerCanBeControlled)
		{
			float referenceAngle = GetGameApp()->GetGraphicsDevice()->GetCamera()->GetOrientation().y;
			BOOL isMoving = FALSE;
			float movementAngle = 0.0f;

			if (GetGameApp()->GetGamePad()->IsDown(DPAD_UP) && GetGameApp()->GetGamePad()->IsDown(DPAD_RIGHT))
			{
				isMoving = TRUE;
				movementAngle = RADIANS_45;
			}
			else if (GetGameApp()->GetGamePad()->IsDown(DPAD_UP) && GetGameApp()->GetGamePad()->IsDown(DPAD_LEFT))
			{
				isMoving = TRUE;
				movementAngle = RADIANS_315;
			}
			else if (GetGameApp()->GetGamePad()->IsDown(DPAD_DOWN) && GetGameApp()->GetGamePad()->IsDown(DPAD_RIGHT))
			{
				isMoving = TRUE;
				movementAngle = RADIANS_135;
			}
			else if (GetGameApp()->GetGamePad()->IsDown(DPAD_DOWN) && GetGameApp()->GetGamePad()->IsDown(DPAD_LEFT))
			{
				isMoving = TRUE;
				movementAngle = RADIANS_225;
			}
			else if (GetGameApp()->GetGamePad()->IsDown(DPAD_UP))
			{
				isMoving = TRUE;
				movementAngle = RADIANS_0;
			}
			else if (GetGameApp()->GetGamePad()->IsDown(DPAD_DOWN))
			{
				isMoving = TRUE;
				movementAngle = RADIANS_180;
			}
			else if (GetGameApp()->GetGamePad()->IsDown(DPAD_LEFT))
			{
				isMoving = TRUE;
				movementAngle = RADIANS_270;
			}
			else if (GetGameApp()->GetGamePad()->IsDown(DPAD_RIGHT))
			{
				isMoving = TRUE;
				movementAngle = RADIANS_90;
			}

			if (isMoving)
			{
				// turn player in new direction
				float angle = RolloverClamp(movementAngle + referenceAngle, RADIANS_0, RADIANS_360);
				player->Get<OrientationXZComponent>()->angle = angle;

				// move forward
				MoveForwardEvent *moveEvent = new MoveForwardEvent(player);
				GetGameApp()->GetEventManager()->Queue(moveEvent);
			}

			if (GetGameApp()->GetGamePad()->IsPressed(BUTTON_A))
			{
				JumpEvent *jumpEvent = new JumpEvent(player);
				GetGameApp()->GetEventManager()->Queue(jumpEvent);
			}
			if (GetGameApp()->GetGamePad()->IsPressed(BUTTON_B))
			{
				AttackEvent *attackEvent = new AttackEvent(player);
				GetGameApp()->GetEventManager()->Queue(attackEvent);
			}
			
			if (GetGameApp()->GetGamePad()->IsPressed(BUTTON_L))
				m_camera->RotateBy(-RADIANS_90);
			if (GetGameApp()->GetGamePad()->IsPressed(BUTTON_R))
				m_camera->RotateBy(RADIANS_90);
		}
	}

	GetGameApp()->GetGraphicsDevice()->GetCamera()->OnUpdate(delta);

	if (!m_introIsRunning && IsTopState())
		m_world->OnUpdate(delta);
}

BOOL GamePlayState::OnTransition(float delta, BOOL isTransitioningOut, BOOL started)
{
	STACK_TRACE;
	if (started)
	{
		FadeEffect *fade = GetEffectManager()->Add<FadeEffect>(FALSE);
		if (isTransitioningOut)
			fade->SetFadeOut();
		else
		{
			fade->SetFadeSpeed(1.5f);
			fade->SetFadeIn(0.5f);
		}
	}
	else
	{
		FadeEffect *fade = GetEffectManager()->Get<FadeEffect>();
		if (fade->IsDoneFading())
		{
			if (!isTransitioningOut)
			{
				fade->MarkInactive();
				
				GetStateManager()->Overlay<IntroCountdownState>();
				GetEffectManager()->Add<DimEffect>(FALSE);
			}
			return TRUE;
		}
	}
	
	return FALSE;
}

BOOL GamePlayState::Handle(const Event *event)
{
	STACK_TRACE;
	if (event->Is<QuitGameEvent>())
	{
		GetStateManager()->SwapTopNonOverlayWith<MainMenuState>();
		return TRUE;
	}
	else if (event->Is<IntroFinishedEvent>())
	{
		m_introIsRunning = FALSE;
	}
	else if (event->Is<OptionsUpdatedEvent>())
	{
		if (GetGameApp()->GetOptions()->showDebugInfo && !GetProcessManager()->HasProcess("debug"))
			GetProcessManager()->Add<DebugInfoProcess>("debug");
		else if (!GetGameApp()->GetOptions()->showDebugInfo && GetProcessManager()->HasProcess("debug"))
			GetProcessManager()->Remove("debug");
	}
	else if (event->Is<DespawnedEvent>())
	{
		const DespawnedEvent *e = event->As<DespawnedEvent>();
		if (e->GetEntity()->Has<PlayerComponent>())
		{
			// player entity is about to be destroyed. remove camera entity target
			// (which is the player) so we don't continue using an invalid pointer
			m_camera->SetTarget(NULL);
			
			// before the player entity is gone, grab the current score out of
			// it so that the fail screen can show it
			ScoreComponent *playerScore = e->GetEntity()->Get<ScoreComponent>();
			m_playerScore = playerScore->score;
			
			// when the player dies, it always signals mission failure ...
			m_missionFailure = TRUE;
			if (m_timeLeftBeforeShowMissionResultScreen == 0.0f)
				m_timeLeftBeforeShowMissionResultScreen = 2.0f;
		}
	}
	else if (event->Is<TimerRanOutEvent>())
	{
		Entity *player = GetWorld()->GetEntityManager()->GetWith<PlayerComponent>();
		if (player == NULL)
		{
			// timer ran out after the player died, this is still a mission failure
			// NOTE: we don't have to clear the camera target entity as that
			//       should have been handled already
			m_missionFailure = TRUE;
			if (m_timeLeftBeforeShowMissionResultScreen == 0.0f)
				m_timeLeftBeforeShowMissionResultScreen = 2.0f;
		}
		else
		{
			// timer ran out with the player still alive, this is a mission success
			m_missionSuccess = TRUE;
			m_timeLeftBeforeShowMissionResultScreen = 0.0f;

			// grab the current score so that the success screen can show it
			ScoreComponent *playerScore = player->Get<ScoreComponent>();
			m_playerScore = playerScore->score;
		}
	}
	
	return FALSE;
}

