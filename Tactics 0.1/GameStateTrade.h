#pragma once
#include "GameState.h"

class GameStateTrade : public GameState
{
public:
	GameStateTrade( GameData & gameData, CommonSprites& commonSprites,
		Point movePosition, Point mousePosition, int tradePartnerIndex );
	~GameStateTrade();

	void update() override;
	void drawToScreen() override;

private:
	Point mousePos; // remembering this for GameStateActionMenu
	Point movePos; // remembering this for GameStateActionMenu
	Unit * otherUnit;
	int otherUnitIndex;
	int menuIndex;
	bool isLeftHandMenu;
	bool wasTraded;
	bool isDragging;
	bool cameFromLeft;
	int originalIndex;

	RectD leftRect;
	RectD rightRect;
	int itemHeight;

	// graphics functions
	virtual void drawSelectedUnit() override;
	void drawInventory( Unit * unit, RectD & rect, int index );
};