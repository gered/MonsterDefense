#ifdef __S3E__
#ifndef __FRAMEWORK_INPUT_MARMALADEMOUSE_H_INCLUDED__
#define __FRAMEWORK_INPUT_MARMALADEMOUSE_H_INCLUDED__

#include "../common.h"
#include "mouse.h"
#include "s3ePointer.h"
#include <stl/set.h>

class MouseListener;

class MarmaladeMouse : public Mouse
{
public:
	MarmaladeMouse();
	virtual ~MarmaladeMouse();

	void ResetDeltas();

	BOOL OnButtonEvent(const s3ePointerEvent *eventArgs);
	BOOL OnMotionEvent(const s3ePointerMotionEvent *eventArgs);

	BOOL IsDown(MOUSE_BUTTONS button)                      { return m_buttons[button] && !m_lockedButtons[button]; }
	BOOL IsPressed(MOUSE_BUTTONS button);
	void Lock(MOUSE_BUTTONS button)                        { m_lockedButtons[button] = TRUE; }

	uint16_t GetX() const                                  { return m_x; }
	uint16_t GetY() const                                  { return m_y; }
	int16_t GetDeltaX() const                              { return m_deltaX; }
	int16_t GetDeltaY() const                              { return m_deltaY; }

	void Reset();

	void RegisterListener(MouseListener *listener);
	void UnregisterListener(MouseListener *listener);

private:
	stl::set<MouseListener*> m_listeners;
	BOOL *m_buttons;
	BOOL *m_lockedButtons;

	uint16_t m_x;
	uint16_t m_y;
	int16_t m_deltaX;
	int16_t m_deltaY;
};

#endif
#endif
