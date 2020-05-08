#include "GameStateEnemy.h"
#include <iostream>
GameStateEnemy::GameStateEnemy( GameData & gameData, CommonSprites& commonSprites )
	:
	GameState( gameData, commonSprites )
{
	// ~~
	for (Unit * enemy : gameData.enemyUnits) {
		currentEnemies.push_back(enemy);
	}
}
GameStateEnemy::~GameStateEnemy()
{
	// ~~
}

void GameStateEnemy::update()
{
	// end enemy phase and give control back to player
	moveMeleeEnemies();
	g.shouldEndTurn = true;
}
void GameStateEnemy::drawToScreen()
{
	// draws the standard background
	g.gfx.draw( &s.background, 0, 0 );
	drawElevation();
	drawGridlines();
	drawEnemyRange();
	drawCharactersOnTilemap();
	
	// draws 'enemy turn' in the corner for clarity
	g.gfx.drawFixed( "Enemy Turn", &s.menuFont, 10, 10, 231, 64, 61 );
}
void GameStateEnemy::moveMeleeEnemies()
{
	for (Unit * enemy : currentEnemies) {
		if (pickPlayerInRange(enemy) == Point(-1, -1)) {
			//find closest player and move towards him
			return;
		}
		//move in legal attacking range of enemy
	}

}
Point GameStateEnemy::pickPlayerInRange(Unit * enemy)
{
	std::vector<Point> * possiblePlayers = enemy->getEnemiesInRange();
	if ((int)possiblePlayers->size() < 1) {
		return Point(-1, -1); //no player in range
	}
	for (int i = 0; i < (int)possiblePlayers->size();i++) {
		std::cout << "found player! X: " << possiblePlayers->at(i).x << " Y: " << possiblePlayers->at(i).y << "\n";
	}
	return Point(-1, -1);
}