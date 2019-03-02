#ifndef __INPUT_GAMEPAD_H_INCLUDED__
#define __INPUT_GAMEPAD_H_INCLUDED__

#include "../framework/common.h"
#include "gamepadbuttons.h"

class Keyboard;
class VirtualGamePad;

class GamePad
{
public:
	GamePad();
	virtual ~GamePad();

	void SetInputSource(Keyboard *keyboard)                { m_keyboard = keyboard; }
	void SetInputSource(VirtualGamePad *virtualGamePad)    { m_virtualGamePad = virtualGamePad; }

	void Poll();

	BOOL IsDown(GAMEPAD_BUTTONS button)                    { return m_buttons[button] && !m_lockedButtons[button]; }
	BOOL IsPressed(GAMEPAD_BUTTONS button);
	void Lock(GAMEPAD_BUTTONS button)                      { m_lockedButtons[button] = TRUE; }

	void Reset();

private:
	void SetButtonState(GAMEPAD_BUTTONS button, BOOL state);

	Keyboard *m_keyboard;
	VirtualGamePad *m_virtualGamePad;

	BOOL *m_buttons;
	BOOL *m_lockedButtons;

};

#endif

