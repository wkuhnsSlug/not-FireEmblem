#include "ScreenLevel.h"
#include "GameStateMap.h"
#include "GameStateEnemy.h"
#include "Potion.h"
#include "Medicine.h"
#include "ShaderContainer.h"
#include <fstream>

ScreenLevel::ScreenLevel( std::string filename, KeyboardBase& Kbds, MouseInterface& m, GraphicsInterface& Gfx, Screen ** ptr )
	:
	Screen( Kbds, m, Gfx, ptr ),
	gameData( tMap, playerUnits, enemyUnits, mouse, gfx, viewport, kbd )
{
	viewport.init( 0, 0, SCREENWIDTH, SCREENHEIGHT );
	gfx.setView( &viewport );
	mouse.setNewViewport( &viewport );
	tMap.init( "Assets\\Maps\\Lv01.txt" );
	viewport.setBounds( 0, 0, (float)( tMap.getMapWidth() * tMap.getTileWidth() ), (float)( tMap.getMapHeight() * tMap.getTileHeight() ) );
	int mapSize = tMap.getMapWidth() * tMap.getMapHeight();
	occupied = new OccupiedStatus[mapSize];
	for ( int i = 0; i < mapSize; i++ )
	{
		occupied[i] = NOT_OCCUPIED;
	}
	gameData.occupied = occupied;
	gameData.gameState = new GameStateMap( gameData, commonSprites );

	commonSprites.menuFont.init( "Assets\\MTCORSVA.TTF", 32 );
	commonSprites.menuFontSmall.init( "Assets\\MTCORSVA.TTF", 26 );

	commonSprites.mouseCursor.initialize( "Assets\\MouseCursor03.png" );
	commonSprites.cursorRed.initialize( "Assets\\Cursor.png" );
	commonSprites.cursorBlue.initialize( "Assets\\CursorBlue.png" );
	commonSprites.blueHighlight.initialize( "Assets\\BlueHighlight2.png" );
	commonSprites.redHighlight.initialize( "Assets\\RedHighlight2.png" );
	commonSprites.greenHighlight.initialize( "Assets\\GreenHighlight.png" );
	commonSprites.redSquare.initialize( "Assets\\RedHighlight.png" );
	commonSprites.menuBackgroundRed.initialize( "Assets\\MenuBackgroundRed.png" );
	commonSprites.menuBackgroundBlue.initialize( "Assets\\MenuBackgroundBlue.png" );
	commonSprites.battleForecastBackground.initialize( "Assets\\BattleForecast.png" );


	ShaderContainer::loadShader( "Assets\\Shaders\\grayscale.frag", "grayscale" );
	ShaderContainer::loadShader( "Assets\\Shaders\\null.frag", "null" );
	loadUnits( playerUnits, "Assets\\Maps\\Lv01Allies.txt", OCCUPIED_ALLY );
	loadUnits( enemyUnits, "Assets\\Maps\\Lv01Enemies.txt", OCCUPIED_ENEMY );

	commonSprites.attackAnimAlly.initialize( "Assets\\Dukattak336x285x10.png", 5, 2, 336, 285, 10, 4 );
	commonSprites.attackAnimEnemy.initialize( "Assets\\Dukattak336x285x10.png", 5, 2, 336, 285, 10, 4 );
	commonSprites.attackCritAlly.initialize( "Assets\\Dukcrit336x285x15.png", 5, 3, 336, 285, 15, 4 );
	commonSprites.attackCritEnemy.initialize( "Assets\\Dukcrit336x285x15.png", 5, 3, 336, 285, 15, 4 );
	commonSprites.attackStandAlly.initialize( "Assets\\Dukstand336x285x2.png", 2, 1, 336, 285, 2, 4 );
	commonSprites.attackStandEnemy.initialize( "Assets\\Dukstand336x285x2.png", 2, 1, 336, 285, 2, 4 );
	commonSprites.attackDodgeAlly.initialize( "Assets\\Dukdodge336x285x2.png", 2, 1, 336, 285, 2, 4 );
	commonSprites.attackDodgeEnemy.initialize( "Assets\\Dukdodge336x285x2.png", 2, 1, 336, 285, 2, 4 );
	commonSprites.attackBackground.initialize( "Assets\\AttackBackground.png" );

	// init movement ranges
	for ( unsigned i = 0; i < playerUnits.size(); i++ )
	{
		playerUnits[i]->calculateMovement( &tMap, occupied );
	}
	for ( unsigned i = 0; i < enemyUnits.size(); i++ )
	{
		enemyUnits[i]->calculateMovement( &tMap, occupied, OCCUPIED_ENEMY );
	}

	commonSprites.background.initialize( "Assets\\Level 1 v4.png" );
	isPlayerTurn = true;
}
ScreenLevel::~ScreenLevel()
{
	for ( unsigned i = 0; i < playerUnits.size(); i++ )
	{
		delete playerUnits[i];
	}
	delete[] occupied;
}

bool ScreenLevel::logic()
{
	// check for end turn
	if ( isPlayerTurn )
	{
		bool endTurn = true;
		if ( !gameData.shouldEndTurn )
		{
			for ( unsigned i = 0; endTurn && i < gameData.playerUnits.size(); i++ )
			{
				if ( gameData.playerUnits[i]->isActive() && !gameData.playerUnits[i]->isRescued() )
				{
					endTurn = false;
				}
			}
		}
		if ( endTurn ) // end player turn
		{
			for ( unsigned i = 0; endTurn && i < gameData.playerUnits.size(); i++ )
			{
				gameData.playerUnits[i]->calculateMovement( &gameData.tMap, gameData.occupied, OCCUPIED_ALLY );
				gameData.playerUnits[i]->setActive( true );
			}
			gameData.shouldEndTurn = false;

			delete gameData.gameState;
			gameData.gameState = new GameStateEnemy( gameData, commonSprites );
			isPlayerTurn = false;
		}
	}
	else // enemy turn
	{
		bool endTurn = true;
		if ( !gameData.shouldEndTurn )
		{
			for ( unsigned i = 0; endTurn && i < gameData.enemyUnits.size(); i++ )
			{
				if ( gameData.enemyUnits[i]->isActive() && !gameData.enemyUnits[i]->isRescued() )
				{
					endTurn = false;
				}
			}
		}
		if ( endTurn ) // end enemy turn
		{
			for ( unsigned i = 0; endTurn && i < gameData.enemyUnits.size(); i++ )
			{
				gameData.enemyUnits[i]->calculateMovement( &gameData.tMap, gameData.occupied, OCCUPIED_ENEMY );
				gameData.enemyUnits[i]->setActive( true );
			}
			gameData.shouldEndTurn = false;

			delete gameData.gameState;
			gameData.gameState = new GameStateMap( gameData, commonSprites );
			isPlayerTurn = true;
		}
	}


	gameData.gameState->update();

	return true;
}
void ScreenLevel::render()
{
	gameData.gameState->drawToScreen();
}

void ScreenLevel::loadUnits( std::vector<Unit*>& units, std::string filename, OccupiedStatus occ )
{
	std::ifstream infile( filename );
	std::string token, name;
	int x, y;
	while ( !infile.eof() )
	{
		infile >> token;

		if ( token == "unit" )
		{
			infile >> name >> x >> y;

			Unit * u = new Unit( name, x, y );
			units.push_back( u );
			occupied[tMap.getInt( { x, y } )] = occ;
		}
		else if ( token == "weapon" )
		{
			infile >> name;
			units[units.size() - 1]->addItem( new Weapon( loadWeapon( name ) ) );
		}
		else if ( token == "potion" )
		{
			infile >> name;
			units[units.size() - 1]->addItem( new Potion( loadPotion( name ) ) );
		}
		else if ( token == "medicine" )
		{
			infile >> name;
			units[units.size() - 1]->addItem( new Medicine( loadMedicine( name ) ) );
		}
	}

	infile.close();
}