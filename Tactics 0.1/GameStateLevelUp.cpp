#include "GameStateLevelUp.h"
#include "GameStateMap.h"
#include "GameStateEnemy.h"
#include <string>

GameStateLevelUp::GameStateLevelUp( GameData & gameData, CommonSprites& commonSprites,
	Sprite * spr, int expGained, bool isPlayer )
	:
	GameState( gameData, commonSprites )
{
	background = spr;
	exp = expGained;
	counter = 0;
	isPlayerTurn = isPlayer;

	levelUp = g.playerUnits[g.selectedUnit]->addExperience( expGained );
	g.playerUnits[g.selectedUnit]->increaseLevel( levelUp );
}
GameStateLevelUp::~GameStateLevelUp()
{
	delete background;
}

void GameStateLevelUp::update()
{
	updateCursorPosition();
	counter++;
	if ( counter > 60 )
	{
		if ( isPlayerTurn )
		{
			g.selectedUnit = -1;
			g.gameState = new GameStateMap( g, s );
			delete this;
			return;
		}
		else
		{
			g.selectedUnit = -1;
			g.gameState = new GameStateEnemy( g, s );
			delete this;
			return;
		}
	}
}
void GameStateLevelUp::drawToScreen()
{
	g.gfx.draw( background, 0, 0 );

	RectD r;
	r.x = 730;
	r.y = 610;
	r.width = 460;
	r.height = 125;

	g.gfx.drawFixed( r, 208, 191, 157, 240 );
	std::string str = "EXP gained: " + std::to_string( exp );
	if ( levelUp ) str += "   LEVEL UP!";
	g.gfx.drawFixed( str, &s.menuFont, 750, 660, 20, 20, 20 );

	drawMouse();
}