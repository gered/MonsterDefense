#ifndef __INPUT_VIRTUALGAMEPAD_H_INCLUDED__
#define __INPUT_VIRTUALGAMEPAD_H_INCLUDED__

#include "../framework/common.h"

#include "gamepadbuttons.h"
#include "../framework/math/rect.h"

class GameApp;
class SpriteBatch;
class Texture;
class TextureAtlas;
struct Circle;

class VirtualGamePad
{
public:
	VirtualGamePad(GameApp *gameApp);
	virtual ~VirtualGamePad();

	void OnLoadGame();
	void OnResize();

	void Poll();
	void Render(SpriteBatch *spriteBatch);

	BOOL IsDown(GAMEPAD_BUTTONS button)                    { return m_buttons[button] && !m_lockedButtons[button]; }
	BOOL IsPressed(GAMEPAD_BUTTONS button);
	void Lock(GAMEPAD_BUTTONS button)                      { m_lockedButtons[button] = TRUE; }

	void SetEnabled(BOOL enabled);
	BOOL IsEnabled() const                                 { return m_enabled; }

private:
	void SizeAndPositionButtons();

	GameApp *m_gameApp;

	BOOL *m_buttons;
	BOOL *m_lockedButtons;
	BOOL *m_buttonsNotTouching;
	BOOL *m_activeButtons;

	TextureAtlas *m_textures;
	uint32_t *m_buttonUpTileIndexes;
	uint32_t *m_buttonDownTileIndexes;
	Rect *m_buttonPositions;
	Circle *m_buttonHitAreas;
	Rect m_bigHitAreaL;
	Rect m_bigHitAreaR;

	uint32_t m_cameraTileIndex;
	Rect m_cameraIconPosition;
	uint32_t m_dpadCenterTileIndex;
	Rect m_dpadCenterPosition;

	BOOL m_enabled;
};

#endif

