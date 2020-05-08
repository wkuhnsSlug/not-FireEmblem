#pragma once
#include "GameState.h"

class GameStateStatusMenu : public GameState
{
public:
	GameStateStatusMenu( GameData & gameData, CommonSprites& commonSprites,
		int infoSelection, bool enemyInformation );
	virtual ~GameStateStatusMenu();

	void update() override;
	void drawToScreen() override;

private:
	int unitInfoSelection;
	bool isEnemyInformation;

	void drawUnitInformation( Unit * unit );
};