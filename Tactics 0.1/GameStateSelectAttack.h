#pragma once
#include "GameState.h"

class GameStateSelectAttack : public GameState
{
public:
	GameStateSelectAttack( GameData & gameData, CommonSprites& commonSprites, Point movePosition, Point mousePosition );
	virtual ~GameStateSelectAttack();

	void update() override;
	void drawToScreen() override;

protected:
	Point mousePos; // remembering this for GameStateActionMenu
	Point movePos;
	int idxSelectedWeapon;
	Unit * selectedEnemy;

	// gfx functions
	virtual void drawSelectedUnit() override;
	void drawBattleForecast( Unit * ally, Unit * enemy );
};