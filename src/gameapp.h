#ifndef __GAMEAPP_H_INCLUDED__
#define __GAMEAPP_H_INCLUDED__

#include "framework/common.h"
#include "framework/basegameapp.h"
#include "contexts/rendercontext.h"

class ContentCache;
class EventManager;
class GamePad;
class Pointer;
class StateManager;
class VirtualGamePad;
struct GameModeSettings;
struct GameOptions;

class GameApp : public BaseGameApp
{
public:
	GameApp();
	virtual ~GameApp();

	void OnAppGainFocus();
	void OnAppLostFocus();
	void OnAppPause();
	void OnAppResume();
	BOOL OnInit();
	void OnLoadGame();
	void OnLostContext();
	void OnNewContext();
	void OnRender();
	void OnResize();
	void OnUpdate(float delta);

	ContentCache* GetContentCache() const                  { return m_contentCache; }
	StateManager* GetStateManager() const                  { return m_stateManager; }
	GamePad* GetGamePad() const                            { return m_gamePad; }
	Pointer* GetPointer() const                            { return m_pointer; }
	VirtualGamePad* GetVirtualGamePad() const              { return m_virtualGamePad; }
	EventManager* GetEventManager() const                  { return m_eventManager; }
	GameOptions* GetOptions() const                        { return m_options; }
	GameModeSettings* GetModeSettings() const              { return m_modeSettings; }

	uint32_t GetScreenScale() const                        { return m_renderContext->GetScreenScale(); }
	uint8_t GetSmallFontSize() const                       { return m_smallFontSize; }
	uint8_t GetNormalFontSize() const                      { return m_normalFontSize; }
	uint8_t GetBigFontSize() const                         { return m_bigFontSize; }

private:
	void RecalculateScreenScale();
	void RecalculateFontSizes();

	uint8_t m_screenScale;
	uint8_t m_smallFontSize;
	uint8_t m_normalFontSize;
	uint8_t m_bigFontSize;

	ContentCache *m_contentCache;
	RenderContext *m_renderContext;
	StateManager *m_stateManager;
	GamePad *m_gamePad;
	Pointer *m_pointer;
	VirtualGamePad *m_virtualGamePad;
	EventManager *m_eventManager;
	GameOptions *m_options;
	GameModeSettings *m_modeSettings;
};

#endif
