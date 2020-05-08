#pragma once
#include "GameState.h"

class GameStateActionMenu : public GameState
{
public:
	GameStateActionMenu( GameData & gameData, CommonSprites& commonSprites, 
		Point movePosition, Point cursorPosition, bool canUndo = true, bool isCantoMove = false );
	virtual ~GameStateActionMenu();

	void update() override;
	void drawToScreen() override;

private:
	enum ActionMenuState
	{
		AM_NORMAL,
		AM_ITEM,
		AM_TRADE,
		AM_RESCUE,
		AM_TRANSFER,
		AM_DROP
	};

	ActionMenuState amState;
	Point movePos;
	Point menuPosition;
	int numMenuElements;
	int currentMenuElement;
	int subMenuElement;
	int numSubMenuElements;
	ActionMenuType actionMenu;
	bool canReturn;

	// gfx functions
	virtual void drawSelectedUnit() override;
	void drawContextMenu();
	void drawWeaponsList();
};