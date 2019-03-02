#ifndef __GAME_GAMEPLAY_WEAPONSELECTIONUI_H_INCLUDED__
#define __GAME_GAMEPLAY_WEAPONSELECTIONUI_H_INCLUDED__

#include "../../states/gwengamestateuicontroller.h"
#include "../entities/weapons/weapon.h"
#include <stl/string.h>
#include <stl/map.h>

class GwenGameState;

namespace Gwen
{
	namespace Controls
	{
		class Base;
	}
}

typedef stl::map<stl::string, WEAPON_TYPE> ButtonNameToWeaponMap;

class WeaponSelectionUI : public GwenGameStateUIController
{
public:
	WeaponSelectionUI(GwenGameState *gameState);
	virtual ~WeaponSelectionUI();
	
	void OnPush();
	void OnPop();
	void OnResize();
	void OnUpdate(float delta);
	
	void OnSelect(Gwen::Controls::Base *sender);
	void OnCancel(Gwen::Controls::Base *sender);
	
private:
	void PositionControls();
	
	ButtonNameToWeaponMap m_buttonToWeaponMap;
};

#endif
