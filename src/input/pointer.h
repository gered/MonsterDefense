#ifndef __INPUT_POINTER_H_INCLUDED__
#define __INPUT_POINTER_H_INCLUDED__

#include "../framework/common.h"

class BaseGameApp;
struct Circle;
struct Rect;

class Pointer
{
public:
	Pointer(BaseGameApp *gameApp);
	virtual ~Pointer();

	void Poll();

	BOOL IsDown()                                          { return m_isDown && !m_locked; }
	BOOL IsPressed();
	void Lock()                                            { m_locked = TRUE; }

	uint16_t GetX() const                                  { return m_x; }
	uint16_t GetY() const                                  { return m_y; }

	BOOL IsWithin(uint16_t left, uint16_t top, uint16_t right, uint16_t bottom);
	BOOL IsWithin(const Rect &area);
	BOOL IsWithin(uint16_t centerX, uint16_t centerY, uint16_t radius);
	BOOL IsWithin(const Circle &area);

private:
	BaseGameApp *m_gameApp;
	BOOL m_isDown;
	uint16_t m_x;
	uint16_t m_y;
	BOOL m_locked;

};

#endif

