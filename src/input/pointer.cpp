#include "../framework/debug.h"

#include "pointer.h"

#include "../framework/basegameapp.h"
#include "../framework/input/mouse.h"
#include "../framework/input/touchscreen.h"
#include "../framework/math/circle.h"
#include "../framework/math/rect.h"

Pointer::Pointer(BaseGameApp *gameApp)
{
	STACK_TRACE;
	m_gameApp = gameApp;
	m_isDown = FALSE;
	m_x = 0;
	m_y = 0;
	m_locked = FALSE;
}

Pointer::~Pointer()
{
	STACK_TRACE;
}

void Pointer::Poll()
{
	STACK_TRACE;
	Mouse *mouse = m_gameApp->GetMouse();
	Touchscreen *touchscreen = m_gameApp->GetTouchscreen();

	ASSERT(mouse != NULL || touchscreen != NULL);

	if (mouse != NULL)
	{
		// only caring about the left mouse button -- the "primary" button if you will
		if (mouse->IsDown(MOUSE_LEFT))
		{
			// down only if not locked
			m_isDown = !m_locked;
		}
		else
		{
			m_isDown = FALSE;
			m_locked = FALSE;
		}

		m_x = mouse->GetX();
		m_y = mouse->GetY();
	}
	else if (touchscreen != NULL)
	{
		// only caring about the "primary" touch point
		const TouchPointer *touchPoint = touchscreen->GetPrimaryPointer();

		if (touchPoint->IsTouching())
		{
			// down only if not locked
			m_isDown = !m_locked;
		}
		else
		{
			m_isDown = FALSE;
			m_locked = FALSE;
		}

		m_x = touchPoint->GetX();
		m_y = touchPoint->GetY();
	}
}

BOOL Pointer::IsPressed()
{
	STACK_TRACE;
	if (m_isDown && !m_locked)
	{
		m_locked = TRUE;
		return TRUE;
	}
	else
		return FALSE;
}

BOOL Pointer::IsWithin(uint16_t left, uint16_t top, uint16_t right, uint16_t bottom)
{
	STACK_TRACE;
	if (m_x >= left && m_y >= top && m_x <= right && m_y <= bottom)
		return TRUE;
	else
		return FALSE;
}

BOOL Pointer::IsWithin(const Rect &area)
{
	STACK_TRACE;
	if (m_x >= area.left && m_y >= area.top && m_x <= area.right && m_y <= area.bottom)
		return TRUE;
	else
		return FALSE;
}

BOOL Pointer::IsWithin(uint16_t centerX, uint16_t centerY, uint16_t radius)
{
	STACK_TRACE;
	uint32_t squaredDistance = ((centerX - m_x) * (centerX - m_x)) + ((centerY - m_y) * (centerY - m_y));
	uint32_t squaredRadius = radius * radius;
	if (squaredDistance <= squaredRadius)
		return TRUE;
	else
		return FALSE;
}

BOOL Pointer::IsWithin(const Circle &area)
{
	STACK_TRACE;
	uint32_t squaredDistance = ((area.x - m_x) * (area.x - m_x)) + ((area.y - m_y) * (area.y - m_y));
	uint32_t squaredRadius = area.radius * area.radius;
	if (squaredDistance <= squaredRadius)
		return TRUE;
	else
		return FALSE;
}

