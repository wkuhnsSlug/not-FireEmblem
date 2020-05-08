#include "GameStateMap.h"
#include "GameStateActionMenu.h"
#include "GameStateStatusMenu.h"
#include "GameStateOptionsMenu.h"

GameStateMap::GameStateMap( GameData & gameData, CommonSprites& commonSprites )
	:
	GameState( gameData, commonSprites )
{
	// ~~~
}

GameStateMap::~GameStateMap()
{
	// ~~~
}


void GameStateMap::update()
{
	updateView();
	updateCursorPosition();
	updateStandardToggles();
	int curpos = g.tMap.getInt( g.cursorpos );

	if ( g.mouse.getButtonRelease( LEFT_CLICK ) )
	{
		if ( g.selectedUnit >= 0 )
		{
			std::vector<Point>* v = g.playerUnits[g.selectedUnit]->getMovement();
			for ( int i = 0; i < (int)v->size(); i++ )
			{
				if ( curpos == g.tMap.getInt( ( *v )[i] ) )
				{
					if ( g.occupied[curpos] == NOT_OCCUPIED
						|| curpos == g.tMap.getInt( g.playerUnits[g.selectedUnit]->getPosition() ) )
					{

						/** /
						menuPosition = g.mouse.getPositionWindowspace();
						movePos = g.cursorpos;
						currentMenuElement = 0;
						g.playerUnits[selectedUnit]->calculateAvailableActions( movePos, g.enemyUnits );
						actionMenu = ( *g.playerUnits[selectedUnit]->getAvailableActions() )[0];
						numMenuElements = ( g.playerUnits[selectedUnit]->getAvailableActions() )->size();
						mode = MENU_ACTION;
						/**/
						g.gameState = new GameStateActionMenu( g, s, g.cursorpos, g.mouse.getPositionWindowspace() );
						delete this;
						return;
					}
					break;
				}
			}
		}
		else // no unit selected
		{
			if ( g.occupied[curpos] == OCCUPIED_ALLY )
			{
				g.selectedUnit = getAllyUnitActiveIndex( g.cursorpos );
				if ( g.selectedUnit >= 0 )
				{
					g.playerUnits[g.selectedUnit]->calculateMovement( &g.tMap, g.occupied );
				}
			}
			else if ( g.occupied[curpos] == OCCUPIED_ENEMY )
			{
				int i = getEnemyUnitIndex( g.cursorpos );
				g.enemyUnits[i]->setMarked( !g.enemyUnits[i]->isMarked() );
				g.enemyUnits[i]->calculateMovement( &g.tMap, g.occupied );
			}
			else if ( g.occupied[curpos] == NOT_OCCUPIED )
			{
				// open options menu
				g.gameState = new GameStateOptionsMenu( g, s );
				delete this;
				return;
			}
		}
	}
	else if ( g.mouse.getButtonRelease( RIGHT_CLICK ) )
	{
		// check if there's a unit there
		bool isUnit = false;
		for ( int i = 0; i < (int)g.playerUnits.size(); i++ )
		{
			if ( curpos == g.tMap.getInt( g.playerUnits[i]->getPosition() ) )
			{
				/** /
				unitInfoSelection = i;
				isUnit = true;
				isEnemyInformation = false;
				mode = MENU_STATUS;
				/**/
				g.gameState = new GameStateStatusMenu( g, s, i, false );
				delete this;
				return;

				break;
			}
		}
		if ( !isUnit )
		{
			for ( int i = 0; i < (int)g.enemyUnits.size(); i++ )
			{
				if ( curpos == g.tMap.getInt( g.enemyUnits[i]->getPosition() ) )
				{
					/** /
					unitInfoSelection = i;
					isUnit = true;
					isEnemyInformation = true;
					mode = MENU_STATUS;
					/**/
					g.gameState = new GameStateStatusMenu( g, s, i, true );
					delete this;
					return;

					break;
				}
			}
		}

		if ( !isUnit )
		{
			// deselect units
			g.selectedUnit = -1;
		}

	}
	else if ( g.mouse.getMouseWheelUp() )
	{
		if ( g.selectedUnit >= 0 )
		{
			do
			{
				g.selectedUnit = ( g.selectedUnit + 1 ) % g.playerUnits.size();
			} while ( !g.playerUnits[g.selectedUnit]->isActive()
				|| g.playerUnits[g.selectedUnit]->isRescued() );
			g.playerUnits[g.selectedUnit]->calculateMovement( &g.tMap, g.occupied );
		}
	}
	else if ( g.mouse.getMouseWheelDown() )
	{
		if ( g.selectedUnit >= 0 )
		{
			do
			{
				g.selectedUnit = ( g.selectedUnit - 1 + g.playerUnits.size() ) % g.playerUnits.size();
			} while ( !g.playerUnits[g.selectedUnit]->isActive()
				|| g.playerUnits[g.selectedUnit]->isRescued() );
			g.playerUnits[g.selectedUnit]->calculateMovement( &g.tMap, g.occupied );
		}
	}
}
void GameStateMap::drawToScreen()
{
	g.gfx.draw( &s.background, 0, 0 );
	drawElevation();
	drawGridlines();

	drawEnemyRange();
	if ( g.selectedUnit >= 0 )
	{
		drawHighlights( g.playerUnits[g.selectedUnit]->getHealRange(), &s.greenHighlight );
		drawHighlights( g.playerUnits[g.selectedUnit]->getAttackRange(), &s.redHighlight );
		drawHighlights( g.playerUnits[g.selectedUnit]->getMovement(), &s.blueHighlight );
		g.gfx.draw( &s.cursorRed, (float)( g.playerUnits[g.selectedUnit]->getPosition().x * g.tMap.getTileWidth() ),
			(float)( g.playerUnits[g.selectedUnit]->getPosition().y * g.tMap.getTileHeight() ) );
	}
	else
	{
		Unit * u = getUnit( g.cursorpos );
		if ( u )
		{
			drawHighlights( u->getHealRange(), &s.greenHighlight, 150 );
			drawHighlights( u->getAttackRange(), &s.redHighlight, 150 );
			drawHighlights( u->getMovement(), &s.blueHighlight, 150 );
		}
	}
	drawCursor();
	drawCharactersOnTilemap();
	Unit * u = getUnit( g.cursorpos );
	if ( u )
	{
		drawUnitThumbnail( u );
	}
	drawTerrainInfo();

	drawMouse();
}