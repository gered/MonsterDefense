#include "../../framework/debug.h"

#include "weaponselectionui.h"
#include "gameplaystate.h"
#include "../world.h"
#include "../entities/components/playercomponent.h"
#include "../entities/components/weaponcomponent.h"
#include "../entities/components/weaponlistcomponent.h"
#include "../entities/events/weaponswitchevent.h"
#include "../../gameapp.h"
#include "../../contexts/contentcache.h"
#include "../../entities/entity.h"
#include "../../entities/entitymanager.h"
#include "../../events/eventmanager.h"
#include "../../framework/graphics/texture.h"
#include "../../framework/graphics/textureatlas.h"
#include "../../framework/math/mathhelpers.h"
#include "../../input/gamepad.h"
#include "../../input/gamepadbuttons.h"
#include "../../states/gwengamestate.h"
#include "../../states/gwengamestateuicontroller.h"
#include "../../states/statemanager.h"
#include <gwen_structures.h>
#include <gwen_texture.h>
#include <controls/gwen_canvas.h>
#include <controls/gwen_base.h>
#include <controls/gwen_button.h>
#include <controls/gwen_label.h>
#include <string.h>

WeaponSelectionUI::WeaponSelectionUI(GwenGameState *gameState)
: GwenGameStateUIController(gameState)
{
	STACK_TRACE;
}

WeaponSelectionUI::~WeaponSelectionUI()
{
	STACK_TRACE;
}

void WeaponSelectionUI::OnPush()
{
	STACK_TRACE;
	ContentCache *contentCache = GetGameState()->GetGameApp()->GetContentCache();
	InitializeGwen(contentCache->GetUISkinFilename(), contentCache->GetUIFontFilename(), contentCache->GetUIFontSize());
	
	PositionControls();
}

void WeaponSelectionUI::PositionControls()
{
	STACK_TRACE;
	World *world = GetStateManager()->GetTopNonOverlayState()->As<GamePlayState>()->GetWorld();
	Entity *player = world->GetEntityManager()->GetWith<PlayerComponent>();
	if (player == NULL)
		return;

	ContentCache *content = GetGameState()->GetGameApp()->GetContentCache();
	WeaponComponent *currentWeapon = player->Get<WeaponComponent>();
	ASSERT(currentWeapon != NULL);
	WeaponListComponent *weaponList = player->Get<WeaponListComponent>();
	ASSERT(weaponList != NULL);

	GetCanvas()->RemoveAllChildren();
	
	Gwen::Controls::Label *lblTitleShadow = new Gwen::Controls::Label(GetCanvas());
	lblTitleShadow->SetText("Select A Weapon");
	lblTitleShadow->SetTextColor(Gwen::Color(0, 0, 0));
	lblTitleShadow->SetSize(GetCanvas()->Width(), 30);
	lblTitleShadow->SetPos(0, 20);
	lblTitleShadow->SetAlignment(Gwen::Pos::Center);

	Gwen::Controls::Label *lblTitle = new Gwen::Controls::Label(GetCanvas());
	lblTitle->SetText("Select A Weapon");
	lblTitle->SetTextColor(Gwen::Color(255, 255, 255));
	lblTitle->SetSize(GetCanvas()->Width(), 30);
	lblTitle->SetPos(lblTitleShadow->X() + 1, lblTitleShadow->Y() + 1);
	lblTitle->SetAlignment(Gwen::Pos::Center);
	
	Gwen::Controls::Button *btnCancel = new Gwen::Controls::Button(GetCanvas());
	btnCancel->SetText("Cancel");
	btnCancel->SetSize(100, 25);
	btnCancel->SetPos((GetCanvas()->Width() - btnCancel->Width()) / 2, GetCanvas()->Height() - btnCancel->Height() - 20);
	btnCancel->onPress.Add(this, &WeaponSelectionUI::OnCancel);
	
	m_buttonToWeaponMap.clear();
	
	const int BUTTON_SPACER = 15;
	const int WEAPON_BUTTON_WIDTH = 52;
	const int WEAPON_BUTTON_HEIGHT = 52;
	const int WEAPON_BUTTONS_PER_ROW = 5;

	int numWeapons = weaponList->weapons.size();
	const int WEAPON_BUTTON_NEEDED_ROWS = ceilf((float)numWeapons / (float)WEAPON_BUTTONS_PER_ROW);
	
	const int SELECTION_AREA_WIDTH = (WEAPON_BUTTON_WIDTH * WEAPON_BUTTONS_PER_ROW) + ((WEAPON_BUTTONS_PER_ROW - 1) * BUTTON_SPACER);
	const int SELECTION_AREA_HEIGHT = (WEAPON_BUTTON_HEIGHT * WEAPON_BUTTON_NEEDED_ROWS) + ((WEAPON_BUTTON_NEEDED_ROWS - 1) * BUTTON_SPACER);
	const int SELECTION_AREA_TOP = (GetCanvas()->Height() - SELECTION_AREA_HEIGHT) / 2;
	const int SELECTION_AREA_LEFT = (GetCanvas()->Width() - SELECTION_AREA_WIDTH) / 2;
		
	int currentRow = 0;
	int buttonsLeftToShow = numWeapons;
	int buttonsOnThisRow = (buttonsLeftToShow > WEAPON_BUTTONS_PER_ROW ? WEAPON_BUTTONS_PER_ROW : buttonsLeftToShow);
	buttonsLeftToShow -= buttonsOnThisRow;
	
	int currentWeaponIndex = 0;
	
	char buttonName[12];
	
	while (buttonsOnThisRow > 0)
	{
		// figure out starting x,y coords for this row of buttons
		int rowY = SELECTION_AREA_TOP + (currentRow * (WEAPON_BUTTON_HEIGHT + BUTTON_SPACER));
		int x = SELECTION_AREA_LEFT + (SELECTION_AREA_WIDTH - ((WEAPON_BUTTON_WIDTH * buttonsOnThisRow) + (BUTTON_SPACER * (buttonsOnThisRow - 1)))) / 2;
		
		while (buttonsOnThisRow > 0)
		{
			Weapon *currentWeapon = weaponList->weapons[currentWeaponIndex];

			// generate unique button control name based on the weapon index 
			// this new button is for
			snprintf(buttonName, 12, "btnWeapon%d", currentWeaponIndex);
			
			// add button to the screen
			Gwen::Controls::Button *btnWeapon = new Gwen::Controls::Button(GetCanvas(), buttonName);
			btnWeapon->SetPos(x, rowY);
			btnWeapon->SetSize(WEAPON_BUTTON_WIDTH, WEAPON_BUTTON_HEIGHT);
			btnWeapon->onPress.Add(this, &WeaponSelectionUI::OnSelect);
				
			// add mapping info from the button control name to the right Weapon
			// object so that the click event can figure out what Weapon it
			// should set on the player
			m_buttonToWeaponMap.insert(ButtonNameToWeaponMap::value_type(buttonName, currentWeapon->GetTypeOf()));
			
			// add weapon image to the button
			Gwen::Texture weaponImage;
			weaponImage.data = (void*)content->GetItems()->GetTexture();
			weaponImage.width = content->GetItems()->GetTexture()->GetWidth();
			weaponImage.height = content->GetItems()->GetTexture()->GetHeight();
			const TextureAtlasTile &weaponImageTile = content->GetItems()->GetTile(currentWeapon->GetImageIndex());
			btnWeapon->SetImage(weaponImage, weaponImageTile.texCoords.left, weaponImageTile.texCoords.top, weaponImageTile.texCoords.right, weaponImageTile.texCoords.bottom, true);
			
			// advance to the next button/column
			x += (WEAPON_BUTTON_WIDTH + BUTTON_SPACER);
			--buttonsOnThisRow;
			++currentWeaponIndex;
		}
		
		// advance to the next row of buttons
		++currentRow;
		buttonsOnThisRow = (buttonsLeftToShow > WEAPON_BUTTONS_PER_ROW ? WEAPON_BUTTONS_PER_ROW : buttonsLeftToShow);
		buttonsLeftToShow -= buttonsOnThisRow;
	}
}

void WeaponSelectionUI::OnPop()
{
	STACK_TRACE;
}

void WeaponSelectionUI::OnResize()
{
	STACK_TRACE;
	GwenGameStateUIController::OnResize();
	PositionControls();
}

void WeaponSelectionUI::OnUpdate(float delta)
{
	STACK_TRACE;
	GwenGameStateUIController::OnUpdate(delta);
	
	if (GetGameState()->IsTopState())
	{
		if (GetGameState()->GetGameApp()->GetGamePad()->IsPressed(BUTTON_BACK))
			SetStateFinished();
	}
}

void WeaponSelectionUI::OnSelect(Gwen::Controls::Base *sender)
{
	STACK_TRACE;
	stl::string buttonName = sender->GetName();
	ButtonNameToWeaponMap::const_iterator i = m_buttonToWeaponMap.find(buttonName);
	ASSERT(i != m_buttonToWeaponMap.end());
	if (i == m_buttonToWeaponMap.end())
		return;
	
	World *world = GetStateManager()->GetTopNonOverlayState()->As<GamePlayState>()->GetWorld();
	Entity *player = world->GetEntityManager()->GetWith<PlayerComponent>();
	ASSERT(player != NULL);
	if (player == NULL)
		return;

	WeaponSwitchEvent weaponSwitch(player);
	weaponSwitch.switchUsingType = TRUE;
	weaponSwitch.switchToType = i->second;
	GetGameState()->GetGameApp()->GetEventManager()->Trigger(&weaponSwitch);

	SetStateFinished();
}

void WeaponSelectionUI::OnCancel(Gwen::Controls::Base *sender)
{
	STACK_TRACE;
	SetStateFinished();
}
