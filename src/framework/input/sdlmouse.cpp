#ifdef SDL
#include "../debug.h"

#include "sdlmouse.h"
#include "mouselistener.h"
#include "../sdlincludes.h"
#include "../sdlsystem.h"
#include <string.h>

// TODO: query hardware for number of buttons?
const int32_t NUM_BUTTONS = 5;

SDLMouse::SDLMouse(SDLSystem *system)
{
	STACK_TRACE;
	m_system = system;

	m_buttons = new BOOL[NUM_BUTTONS];
	ASSERT(m_buttons != NULL);
	m_lockedButtons = new BOOL[NUM_BUTTONS];
	ASSERT(m_lockedButtons != NULL);

	Reset();
}

SDLMouse::~SDLMouse()
{
	STACK_TRACE;
	SAFE_DELETE_ARRAY(m_buttons);
	SAFE_DELETE_ARRAY(m_lockedButtons);
}

void SDLMouse::ResetDeltas()
{
	STACK_TRACE;
	m_deltaX = 0;
	m_deltaY = 0;
}

BOOL SDLMouse::OnButtonEvent(const SDL_MouseButtonEvent *eventArgs)
{
	STACK_TRACE;
	// translate from SDL's button values to our own MOUSE_BUTTONS enum
	int32_t button = (int32_t)eventArgs->button - 1;

	if (eventArgs->state == SDL_PRESSED)
	{
		// Pressed only if not locked
		m_buttons[button] = !(m_lockedButtons[button]);
		
		uint16_t x = eventArgs->x;
		uint16_t y = eventArgs->y;

		// always report button down events
		// NOTE: we're ignoring the "locked button" state because listeners
		//       don't have support for it (yet)
		for (stl::set<MouseListener*>::iterator i = m_listeners.begin(); i != m_listeners.end(); ++i)
		{
			if ((*i)->OnMouseButtonDown((MOUSE_BUTTONS)button, x, y))
				break;
		}
	}
	else
	{
		uint16_t x = eventArgs->x;
		uint16_t y = eventArgs->y;

		// if the button is just being released this tick, then trigger an event in all listeners
		if (m_buttons[button])
		{
			for (stl::set<MouseListener*>::iterator i = m_listeners.begin(); i != m_listeners.end(); ++i)
			{
				if ((*i)->OnMouseButtonUp((MOUSE_BUTTONS)button, x, y))
					break;
			}
		}

		m_buttons[button] = FALSE;
		m_lockedButtons[button] = FALSE;
	}

	return TRUE;
}

BOOL SDLMouse::OnMotionEvent(const SDL_MouseMotionEvent *eventArgs)
{
	STACK_TRACE;
	m_deltaX = eventArgs->x - m_x;
	m_deltaY = eventArgs->y - m_y;

	m_x = eventArgs->x;
	m_y = eventArgs->y;

	// raise listener events for the mouse position only if it's moved this tick
	if (m_deltaX != 0 || m_deltaY != 0)
	{
		for (stl::set<MouseListener*>::iterator i = m_listeners.begin(); i != m_listeners.end(); ++i)
		{
			if ((*i)->OnMouseMove(m_x, m_y, m_deltaX, m_deltaY))
				break;
		}
	}

	return TRUE;
}

BOOL SDLMouse::IsPressed(MOUSE_BUTTONS button)
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

void SDLMouse::Reset()
{
	STACK_TRACE;
	memset(m_buttons, FALSE, sizeof(BOOL) * NUM_BUTTONS);
	memset(m_lockedButtons, FALSE, sizeof(BOOL) * NUM_BUTTONS);
	m_x = 0;
	m_y = 0;
	m_deltaX = 0;
	m_deltaY = 0;
}

void SDLMouse::RegisterListener(MouseListener *listener)
{
	m_listeners.insert(listener);
}

void SDLMouse::UnregisterListener(MouseListener *listener)
{
	m_listeners.erase(listener);
}

#endif

