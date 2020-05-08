#pragma once
#include "GameState.h"

class GameStateEnemy : public GameState
{
public:
	GameStateEnemy( GameData & gameData, CommonSprites& commonSprites );
	virtual ~GameStateEnemy();
	
	void update() override;
	void drawToScreen() override;
	
	void moveMeleeEnemies();
	Point pickPlayerInRange(Unit * enemy);

private:

	std::vector<Unit *> currentEnemies;
};