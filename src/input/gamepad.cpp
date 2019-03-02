#include "../framework/debug.h"

#include "gamepad.h"

#include "../framework/input/keyboard.h"
#include "virtualgamepad.h"

#include <string.h>

GamePad::GamePad()
{
	STACK_TRACE;
	m_keyboard = NULL;
	m_virtualGamePad = NULL;

	m_buttons = new BOOL[BUTTON_LAST];
	ASSERT(m_buttons != NULL);
	m_lockedButtons = new BOOL[BUTTON_LAST];
	ASSERT(m_lockedButtons != NULL);

	Reset();
}

GamePad::~GamePad()
{
	STACK_TRACE;
	SAFE_DELETE_ARRAY(m_buttons);
	SAFE_DELETE_ARRAY(m_lockedButtons);
}

void GamePad::Poll()
{
	STACK_TRACE;
	BOOL keyboardState[BUTTON_LAST];
	BOOL virtualState[BUTTON_LAST];
	memset(keyboardState, FALSE, sizeof(BOOL) * BUTTON_LAST);
	memset(virtualState, FALSE, sizeof(BOOL) * BUTTON_LAST);

	// collect keyboard state from keys mapping to gamepad buttons
	if (m_keyboard != NULL)
	{
		// TODO: make this better
		keyboardState[DPAD_UP] = m_keyboard->IsDown(KSYM_UP);
		keyboardState[DPAD_DOWN] = m_keyboard->IsDown(KSYM_DOWN);
		keyboardState[DPAD_LEFT] = m_keyboard->IsDown(KSYM_LEFT);
		keyboardState[DPAD_RIGHT] = m_keyboard->IsDown(KSYM_RIGHT);
#ifdef MOBILE
		keyboardState[BUTTON_A] = m_keyboard->IsDown(KSYM_BUTTON_A) || m_keyboard->IsDown(KSYM_Z);
		keyboardState[BUTTON_B] = m_keyboard->IsDown(KSYM_BUTTON_B) || m_keyboard->IsDown(KSYM_X);
		keyboardState[BUTTON_X] = m_keyboard->IsDown(KSYM_BUTTON_X) || m_keyboard->IsDown(KSYM_A);
		keyboardState[BUTTON_Y] = m_keyboard->IsDown(KSYM_BUTTON_Y) || m_keyboard->IsDown(KSYM_S);
		keyboardState[BUTTON_L] = m_keyboard->IsDown(KSYM_BUTTON_L1) || m_keyboard->IsDown(KSYM_Q);
		keyboardState[BUTTON_R] = m_keyboard->IsDown(KSYM_BUTTON_R1) || m_keyboard->IsDown(KSYM_W);
		keyboardState[BUTTON_START] = m_keyboard->IsDown(KSYM_BUTTON_START) || m_keyboard->IsDown(KSYM_RETURN);
		keyboardState[BUTTON_SELECT] = m_keyboard->IsDown(KSYM_BUTTON_SELECT) || m_keyboard->IsDown(KSYM_TAB);
		keyboardState[BUTTON_BACK] = m_keyboard->IsDown(KSYM_BACK) || m_keyboard->IsDown(KSYM_ESCAPE);
#else
		// TODO: these should definitely be configurable
		keyboardState[BUTTON_A] = m_keyboard->IsDown(KSYM_Z);
		keyboardState[BUTTON_B] = m_keyboard->IsDown(KSYM_X);
		keyboardState[BUTTON_X] = m_keyboard->IsDown(KSYM_A);
		keyboardState[BUTTON_Y] = m_keyboard->IsDown(KSYM_S);
		keyboardState[BUTTON_L] = m_keyboard->IsDown(KSYM_Q);
		keyboardState[BUTTON_R] = m_keyboard->IsDown(KSYM_W);
		keyboardState[BUTTON_START] = m_keyboard->IsDown(KSYM_RETURN);
		keyboardState[BUTTON_SELECT] = m_keyboard->IsDown(KSYM_TAB);
		keyboardState[BUTTON_BACK] = m_keyboard->IsDown(KSYM_ESCAPE);
#endif
	}

#ifdef MOBILE
	// collect virtual gamepad button state
	if (m_virtualGamePad != NULL && m_virtualGamePad->IsEnabled())
	{
		for (uint32_t b = 0; b < BUTTON_LAST; ++b)
			virtualState[b] = m_virtualGamePad->IsDown((GAMEPAD_BUTTONS)b);
	}
#endif

	// set the button states now based on the keyboard and virtual pad states
	// if at least one of them is set, then the button here will be set
	for (uint32_t b = 0; b < BUTTON_LAST; ++b)
	{
		BOOL overallState = keyboardState[b] || virtualState[b];
		SetButtonState((GAMEPAD_BUTTONS)b, overallState);
	}
}

BOOL GamePad::IsPressed(GAMEPAD_BUTTONS button)
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

void GamePad::Reset()
{
	STACK_TRACE;
	memset(m_buttons, FALSE, sizeof(BOOL) * BUTTON_LAST);
	memset(m_lockedButtons, FALSE, sizeof(BOOL) * BUTTON_LAST);
}

void GamePad::SetButtonState(GAMEPAD_BUTTONS button, BOOL state)
{
	STACK_TRACE;
	if (state)
	{
		m_buttons[button] = !(m_lockedButtons[button]);
	}
	else
	{
		m_buttons[button] = FALSE;
		m_lockedButtons[button] = FALSE;
	}
}

