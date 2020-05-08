#include "GameStateSelectHeal.h"
#include "GameStateActionMenu.h"
#include "GameStateMap.h"
#include "Medicine.h"

GameStateSelectHeal::GameStateSelectHeal( GameData & gameData, CommonSprites& commonSprites,
	Point movePosition, Point mousePosition )
	:
	GameStateSelectAttack( gameData, commonSprites, movePosition, mousePosition )
{
	g.playerUnits[g.selectedUnit]->calculateAdjacentAllies( movePos, g.playerUnits );
}

GameStateSelectHeal::~GameStateSelectHeal()
{
	// ~~~
}

void GameStateSelectHeal::update()
{
	updateView();
	updateCursorPosition();
	int curpos = g.tMap.getInt( g.cursorpos );

	if ( g.occupied[curpos] == OCCUPIED_ALLY )
	{
		// find enemy
		selectedEnemy = getUnit( g.cursorpos );

		// then check if he's in range
		std::vector<Point> * eir = g.playerUnits[g.selectedUnit]->getAdjacentAllies();
		if ( !pointIsInRange( g.cursorpos, *eir ) )
		{
			selectedEnemy = NULL;
		}
	}
	else
	{
		selectedEnemy = NULL;
	}

	if ( selectedEnemy )
	{
		Unit * player = g.playerUnits[g.selectedUnit];
		while ( player->getInventory()[idxSelectedWeapon]->itemType != ITEM_HEAL_OTHER
			|| !player->canWield( (Medicine*)player->getInventory()[idxSelectedWeapon] )
			|| !pointIsInRange( g.cursorpos, *g.playerUnits[g.selectedUnit]->getAdjacentAllies() ) )
		{
			idxSelectedWeapon = ( idxSelectedWeapon + 1 ) % player->getInventorySize();
		}
	}



	if ( g.mouse.getButtonRelease( RIGHT_CLICK ) )
	{
		g.gameState = new GameStateActionMenu( g, s, movePos, mousePos );
		delete this;
		return;
	}
	else if ( g.mouse.getMouseWheelUp() && selectedEnemy )
	{
		Unit * player = g.playerUnits[g.selectedUnit];
		do
		{
			idxSelectedWeapon = ( idxSelectedWeapon + 1 ) % player->getInventorySize();
		} while ( player->getInventory()[idxSelectedWeapon]->itemType != ITEM_HEAL_OTHER
			|| !player->canWield( (Medicine*)player->getInventory()[idxSelectedWeapon] )
			|| !pointIsInRange( g.cursorpos, *g.playerUnits[g.selectedUnit]->getAdjacentAllies() ) );
	}
	else if ( g.mouse.getMouseWheelDown() && selectedEnemy )
	{
		Unit * player = g.playerUnits[g.selectedUnit];
		do
		{
			idxSelectedWeapon = ( idxSelectedWeapon - 1 + player->getInventorySize() )
				% player->getInventorySize();
		} while ( player->getInventory()[idxSelectedWeapon]->itemType != ITEM_HEAL_OTHER
			|| !player->canWield( (Medicine*)player->getInventory()[idxSelectedWeapon] )
			|| !pointIsInRange( g.cursorpos, *g.playerUnits[g.selectedUnit]->getAdjacentAllies() ) );
	}
	else if ( g.mouse.getButtonRelease( LEFT_CLICK ) && selectedEnemy )
	{
		for ( int i = 0; i < (int)g.playerUnits.size(); i++ )
		{
			Point enemyPos = g.playerUnits[i]->getPosition();
			if ( curpos == g.tMap.getInt( enemyPos ) )
			{
				int griddist = Tilemap::gridDistance( enemyPos, movePos );
				if ( griddist != 1 )
				{
					break; // can't attack
				}
				moveUnit( g.playerUnits[g.selectedUnit], movePos, true );
				//g.playerUnits[g.selectedUnit]->equipWeapon( idxSelectedWeapon );
				g.playerUnits[g.selectedUnit]->setActive( false );

				// calculate heal
				int healAmount = g.playerUnits[g.selectedUnit]->getSkill() +
					( (Medicine*)( g.playerUnits[g.selectedUnit]->getInventory() )[idxSelectedWeapon] )->recoveryAmount;
				int healTo = g.playerUnits[i]->getCurrentHP() + healAmount;
				int maxHP = g.playerUnits[i]->getMaxHP();
				g.playerUnits[i]->setCurrentHP( healTo > maxHP ? maxHP : healTo );

				// decrement uses
				g.playerUnits[g.selectedUnit]->getInventory()[idxSelectedWeapon]->currentUses--;
				if ( g.playerUnits[g.selectedUnit]->getInventory()[idxSelectedWeapon]->currentUses <= 0 )
				{
					g.playerUnits[g.selectedUnit]->removeItem( idxSelectedWeapon );
				}

				g.selectedUnit = -1;
				g.gameState = new GameStateMap( g, s );
				delete this;
				return;

				break;
			}
		}
	}
}
void GameStateSelectHeal::drawToScreen()
{
	g.gfx.draw( &s.background, 0, 0 );

	drawEnemyRange();
	if ( g.selectedUnit >= 0 )
	{
		drawHighlights( g.playerUnits[g.selectedUnit]->getAdjacentAllies(), &s.greenHighlight );
	}
	drawCursor();
	drawCharactersOnTilemap();
	Unit * u = getUnit( g.cursorpos );
	if ( u )
	{
		drawUnitThumbnail( u );
	}
	drawTerrainInfo();

	int curpos = g.tMap.getInt( g.cursorpos );
	/*for ( int i = 0; i < (int)g.enemyUnits.size(); i++ )
	{
		Point enemyPos = g.enemyUnits[i]->getPosition();
		if ( curpos == g.tMap.getInt( enemyPos ) )
		{
			int griddist = Tilemap::gridDistance( enemyPos, movePos );
			if ( griddist >= g.playerUnits[g.selectedUnit]->getAttackRangeMin()
				&& griddist <= g.playerUnits[g.selectedUnit]->getAttackRangeMax() )
			{
				drawBattleForecast( g.playerUnits[g.selectedUnit], u );
				break;
			}
		}
	}*/
	if ( selectedEnemy )
	{
		//drawBattleForecast( g.playerUnits[g.selectedUnit], selectedEnemy );
	}

	drawMouse();
}
