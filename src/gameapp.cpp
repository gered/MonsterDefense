#include "framework/debug.h"
#include "framework/log.h"

#include "gameapp.h"

#include "contexts/contentcache.h"
#include "contexts/rendercontext.h"
#include "events/eventmanager.h"
#include "framework/sdlgamewindow.h"
#include "framework/marmaladegamewindow.h"
#include "framework/content/contentmanager.h"
#include "framework/content/imageloader.h"
#include "framework/content/keyframemeshloader.h"
#include "framework/content/spritefontloader.h"
#include "framework/content/staticmeshloader.h"
#include "framework/content/textloader.h"
#include "framework/content/textureloader.h"
#include "framework/graphics/graphicsdevice.h"
#include "framework/graphics/textureparameters.h"
#include "input/gamepad.h"
#include "input/pointer.h"
#include "input/virtualgamepad.h"
#include "game/gamemodesettings.h"
#include "game/gameoptions.h"
#include "game/gameplay/gameplaystate.h"
#include "game/mainmenu/mainmenustate.h"
#include "states/statemanager.h"

GameApp::GameApp()
	: BaseGameApp()
{
	STACK_TRACE;
	m_screenScale = 0;
	m_contentCache = NULL;
	m_renderContext = NULL;
	m_stateManager = NULL;
	m_gamePad = NULL;
	m_pointer = NULL;
	m_virtualGamePad = NULL;
	m_eventManager = NULL;
	m_options = NULL;
	m_modeSettings = NULL;
}

GameApp::~GameApp()
{
	STACK_TRACE;
	SAFE_DELETE(m_stateManager);
	SAFE_DELETE(m_eventManager);
	SAFE_DELETE(m_gamePad);
	SAFE_DELETE(m_pointer);
	SAFE_DELETE(m_virtualGamePad);
	SAFE_DELETE(m_contentCache);
	SAFE_DELETE(m_renderContext);
	SAFE_DELETE(m_options);
	SAFE_DELETE(m_modeSettings);
}

void GameApp::OnAppGainFocus()
{
	STACK_TRACE;
	BaseGameApp::OnAppGainFocus();
	m_stateManager->OnAppGainFocus();
}

void GameApp::OnAppLostFocus()
{
	STACK_TRACE;
	BaseGameApp::OnAppLostFocus();
	m_stateManager->OnAppLostFocus();
}

void GameApp::OnAppPause()
{
	STACK_TRACE;
	BaseGameApp::OnAppPause();
	m_stateManager->OnAppPause();
}

void GameApp::OnAppResume()
{
	STACK_TRACE;
	BaseGameApp::OnAppResume();
	m_stateManager->OnAppResume();
}

BOOL GameApp::OnInit()
{
	STACK_TRACE;

#if defined(DESKTOP) && defined(SDL)
	SDLGameWindowParams windowParams;
	windowParams.title = "Monster Defense";
	windowParams.width = 854;
	windowParams.height = 480;
	windowParams.bpp = 0;
	windowParams.windowed = TRUE;
	windowParams.resizable = TRUE;
#elif MOBILE
	GameWindowParams windowParams;
	windowParams.windowed = FALSE;
#else
#error Undefined platform.
#endif

	if (!Initialize(&windowParams))
		return FALSE;
	
	m_options = new GameOptions();
	m_modeSettings = new GameModeSettings();

	SeedRnd(GetOperatingSystem()->GetTicks());

	GetContentManager()->RegisterLoader(new ImageLoader(GetContentManager()));
	GetContentManager()->RegisterLoader(new KeyframeMeshLoader(GetContentManager()));
	GetContentManager()->RegisterLoader(new SpriteFontLoader(GetContentManager()));
	GetContentManager()->RegisterLoader(new StaticMeshLoader(GetContentManager()));
	GetContentManager()->RegisterLoader(new TextLoader(GetContentManager()));
	GetContentManager()->RegisterLoader(new TextureLoader(GetContentManager()));

	KeyframeMeshLoader *keyframeMeshLoader = (KeyframeMeshLoader*)GetContentManager()->GetLoaderForType<KeyframeMesh>();
	keyframeMeshLoader->SetTransform(Matrix4x4::CreateRotationY(RADIANS_90));

	m_gamePad = new GamePad();
	ASSERT(m_gamePad != NULL);
	m_gamePad->SetInputSource(GetKeyboard());

	m_virtualGamePad = new VirtualGamePad(this);
	ASSERT(m_virtualGamePad != NULL);
	m_gamePad->SetInputSource(m_virtualGamePad);

	m_pointer = new Pointer(this);
	ASSERT(m_pointer != NULL);

	m_eventManager = new EventManager();
	ASSERT(m_eventManager != NULL);

	m_stateManager = new StateManager(this);
	ASSERT(m_stateManager != NULL);

	m_renderContext = new RenderContext(GetGraphicsDevice(), GetContentManager());
	ASSERT(m_renderContext != NULL);

	m_contentCache = new ContentCache(GetContentManager());
	ASSERT(m_contentCache != NULL);

	return TRUE;
}

void GameApp::OnLoadGame()
{
	STACK_TRACE;
	GetGraphicsDevice()->SetTextureParameters(TEXPARAM_PIXELATED);

	m_virtualGamePad->OnLoadGame();

	m_renderContext->OnLoadGame();
	m_contentCache->OnLoadGame();

	m_stateManager->Push<MainMenuState>();
	//m_stateManager->Push<GamePlayState>();
}

void GameApp::OnLostContext()
{
	STACK_TRACE;
	BaseGameApp::OnLostContext();
	m_stateManager->OnLostContext();
}

void GameApp::OnNewContext()
{
	STACK_TRACE;
	BaseGameApp::OnNewContext();
	m_stateManager->OnNewContext();
}

void GameApp::OnRender()
{
	STACK_TRACE;
	BaseGameApp::OnRender();
	m_renderContext->OnPreRender();
	m_stateManager->OnRender(m_renderContext);
	if (!m_stateManager->IsTransitioning())
		m_virtualGamePad->Render(m_renderContext->GetSpriteBatch());
	m_renderContext->OnPostRender();
}

void GameApp::OnResize()
{
	STACK_TRACE;
	BaseGameApp::OnResize();
	m_renderContext->OnResize();
	RecalculateScreenScale();
	RecalculateFontSizes();
	m_virtualGamePad->OnResize();
	m_stateManager->OnResize();
}

void GameApp::OnUpdate(float delta)
{
	STACK_TRACE;
	BaseGameApp::OnUpdate(delta);

	m_virtualGamePad->Poll();
	m_gamePad->Poll();
	m_pointer->Poll();

	m_eventManager->ProcessQueue();

	m_stateManager->OnUpdate(delta);

	if (m_stateManager->IsEmpty())
	{
		LOG_INFO(LOGCAT_GAMEAPP, "No states running, quitting.\n");
		Quit();
	}
}

void GameApp::RecalculateScreenScale()
{
	STACK_TRACE;
	uint16_t width = GetGraphicsDevice()->GetViewportWidth();
	uint16_t height = GetGraphicsDevice()->GetViewportHeight();

	m_screenScale = 1;

	if (width < 640 || height < 480)
		return;

	m_screenScale = 2;

	if (width < 960 || height < 720)
		return;

	m_screenScale = 3;

	if (width < 1280 || height < 960)
		return;
}

void GameApp::RecalculateFontSizes()
{
	STACK_TRACE;
	const int BASE_FONT_SIZE = 8;

	m_normalFontSize = BASE_FONT_SIZE * m_screenScale;
	m_bigFontSize = Min(BASE_FONT_SIZE * m_screenScale + (BASE_FONT_SIZE * 2), BASE_FONT_SIZE * 5);
	m_smallFontSize = Max(BASE_FONT_SIZE * (m_screenScale - 1), BASE_FONT_SIZE);
}

