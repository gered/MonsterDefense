#ifndef __GAME_GAMEPLAY_STATUSUI_H_INCLUDED__
#define __GAME_GAMEPLAY_STATUSUI_H_INCLUDED__

#include "../../framework/common.h"
#include "../../processes/gwengameprocessuicontroller.h"

class GwenGameProcess;
class RenderContext;
struct Event;

namespace Gwen
{
	namespace Controls
	{
		class Base;
	}
}

class StatusUI : public GwenGameProcessUIController
{
public:
	StatusUI(GwenGameProcess *gameProcess);
	virtual ~StatusUI();
	
	void OnAdd();
	void OnResume(BOOL fromOverlay);
	void OnUpdate(float delta);
	void OnRender(RenderContext *renderContext);
	void OnResize();
	
	BOOL Handle(const Event *event);
	
	void OnWeaponsClick(Gwen::Controls::Base *sender);
	void OnMenuClick(Gwen::Controls::Base *sender);
	
private:
	void AddControls();
	void PositionControls();
	void SetCurrentWeaponImage();

	BOOL m_timerIsPaused;
	BOOL m_timerIsCountingDown;
	float m_timerTime;
};

#endif
