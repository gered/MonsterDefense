#include "../framework/debug.h"
#include "../framework/log.h"

#include "virtualgamepad.h"

#include "../gameapp.h"
#include "../framework/content/contentmanager.h"
#include "../framework/graphics/customtextureatlas.h"
#include "../framework/graphics/graphicsdevice.h"
#include "../framework/graphics/spritebatch.h"
#include "../framework/graphics/texture.h"
#include "../framework/graphics/textureatlas.h"
#include "../framework/input/touchscreen.h"
#include "../framework/math/circle.h"

#include <string.h>

const uint16_t BASE_DPAD_SIZE = 8;
const uint16_t BASE_BUTTON_SIZE = 16;
const uint16_t BASE_MARGIN = 20;
const uint16_t BASE_SCALE_FACTOR = 4;

VirtualGamePad::VirtualGamePad(GameApp *gameApp)
{
	STACK_TRACE;
	m_gameApp = gameApp;

	m_buttons = new BOOL[BUTTON_LAST];
	ASSERT(m_buttons != NULL);
	m_lockedButtons = new BOOL[BUTTON_LAST];
	ASSERT(m_lockedButtons != NULL);
	m_buttonsNotTouching = new BOOL[BUTTON_LAST];
	ASSERT(m_buttonsNotTouching != NULL);
	m_activeButtons = new BOOL[BUTTON_LAST];
	ASSERT(m_activeButtons != NULL);

	memset(m_buttons, FALSE, sizeof(BOOL) * BUTTON_LAST);
	memset(m_lockedButtons, FALSE, sizeof(BOOL) * BUTTON_LAST);
	memset(m_buttonsNotTouching, FALSE, sizeof(BOOL) * BUTTON_LAST);
	memset(m_activeButtons, FALSE, sizeof(BOOL) * BUTTON_LAST);

	m_buttonUpTileIndexes = new uint32_t[BUTTON_LAST];
	ASSERT(m_buttonUpTileIndexes != NULL);
	m_buttonDownTileIndexes = new uint32_t[BUTTON_LAST];
	ASSERT(m_buttonDownTileIndexes != NULL);
	m_buttonPositions = new Rect[BUTTON_LAST];
	ASSERT(m_buttonPositions);
	m_buttonHitAreas = new Circle[BUTTON_LAST];
	ASSERT(m_buttonHitAreas);

	m_textures = NULL;

	m_enabled = FALSE;
}

VirtualGamePad::~VirtualGamePad()
{
	STACK_TRACE;
	SAFE_DELETE_ARRAY(m_buttons);
	SAFE_DELETE_ARRAY(m_lockedButtons);
	SAFE_DELETE_ARRAY(m_buttonsNotTouching);
	SAFE_DELETE_ARRAY(m_activeButtons);
	SAFE_DELETE(m_textures);
	SAFE_DELETE_ARRAY(m_buttonUpTileIndexes);
	SAFE_DELETE_ARRAY(m_buttonDownTileIndexes);
	SAFE_DELETE_ARRAY(m_buttonPositions);
	SAFE_DELETE_ARRAY(m_buttonHitAreas);
	m_gameApp->GetContentManager()->Free<Texture>("assets://gamepad.png");
}

void VirtualGamePad::OnLoadGame()
{
	STACK_TRACE;
	Texture *gamePadIconsTexture = m_gameApp->GetContentManager()->Get<Texture>("assets://gamepad.png");
	CustomTextureAtlas *gamePadIcons = new CustomTextureAtlas(gamePadIconsTexture);
	ASSERT(gamePadIcons != NULL);

	// get individual button icons from the atlas
	m_buttonUpTileIndexes[BUTTON_A] = gamePadIcons->Add(0, 0, 16, 16);
	m_buttonDownTileIndexes[BUTTON_A] = gamePadIcons->Add(16, 0, 32, 16);
	m_buttonUpTileIndexes[BUTTON_B] = gamePadIcons->Add(32, 0, 48, 16);
	m_buttonDownTileIndexes[BUTTON_B] = gamePadIcons->Add(48, 0, 64, 16);
	m_buttonUpTileIndexes[DPAD_DOWN] = gamePadIcons->Add(0, 16, 8, 24);
	m_buttonDownTileIndexes[DPAD_DOWN] = gamePadIcons->Add(8, 16, 16, 24);
	m_buttonUpTileIndexes[DPAD_LEFT] = gamePadIcons->Add(16, 16, 24, 24);
	m_buttonDownTileIndexes[DPAD_LEFT] = gamePadIcons->Add(24, 16, 32, 24);
	m_buttonUpTileIndexes[DPAD_UP] = gamePadIcons->Add(32, 16, 40, 24);
	m_buttonDownTileIndexes[DPAD_UP] = gamePadIcons->Add(40, 16, 48, 24);
	m_buttonUpTileIndexes[DPAD_RIGHT] = gamePadIcons->Add(48, 16, 56, 24);
	m_buttonDownTileIndexes[DPAD_RIGHT] = gamePadIcons->Add(56, 16, 64, 24);
	m_dpadCenterTileIndex = gamePadIcons->Add(64, 16, 72, 24);
	m_cameraTileIndex = gamePadIcons->Add(0, 24, 16, 40);
	m_buttonUpTileIndexes[BUTTON_L] = gamePadIcons->Add(16, 24, 32, 40);
	m_buttonDownTileIndexes[BUTTON_L] = gamePadIcons->Add(32, 24, 48, 40);
	m_buttonUpTileIndexes[BUTTON_R] = gamePadIcons->Add(48, 24, 64, 40);
	m_buttonDownTileIndexes[BUTTON_R] = gamePadIcons->Add(64, 24, 80, 40);

	// unused
	m_buttonUpTileIndexes[BUTTON_X] = 0;
	m_buttonDownTileIndexes[BUTTON_X] = 0;
	m_buttonUpTileIndexes[BUTTON_Y] = 0;
	m_buttonDownTileIndexes[BUTTON_Y] = 0;
	m_buttonUpTileIndexes[BUTTON_START] = 0;
	m_buttonDownTileIndexes[BUTTON_START] = 0;
	m_buttonUpTileIndexes[BUTTON_SELECT] = 0;
	m_buttonDownTileIndexes[BUTTON_SELECT] = 0;
	m_buttonUpTileIndexes[BUTTON_BACK] = 0;
	m_buttonDownTileIndexes[BUTTON_BACK] = 0;

	m_textures = gamePadIcons;
}

void VirtualGamePad::OnResize()
{
	STACK_TRACE;
	SizeAndPositionButtons();
}

void VirtualGamePad::Poll()
{
	STACK_TRACE;
	if (!m_enabled)
		return;

	Touchscreen *touchscreen = m_gameApp->GetTouchscreen();
	if (touchscreen == NULL)
		return;

	memset(m_buttonsNotTouching, TRUE, sizeof(BOOL) * BUTTON_LAST);

	// for all touch pointers currently down, find all the buttons that are and
	// are not being touched
	for (uint32_t p = 0; p < touchscreen->GetPointerCount(); ++p)
	{
		const TouchPointer *pointer = touchscreen->GetPointer(p);
		if (!pointer->IsTouching())
			continue;

		for (uint32_t b = 0; b < BUTTON_LAST; ++b)
		{
			if (m_activeButtons[b])
			{
				// if touching within this button's touch area, set this button
				// as touched, and that it's not not being touched (yay, double negative)
				if (b == BUTTON_L && pointer->IsTouchingWithinArea(m_bigHitAreaL))
				{
					m_buttons[b] = !(m_lockedButtons[b]);      // pressed only if not locked
					m_buttonsNotTouching[b] = FALSE;
				}
				else if (b == BUTTON_R && pointer->IsTouchingWithinArea(m_bigHitAreaR))
				{
					m_buttons[b] = !(m_lockedButtons[b]);      // pressed only if not locked
					m_buttonsNotTouching[b] = FALSE;
				}
				else if (pointer->IsTouchingWithinArea(m_buttonHitAreas[b]))
				{
					m_buttons[b] = !(m_lockedButtons[b]);      // pressed only if not locked
					m_buttonsNotTouching[b] = FALSE;
				}
			}
		}
	}

	// for all buttons that were not being touched, clear their pressed and 
	// locked status
	for (uint32_t b = 0; b < BUTTON_LAST; ++b)
	{
		if (m_buttonsNotTouching[b])
		{
			m_buttons[b] = FALSE;
			m_lockedButtons[b] = FALSE;
		}
	}
}

void VirtualGamePad::Render(SpriteBatch *spriteBatch)
{
	STACK_TRACE;
	if (!m_enabled)
		return;

	spriteBatch->Render(m_textures, (m_buttons[DPAD_UP] ? m_buttonDownTileIndexes[DPAD_UP] : m_buttonUpTileIndexes[DPAD_UP]), m_buttonPositions[DPAD_UP].left, m_buttonPositions[DPAD_UP].top, m_buttonPositions[DPAD_UP].GetWidth(), m_buttonPositions[DPAD_UP].GetHeight());
	spriteBatch->Render(m_textures, (m_buttons[DPAD_DOWN] ? m_buttonDownTileIndexes[DPAD_DOWN] : m_buttonUpTileIndexes[DPAD_DOWN]), m_buttonPositions[DPAD_DOWN].left, m_buttonPositions[DPAD_DOWN].top, m_buttonPositions[DPAD_DOWN].GetWidth(), m_buttonPositions[DPAD_DOWN].GetHeight());
	spriteBatch->Render(m_textures, (m_buttons[DPAD_LEFT] ? m_buttonDownTileIndexes[DPAD_LEFT] : m_buttonUpTileIndexes[DPAD_LEFT]), m_buttonPositions[DPAD_LEFT].left, m_buttonPositions[DPAD_LEFT].top, m_buttonPositions[DPAD_LEFT].GetWidth(), m_buttonPositions[DPAD_LEFT].GetHeight());
	spriteBatch->Render(m_textures, (m_buttons[DPAD_RIGHT] ? m_buttonDownTileIndexes[DPAD_RIGHT] : m_buttonUpTileIndexes[DPAD_RIGHT]), m_buttonPositions[DPAD_RIGHT].left, m_buttonPositions[DPAD_RIGHT].top, m_buttonPositions[DPAD_RIGHT].GetWidth(), m_buttonPositions[DPAD_RIGHT].GetHeight());
	spriteBatch->Render(m_textures, (m_buttons[BUTTON_A] ? m_buttonDownTileIndexes[BUTTON_A] : m_buttonUpTileIndexes[BUTTON_A]), m_buttonPositions[BUTTON_A].left, m_buttonPositions[BUTTON_A].top, m_buttonPositions[BUTTON_A].GetWidth(), m_buttonPositions[BUTTON_A].GetHeight());
	spriteBatch->Render(m_textures, (m_buttons[BUTTON_B] ? m_buttonDownTileIndexes[BUTTON_B] : m_buttonUpTileIndexes[BUTTON_B]), m_buttonPositions[BUTTON_B].left, m_buttonPositions[BUTTON_B].top, m_buttonPositions[BUTTON_B].GetWidth(), m_buttonPositions[BUTTON_B].GetHeight());
	spriteBatch->Render(m_textures, m_dpadCenterTileIndex, m_dpadCenterPosition.left, m_dpadCenterPosition.top, m_dpadCenterPosition.GetWidth(), m_dpadCenterPosition.GetHeight());

	// camera buttons
	//spriteBatch->Render(m_textures, (m_buttons[BUTTON_L] ? m_buttonDownTileIndexes[BUTTON_L] : m_buttonUpTileIndexes[BUTTON_L]), m_buttonPositions[BUTTON_L].left, m_buttonPositions[BUTTON_L].top, m_buttonPositions[BUTTON_L].GetWidth(), m_buttonPositions[BUTTON_L].GetHeight());
	//spriteBatch->Render(m_textures, (m_buttons[BUTTON_R] ? m_buttonDownTileIndexes[BUTTON_R] : m_buttonUpTileIndexes[BUTTON_R]), m_buttonPositions[BUTTON_R].left, m_buttonPositions[BUTTON_R].top, m_buttonPositions[BUTTON_R].GetWidth(), m_buttonPositions[BUTTON_R].GetHeight());
	//spriteBatch->Render(m_textures, m_cameraTileIndex, m_cameraIconPosition.left, m_cameraIconPosition.top, m_cameraIconPosition.GetWidth(), m_cameraIconPosition.GetHeight());
}

BOOL VirtualGamePad::IsPressed(GAMEPAD_BUTTONS button)
{
	STACK_TRACE;
	if (m_buttons[button] && !m_lockedButtons[button])
	{
		m_lockedButtons[button] = TRUE;
		return TRUE;
	}
	else
		return FALSE;
}

void VirtualGamePad::SetEnabled(BOOL enabled)
{
	STACK_TRACE;
	LOG_INFO("VIRTUALGAMEPAD", "%s virtual game pad (previously %s).\n", (enabled ? "Enabling" : "Disabling"), (m_enabled ? "enabled" : "disabled"));
	m_enabled = enabled;
}

void VirtualGamePad::SizeAndPositionButtons()
{
	STACK_TRACE;
	// set up button positions
	uint16_t margin = BASE_MARGIN;
	uint16_t screenWidth = m_gameApp->GetGraphicsDevice()->GetViewportWidth();
	uint16_t screenHeight = m_gameApp->GetGraphicsDevice()->GetViewportHeight();

	const uint16_t dpadSize = BASE_DPAD_SIZE * m_gameApp->GetScreenScale() * BASE_SCALE_FACTOR;
	const uint16_t halfDpadSize = dpadSize / 2;
	const uint16_t dpadRadius = dpadSize;// / 1.5f;
	const uint16_t buttonSize = BASE_BUTTON_SIZE * m_gameApp->GetScreenScale() * BASE_SCALE_FACTOR;
	const uint16_t halfButtonSize = buttonSize / 2;
	const uint16_t buttonRadius = halfButtonSize;
	const uint16_t quarterButtonSize = buttonSize / 4;

	m_buttonPositions[DPAD_UP].left = margin + dpadSize;
	m_buttonPositions[DPAD_UP].top = screenHeight - margin - (dpadSize * 3);
	m_buttonPositions[DPAD_UP].right = m_buttonPositions[DPAD_UP].left + dpadSize;
	m_buttonPositions[DPAD_UP].bottom = m_buttonPositions[DPAD_UP].top + dpadSize;
	m_buttonHitAreas[DPAD_UP].Set(m_buttonPositions[DPAD_UP].left + halfDpadSize, m_buttonPositions[DPAD_UP].top + halfDpadSize, dpadRadius);
	m_activeButtons[DPAD_UP] = TRUE;

	m_buttonPositions[DPAD_DOWN].left = margin + dpadSize;
	m_buttonPositions[DPAD_DOWN].top = screenHeight - margin - dpadSize;
	m_buttonPositions[DPAD_DOWN].right = m_buttonPositions[DPAD_DOWN].left + dpadSize;
	m_buttonPositions[DPAD_DOWN].bottom = m_buttonPositions[DPAD_DOWN].top + dpadSize;
	m_buttonHitAreas[DPAD_DOWN].Set(m_buttonPositions[DPAD_DOWN].left + halfDpadSize, m_buttonPositions[DPAD_DOWN].top + halfDpadSize, dpadRadius);
	m_activeButtons[DPAD_DOWN] = TRUE;

	m_buttonPositions[DPAD_LEFT].left = margin;
	m_buttonPositions[DPAD_LEFT].top = screenHeight - margin - (dpadSize * 2);
	m_buttonPositions[DPAD_LEFT].right = m_buttonPositions[DPAD_LEFT].left + dpadSize;
	m_buttonPositions[DPAD_LEFT].bottom = m_buttonPositions[DPAD_LEFT].top + dpadSize;
	m_buttonHitAreas[DPAD_LEFT].Set(m_buttonPositions[DPAD_LEFT].left + halfDpadSize, m_buttonPositions[DPAD_LEFT].top + halfDpadSize, dpadRadius);
	m_activeButtons[DPAD_LEFT] = TRUE;

	m_buttonPositions[DPAD_RIGHT].left = margin + (dpadSize * 2);
	m_buttonPositions[DPAD_RIGHT].top = screenHeight - margin - (dpadSize * 2);
	m_buttonPositions[DPAD_RIGHT].right = m_buttonPositions[DPAD_RIGHT].left + dpadSize;
	m_buttonPositions[DPAD_RIGHT].bottom = m_buttonPositions[DPAD_RIGHT].top + dpadSize;
	m_buttonHitAreas[DPAD_RIGHT].Set(m_buttonPositions[DPAD_RIGHT].left + halfDpadSize, m_buttonPositions[DPAD_RIGHT].top + halfDpadSize, dpadRadius);
	m_activeButtons[DPAD_RIGHT] = TRUE;

	m_buttonPositions[BUTTON_A].left = screenWidth - margin - buttonSize;
	m_buttonPositions[BUTTON_A].top = screenHeight - margin - buttonSize;
	m_buttonPositions[BUTTON_A].right = m_buttonPositions[BUTTON_A].left + buttonSize;
	m_buttonPositions[BUTTON_A].bottom = m_buttonPositions[BUTTON_A].top + buttonSize;
	m_buttonHitAreas[BUTTON_A].Set(m_buttonPositions[BUTTON_A].left + halfButtonSize, m_buttonPositions[BUTTON_A].top + halfButtonSize, buttonRadius);
	m_activeButtons[BUTTON_A] = TRUE;

	m_buttonPositions[BUTTON_B].left = screenWidth - (margin * 2) - (buttonSize * 2);
	m_buttonPositions[BUTTON_B].top = screenHeight - margin - buttonSize;
	m_buttonPositions[BUTTON_B].right = m_buttonPositions[BUTTON_B].left + buttonSize;
	m_buttonPositions[BUTTON_B].bottom = m_buttonPositions[BUTTON_B].top + buttonSize;
	m_buttonHitAreas[BUTTON_B].Set(m_buttonPositions[BUTTON_B].left + halfButtonSize, m_buttonPositions[BUTTON_B].top + halfButtonSize, buttonRadius);
	m_activeButtons[BUTTON_B] = TRUE;

	/*
	m_buttonPositions[BUTTON_L].left = screenWidth - (margin * 3) - (halfButtonSize * 3);
	m_buttonPositions[BUTTON_L].top = m_buttonPositions[BUTTON_A].top - margin - buttonSize;
	m_buttonPositions[BUTTON_L].right = m_buttonPositions[BUTTON_L].left + halfButtonSize;
	m_buttonPositions[BUTTON_L].bottom = m_buttonPositions[BUTTON_L].top + halfButtonSize;
	m_buttonHitAreas[BUTTON_L].Set(m_buttonPositions[BUTTON_L].left + quarterButtonSize, m_buttonPositions[BUTTON_L].top + quarterButtonSize, buttonRadius);
	m_activeButtons[BUTTON_L] = TRUE;

	m_buttonPositions[BUTTON_R].left = screenWidth - margin - halfButtonSize;
	m_buttonPositions[BUTTON_R].top = m_buttonPositions[BUTTON_A].top - margin - buttonSize;
	m_buttonPositions[BUTTON_R].right = m_buttonPositions[BUTTON_R].left + halfButtonSize;
	m_buttonPositions[BUTTON_R].bottom = m_buttonPositions[BUTTON_R].top + halfButtonSize;
	m_buttonHitAreas[BUTTON_R].Set(m_buttonPositions[BUTTON_R].left + quarterButtonSize, m_buttonPositions[BUTTON_R].top + quarterButtonSize, buttonRadius);
	m_activeButtons[BUTTON_R] = TRUE;
	 */

	m_buttonPositions[BUTTON_L].left = 0;
	m_buttonPositions[BUTTON_L].top = m_buttonPositions[DPAD_UP].top - (margin * 2) - (70 * m_gameApp->GetScreenScale());
	m_buttonPositions[BUTTON_L].right = screenWidth / 2 - (20 * m_gameApp->GetScreenScale());
	m_buttonPositions[BUTTON_L].bottom = m_buttonPositions[DPAD_UP].top - (margin * 2);
	//m_buttonHitAreas[BUTTON_L].Set(m_buttonPositions[BUTTON_L].left + quarterButtonSize, m_buttonPositions[BUTTON_L].top + quarterButtonSize, buttonRadius);
	m_bigHitAreaL = m_buttonPositions[BUTTON_L];
	m_activeButtons[BUTTON_L] = TRUE;
	
	m_buttonPositions[BUTTON_R].left = screenWidth - (screenWidth / 2 - (20 * m_gameApp->GetScreenScale()));
	m_buttonPositions[BUTTON_R].top = m_buttonPositions[BUTTON_A].top - (margin * 2) - (80 * m_gameApp->GetScreenScale());
	m_buttonPositions[BUTTON_R].right = screenWidth - 1;
	m_buttonPositions[BUTTON_R].bottom = m_buttonPositions[BUTTON_A].top - (margin * 2);
	//m_buttonHitAreas[BUTTON_R].Set(m_buttonPositions[BUTTON_R].left + quarterButtonSize, m_buttonPositions[BUTTON_R].top + quarterButtonSize, buttonRadius);
	m_bigHitAreaR = m_buttonPositions[BUTTON_R];
	m_activeButtons[BUTTON_R] = TRUE;
	
	m_cameraIconPosition.left = m_buttonPositions[BUTTON_L].left + margin + halfButtonSize;
	m_cameraIconPosition.top = m_buttonPositions[BUTTON_L].top;
	m_cameraIconPosition.right = m_cameraIconPosition.left + halfButtonSize;
	m_cameraIconPosition.bottom = m_cameraIconPosition.top + halfButtonSize;

	m_dpadCenterPosition.left = m_buttonPositions[DPAD_UP].left;
	m_dpadCenterPosition.top = m_buttonPositions[DPAD_UP].top + dpadSize;
	m_dpadCenterPosition.right = m_dpadCenterPosition.left + dpadSize;
	m_dpadCenterPosition.bottom = m_dpadCenterPosition.top + dpadSize;

	// set unused buttons
	m_buttonPositions[BUTTON_X] = Rect(0, 0, 0, 0);
	m_buttonPositions[BUTTON_Y] = Rect(0, 0, 0, 0);
	m_buttonPositions[BUTTON_START] = Rect(0, 0, 0, 0);
	m_buttonPositions[BUTTON_SELECT] = Rect(0, 0, 0, 0);
	m_buttonPositions[BUTTON_BACK] = Rect(0, 0, 0, 0);
}
