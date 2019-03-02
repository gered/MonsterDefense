#include "../../framework/debug.h"

#include "statusui.h"
#include "gameplaystate.h"
#include "pausemenuui.h"
#include "weaponselectionui.h"
#include "../world.h"
#include "../gamemodesettings.h"
#include "../entities/buffs/appliedbuff.h"
#include "../entities/buffs/buff.h"
#include "../entities/buffs/buffcollection.h"
#include "../entities/components/buffscomponent.h"
#include "../entities/components/lifecomponent.h"
#include "../entities/components/playercomponent.h"
#include "../entities/components/scorecomponent.h"
#include "../entities/components/weaponcomponent.h"
#include "../entities/components/weaponlistcomponent.h"
#include "../entities/globalcomponents/monsterpowercomponent.h"
#include "../entities/weapons/weapon.h"
#include "../events/timerpauseevent.h"
#include "../events/timerranoutevent.h"
#include "../events/timerresumeevent.h"
#include "../events/timersetevent.h"
#include "../../gameapp.h"
#include "../../contexts/contentcache.h"
#include "../../entities/entity.h"
#include "../../entities/entitymanager.h"
#include "../../framework/graphics/color.h"
#include "../../framework/graphics/graphicsdevice.h"
#include "../../framework/graphics/spritebatch.h"
#include "../../framework/graphics/spritefont.h"
#include "../../framework/graphics/texture.h"
#include "../../framework/graphics/textureatlas.h"
#include "../../processes/gwengameprocess.h"
#include "../../states/gwengamestate.h"
#include "../../states/statemanager.h"
#include <controls/gwen_button.h>
#include <controls/gwen_canvas.h>
#include <controls/gwen_progressbar.h>
#include <string.h>

StatusUI::StatusUI(GwenGameProcess *gameProcess)
	: GwenGameProcessUIController(gameProcess)
{
	STACK_TRACE;
	m_timerIsPaused = TRUE;
	m_timerIsCountingDown = FALSE;
	m_timerTime = 0.0f;
	
	ListenFor<TimerPauseEvent>();
	ListenFor<TimerResumeEvent>();
	ListenFor<TimerSetEvent>();
}

StatusUI::~StatusUI()
{
	STACK_TRACE;
	StopListeningFor<TimerPauseEvent>();
	StopListeningFor<TimerResumeEvent>();
	StopListeningFor<TimerSetEvent>();
}

void StatusUI::OnAdd()
{
	STACK_TRACE;
	ContentCache *contentCache = GetGameProcess()->GetGameApp()->GetContentCache();
	InitializeGwen(contentCache->GetUISkinFilename(), contentCache->GetUIFontFilename(), contentCache->GetUIFontSize());
	
	AddControls();
	PositionControls();
	SetCurrentWeaponImage();
}

void StatusUI::AddControls()
{
	STACK_TRACE;
	Gwen::Controls::Canvas *canvas = GetCanvas();
	
	Gwen::Controls::ProgressBar *pbrHealth = new Gwen::Controls::ProgressBar(canvas, "pbrHealth");
	pbrHealth->SetAutoLabel(true);
	pbrHealth->SetSize(100, 20);

	Gwen::Controls::Button *btnWeapons = new Gwen::Controls::Button(canvas, "btnWeapons");
	btnWeapons->SetSize(36, 36);
	btnWeapons->onPress.Add(this, &StatusUI::OnWeaponsClick);

	Gwen::Controls::Button *btnMenu = new Gwen::Controls::Button(canvas, "btnMenu");
	btnMenu->SetText("Menu");
	btnMenu->SetSize(50, 36);
	btnMenu->onPress.Add(this, &StatusUI::OnMenuClick);
}

void StatusUI::PositionControls()
{
	STACK_TRACE;
	Gwen::Controls::Canvas *canvas = GetCanvas();
	
	Gwen::Controls::ProgressBar *pbrHealth = canvas->FindChild<Gwen::Controls::ProgressBar>("pbrHealth");
	Gwen::Controls::Button *btnWeapons = canvas->FindChild<Gwen::Controls::Button>("btnWeapons");
	Gwen::Controls::Button *btnMenu = canvas->FindChild<Gwen::Controls::Button>("btnMenu");
	
	btnMenu->SetPos(5, 2);
	btnWeapons->SetPos(canvas->Width() - 5 - btnWeapons->Width(), 2);
	pbrHealth->SetPos(btnWeapons->X() - 5 - pbrHealth->Width(), 2);
}

void StatusUI::OnResume(BOOL fromOverlay)
{
	STACK_TRACE;
	GwenGameProcessUIController::OnResume(fromOverlay);
	
	if (fromOverlay)
		SetCurrentWeaponImage();
}

void StatusUI::OnUpdate(float delta)
{
	STACK_TRACE;
	GwenGameProcessUIController::OnUpdate(delta);
	
	char tempScoreString[30];
	
	GamePlayState *gameState = GetGameState()->As<GamePlayState>();
	ASSERT(gameState != NULL);

	Entity *player = gameState->GetWorld()->GetEntityManager()->GetWith<PlayerComponent>();
	
	////////////////////////////////////////////////////////////////////////////
	// update player status display
	if (player != NULL)
	{
		LifeComponent *playerLife = player->Get<LifeComponent>();
		ScoreComponent *playerScore = player->Get<ScoreComponent>();
	
		Gwen::Controls::ProgressBar *pbrHealth = GetCanvas()->FindChild<Gwen::Controls::ProgressBar>("pbrHealth", true);
		Gwen::Controls::Label *lblScore = GetCanvas()->FindChild<Gwen::Controls::Label>("lblScore", true);
		
		pbrHealth->SetValue(playerLife->GetHealthPercent());
		snprintf(tempScoreString, 30, "%d", playerScore->score);
		//lblScore->SetText(tempScoreString);
	}
	////////////////////////////////////////////////////////////////////////////
	
	////////////////////////////////////////////////////////////////////////////
	// update timer
	if (!m_timerIsPaused)
	{	
		if (m_timerIsCountingDown)
		{
			m_timerTime -= delta;
			if (m_timerTime <= 0.0f)
			{
				m_timerTime = 0.0f;
				m_timerIsPaused = TRUE;   // so we don't continue counting down and
				// keep triggering the below event
				
				TimerRanOutEvent timerRanOut;
				gameState->GetEventManager()->Trigger(&timerRanOut);
			}
		}
		else
			m_timerTime += delta;
	}
	////////////////////////////////////////////////////////////////////////////
}

void StatusUI::OnRender(RenderContext *renderContext)
{
	STACK_TRACE;
	ContentCache *contentCache = GetGameProcess()->GetGameApp()->GetContentCache();
	SpriteFont *font = contentCache->GetFont();
	SpriteFont *scoreFont = contentCache->GetUIFont();
	SpriteFont *smallFont = contentCache->GetSmallFont();
	uint16_t viewportWidth = renderContext->GetGraphicsDevice()->GetViewportWidth();
	World *world = GetGameState()->As<GamePlayState>()->GetWorld();
	Entity *player = world->GetEntityManager()->GetWith<PlayerComponent>();
	const float SCREEN_SCALE = renderContext->GetScreenScale();

	Gwen::Controls::Canvas *canvas = GetCanvas();
	Gwen::Controls::Button *btnMenu = canvas->FindChild<Gwen::Controls::Button>("btnMenu");
	Gwen::Controls::Button *btnWeapons = canvas->FindChild<Gwen::Controls::Button>("btnWeapons");
	Gwen::Controls::ProgressBar *pbrHealth = canvas->FindChild<Gwen::Controls::ProgressBar>("pbrHealth");
	
	////////////////////////////////////////////////////////////////////////////
	// background
	uint16_t bgLeft = 0;
	uint16_t bgTop = 0;
	uint16_t bgRight = renderContext->GetGraphicsDevice()->GetViewportRight();
	uint16_t bgBottom = 40 * renderContext->GetScreenScale();
	renderContext->GetSpriteBatch()->RenderFilledBox(bgLeft, bgTop, bgRight, bgBottom, Color(0.0f, 0.0f, 0.0f, 0.3f));
	////////////////////////////////////////////////////////////////////////////

	// render GWEN UI stuff
	GwenGameProcessUIController::OnRender(renderContext);
	
	// now render everything else

	////////////////////////////////////////////////////////////////////////////
	// timer display
	const float TIMER_FONT_SCALE = SCREEN_SCALE * 1.3f;
	float timerShadowOffset = TIMER_FONT_SCALE;

	uint32_t ticks = (uint32_t)(m_timerTime * 1000.0f);
	
	int minutes = ticks / 1000 / 60;
	ticks -= (minutes * 60 * 1000);
	int seconds = ticks / 1000;
	ticks -= (seconds * 1000);
	int milliseconds = ticks;
	
	uint16_t timerWidth;
	uint16_t timerHeight;
	font->MeasureString(&timerWidth, &timerHeight, TIMER_FONT_SCALE, "%02d:%02d:%03d", minutes, seconds, milliseconds);
	uint32_t timerLeft = (btnMenu->Right() + 5) * SCREEN_SCALE;
	uint32_t timerTop = 0;
	
	renderContext->GetSpriteBatch()->Printf(font, timerLeft + timerShadowOffset, timerTop + timerShadowOffset, COLOR_BLACK, TIMER_FONT_SCALE, "%02d:%02d:%03d", minutes, seconds, milliseconds);
	renderContext->GetSpriteBatch()->Printf(font, timerLeft, timerTop, COLOR_WHITE, TIMER_FONT_SCALE, "%02d:%02d:%03d", minutes, seconds, milliseconds);
	////////////////////////////////////////////////////////////////////////////
	
	////////////////////////////////////////////////////////////////////////////
	// monster power display
	if (GetGameState()->GetGameApp()->GetModeSettings()->type == GAME_TYPE_ENDLESS)
	{
		const float MONSTERPOWER_FONT_SCALE = SCREEN_SCALE;
		
		MonsterPowerComponent *monsterPower = world->GetEntityManager()->GetGlobalComponent<MonsterPowerComponent>();
		ASSERT(monsterPower != NULL);
		
		int attackPowerBonus = (monsterPower->attackPercentAboveBase * 100.0f);
		
		uint16_t monsterPowerWidth;
		smallFont->MeasureString(&monsterPowerWidth, NULL, MONSTERPOWER_FONT_SCALE, "Monster Power Bonus: %d%%", attackPowerBonus);
		uint32_t monsterPowerLeft = timerLeft + (timerWidth - monsterPowerWidth) / 2;
		uint32_t monsterPowerTop = timerHeight;
		
		renderContext->GetSpriteBatch()->Printf(smallFont, monsterPowerLeft, monsterPowerTop, COLOR_WHITE, MONSTERPOWER_FONT_SCALE, "Monster Power Bonus: %d%%", attackPowerBonus);
	}
	////////////////////////////////////////////////////////////////////////////
	
	////////////////////////////////////////////////////////////////////////////
	// player score
	const float SCORE_FONT_SCALE = SCREEN_SCALE;
	
	char scoreString[30];
	if (player != NULL)
	{
		ScoreComponent *playerScore = player->Get<ScoreComponent>();
		snprintf(scoreString, 30, "%d", playerScore->score);
	}
	else
		snprintf(scoreString, 30, "%d", 0);
	
	uint16_t scoreWidth;
	scoreFont->MeasureString(&scoreWidth, NULL, SCORE_FONT_SCALE, scoreString);
	uint16_t scoreTop = (pbrHealth->Bottom() + 2) * SCREEN_SCALE;
	uint16_t scoreLeft = (pbrHealth->Right()) * SCREEN_SCALE - scoreWidth;
	renderContext->GetSpriteBatch()->Render(scoreFont, scoreLeft, scoreTop, COLOR_WHITE, SCORE_FONT_SCALE, scoreString);
	
	////////////////////////////////////////////////////////////////////////////
	
	////////////////////////////////////////////////////////////////////////////
	// player active buff display
	if (player != NULL)
	{
		const float BUFFLIST_FONT_SCALE = SCREEN_SCALE;
		const uint16_t BUFF_BOX_WIDTH = 100 * SCREEN_SCALE;
		const uint16_t BUFF_BOX_HEIGHT = 20 * SCREEN_SCALE;
		const uint16_t BUFF_BOX_SPACING = 3 * SCREEN_SCALE;
		const Color BUFF_BOX_BG_COLOR = Color(0.0f, 0.0f, 0.0f, 0.75f);
		const Color BUFF_BOX_TIME_BG_COLOR = Color(0.0f, 0.0f, 0.5f, 0.5f);
		
		BuffsComponent *buffs = player->Get<BuffsComponent>();
		ASSERT(buffs != NULL);
		
		if (buffs->buffs.size() > 0)
		{
			Gwen::Controls::Button *btnWeapons = GetCanvas()->FindChild<Gwen::Controls::Button>("btnWeapons", true);
			
			uint16_t buffListTop = (uint16_t)((btnWeapons->Bottom() + 5) * GetCanvas()->Scale());
			uint16_t buffListLeft = viewportWidth - (3 * SCREEN_SCALE) - BUFF_BOX_WIDTH;
			
			uint16_t x = buffListLeft;
			uint16_t y = buffListTop;
			
			for (AppliedBuffsList::iterator i = buffs->buffs.begin(); i != buffs->buffs.end(); ++i)
			{
				AppliedBuff *buff = &(*i);
				
				float percentTimeRemaining = buff->remainingTime / buff->buff->GetDuration();
				uint16_t buffBoxTimeRemainingWidth = (uint16_t)((float)BUFF_BOX_WIDTH * percentTimeRemaining);
				
				const TextureAtlasTile &buffImageTile = contentCache->GetItems()->GetTile(buff->buff->GetImageIndex());
				uint16_t buffImageWidth = buffImageTile.dimensions.GetWidth() * SCREEN_SCALE;
				uint16_t buffImageHeight = buffImageTile.dimensions.GetHeight() * SCREEN_SCALE;
				uint16_t buffImageX = x + 3 * SCREEN_SCALE;
				uint16_t buffImageY = y + ((BUFF_BOX_HEIGHT - buffImageHeight) / 2);
				
				uint16_t buffNameWidth;
				uint16_t buffNameHeight;
				smallFont->MeasureString(&buffNameWidth, &buffNameHeight, BUFFLIST_FONT_SCALE, "%s", buff->buff->GetTypeOf());
				uint16_t buffNameX = buffImageX + buffImageWidth + 3 * SCREEN_SCALE;
				uint16_t buffNameY = y + ((BUFF_BOX_HEIGHT - buffNameHeight) / 2);
				
				renderContext->GetSpriteBatch()->RenderFilledBox(x, y, x + buffBoxTimeRemainingWidth, y + BUFF_BOX_HEIGHT, BUFF_BOX_TIME_BG_COLOR);
				renderContext->GetSpriteBatch()->RenderFilledBox(x + buffBoxTimeRemainingWidth, y, x + BUFF_BOX_WIDTH, y + BUFF_BOX_HEIGHT, BUFF_BOX_BG_COLOR);
				renderContext->GetSpriteBatch()->Render(contentCache->GetItems(), buff->buff->GetImageIndex(), buffImageX, buffImageY, buffImageWidth, buffImageHeight);
				renderContext->GetSpriteBatch()->Printf(smallFont, buffNameX + SCREEN_SCALE, buffNameY + SCREEN_SCALE, COLOR_BLACK, BUFFLIST_FONT_SCALE, "%s", buff->buff->GetTypeOf());
				renderContext->GetSpriteBatch()->Printf(smallFont, buffNameX, buffNameY, COLOR_WHITE, BUFFLIST_FONT_SCALE, "%s", buff->buff->GetTypeOf());
				
				y += BUFF_BOX_SPACING + BUFF_BOX_HEIGHT;
			}
		}
	}
	////////////////////////////////////////////////////////////////////////////
}

void StatusUI::OnResize()
{
	STACK_TRACE;
	GwenGameProcessUIController::OnResize();
	PositionControls();
}

BOOL StatusUI::Handle(const Event *event)
{
	STACK_TRACE;
	if (event->Is<TimerPauseEvent>())
	{
		m_timerIsPaused = TRUE;
	}
	else if (event->Is<TimerResumeEvent>())
	{
		// don't resume if the timer will just immediately run out again
		if (!m_timerIsCountingDown || m_timerTime > 0.0f)
			m_timerIsPaused = FALSE;
	}
	else if (event->Is<TimerSetEvent>())
	{
		const TimerSetEvent *e = event->As<TimerSetEvent>();
		m_timerIsCountingDown = e->isCountingDown;
		m_timerTime = (float)e->milliseconds / 1000.0f;
	}
	return FALSE;
}

void StatusUI::OnWeaponsClick(Gwen::Controls::Base *sender)
{
	STACK_TRACE;
	World *world = GetGameState()->As<GamePlayState>()->GetWorld();
	Entity *player = world->GetEntityManager()->GetWith<PlayerComponent>();
	if (player == NULL)
		return;
	
	// we're assuming this couldn't be clicked if the pause menu overlay state
	// is already up...
	GetGameProcess()->GetGameApp()->GetStateManager()->Overlay<GwenGameState>()->SetGwenHandler<WeaponSelectionUI>();
}

void StatusUI::OnMenuClick(Gwen::Controls::Base *sender)
{
	STACK_TRACE;
	// we're assuming this couldn't be clicked if the pause menu overlay state
	// is already up...
	GetGameProcess()->GetGameApp()->GetStateManager()->Overlay<GwenGameState>()->SetGwenHandler<PauseMenuUI>();	
}

void StatusUI::SetCurrentWeaponImage()
{
	ContentCache *contentCache = GetGameProcess()->GetGameApp()->GetContentCache();
	World *world = GetGameState()->As<GamePlayState>()->GetWorld();
	Entity *player = world->GetEntityManager()->GetWith<PlayerComponent>();
	if (player == NULL)
		return;

	//Gwen::Controls::ImagePanel *imgWeapon = GetCanvas()->FindChild<Gwen::Controls::ImagePanel>("imgWeapon", true);
	Gwen::Controls::Button *btnWeapons = GetCanvas()->FindChild<Gwen::Controls::Button>("btnWeapons");
	
	Gwen::Texture weaponImage;
	weaponImage.data = (void*)contentCache->GetItems()->GetTexture();
	weaponImage.width = contentCache->GetItems()->GetTexture()->GetWidth();
	weaponImage.height = contentCache->GetItems()->GetTexture()->GetHeight();
	
	uint32_t weaponTileIndex = 0;
	if (player != NULL)
	{
		WeaponComponent *playerWeapon = player->Get<WeaponComponent>();
		weaponTileIndex = playerWeapon->weapon->GetImageIndex();
	}
	const TextureAtlasTile &weaponImageTile = contentCache->GetItems()->GetTile(weaponTileIndex);
	btnWeapons->SetImage(weaponImage, weaponImageTile.texCoords.left, weaponImageTile.texCoords.top, weaponImageTile.texCoords.right, weaponImageTile.texCoords.bottom, true);
};
