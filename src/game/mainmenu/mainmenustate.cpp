#include "../../framework/debug.h"
#include "../../framework/log.h"
#include "../../framework/common.h"

#include "mainmenustate.h"

#include <stl/string.h>

#include "titletextprocess.h"
#include "mainmenuui.h"
#include "menuuiconstants.h"
#include "../gameoptions.h"
#include "../world.h"
#include "../cameras/overheadfollowcamera.h"
#include "../entities/components/playercomponent.h"
#include "../events/quitappevent.h"
#include "../levels/mainmenulevel.h"
#include "../../gameapp.h"
#include "../../contexts/rendercontext.h"
#include "../../effects/dimeffect.h"
#include "../../effects/effectmanager.h"
#include "../../entities/entity.h"
#include "../../entities/entitymanager.h"
#include "../../events/event.h"
#include "../../events/eventmanager.h"
#include "../../framework/content/contentmanager.h"
#include "../../framework/graphics/color.h"
#include "../../framework/graphics/graphicsdevice.h"
#include "../../framework/graphics/spritebatch.h"
#include "../../framework/math/camera.h"
#include "../../framework/math/common.h"
#include "../../framework/math/mathhelpers.h"
#include "../../framework/math/vector3.h"
#include "../../input/gamepad.h"
#include "../../input/gamepadbuttons.h"
#include "../../processes/processmanager.h"
#include "../../states/gamestate.h"
#include "../../states/gwengamestate.h"
#include "../../states/statemanager.h"

MainMenuState::MainMenuState(GameApp *gameApp, StateManager *stateManager)
	: GameState(gameApp, stateManager)
{
	STACK_TRACE;
	m_world = NULL;
	m_camera = NULL;
	m_rotation = RADIANS_180;
	
	ListenFor<QuitAppEvent>();
}

MainMenuState::~MainMenuState()
{
	STACK_TRACE;
	StopListeningFor<QuitAppEvent>();
}

void MainMenuState::OnPush()
{
	STACK_TRACE;
	m_world = new World(GetGameApp());
	m_world->Initialize();
	m_world->SetLevel<MainMenuLevel>();
	
	m_camera = new OverheadFollowCamera(GetGameApp()->GetGraphicsDevice());
	m_camera->SetTarget(m_world->GetEntityManager()->GetWith<PlayerComponent>());
	m_camera->SetFacingAngle(m_rotation);
	m_camera->SetElevationDistance(5.0f);
	m_camera->SetElevationAngle(DegreesToRadians(45.0f));
	GetGameApp()->GetGraphicsDevice()->SetCamera(m_camera);
	
	// need to update at least once to ensure some entity state stuff is
	// set correctly for rendering (or we could do that manually ourselves...)
	m_world->OnUpdate(0.0f);
	
	GetEffectManager()->Add<DimEffect>();
	
	GetProcessManager()->Add<TitleTextProcess>();
	GetStateManager()->Overlay<GwenGameState>()->SetGwenHandler<MainMenuUI>();
	
	m_menuSideWidth = MENU_SIDE_WIDTH * GetGameApp()->GetScreenScale();
}

void MainMenuState::OnPop()
{
	STACK_TRACE;
	GetGameApp()->GetGraphicsDevice()->SetCamera(NULL);
	
	SAFE_DELETE(m_world);
	SAFE_DELETE(m_camera);
}

void MainMenuState::OnPause(BOOL dueToOverlay)
{
	STACK_TRACE;
	GameState::OnPause(dueToOverlay);
}

void MainMenuState::OnResume(BOOL fromOverlay)
{
	STACK_TRACE;
	GameState::OnResume(fromOverlay);
}

void MainMenuState::OnAppGainFocus()
{
	STACK_TRACE;
	GameState::OnAppGainFocus();
}

void MainMenuState::OnAppLostFocus()
{
	STACK_TRACE;
	GameState::OnAppLostFocus();
}

void MainMenuState::OnLostContext()
{
	STACK_TRACE;
	GameState::OnLostContext();
	m_world->OnLostContext();
}

void MainMenuState::OnNewContext()
{
	STACK_TRACE;
	GameState::OnNewContext();
	m_world->OnNewContext();
}

void MainMenuState::OnRender(RenderContext *renderContext)
{
	STACK_TRACE;
	renderContext->GetGraphicsDevice()->Clear(COLOR_BLACK);
	m_world->OnRender(renderContext);
	GameState::OnRender(renderContext);
	
	if (!IsTopState())
	{
		uint16_t top = 0;
		uint16_t left = renderContext->GetGraphicsDevice()->GetViewportRight() - m_menuSideWidth;
		uint16_t bottom = renderContext->GetGraphicsDevice()->GetViewportBottom();
		uint16_t right = renderContext->GetGraphicsDevice()->GetViewportRight();
		
		renderContext->GetSpriteBatch()->RenderFilledBox(left, top, right, bottom, Color(0.0f, 0.0f, 0.0f, 0.5f));
	}
}

void MainMenuState::OnResize()
{
	STACK_TRACE;
	GameState::OnResize();
	m_world->OnResize();
	
	m_menuSideWidth = MENU_SIDE_WIDTH * GetGameApp()->GetScreenScale();
}

void MainMenuState::OnUpdate(float delta)
{
	STACK_TRACE;
	GameState::OnUpdate(delta);
	
	//m_rotation += delta;
	m_rotation = RolloverClamp(m_rotation + (delta / 5.0f), RADIANS_0, RADIANS_360);
	m_camera->SetFacingAngle(m_rotation);
	
	GetGameApp()->GetGraphicsDevice()->GetCamera()->OnUpdate(delta);
}

BOOL MainMenuState::OnTransition(float delta, BOOL isTransitioningOut, BOOL started)
{
	STACK_TRACE;
	return TRUE;
}

BOOL MainMenuState::Handle(const Event *event)
{
	STACK_TRACE;
	if (event->Is<QuitAppEvent>())
		SetFinished();
	
	return FALSE;
}

