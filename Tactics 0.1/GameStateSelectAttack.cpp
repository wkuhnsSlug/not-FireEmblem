#include "GameStateSelectAttack.h"
#include "GameStateActionMenu.h"
#include "GameStateCombat.h"

GameStateSelectAttack::GameStateSelectAttack( GameData & gameData, CommonSprites& commonSprites, 
	Point movePosition, Point mousePosition )
	:
	GameState( gameData, commonSprites )
{
	mousePos = mousePosition;
	movePos = movePosition;
	idxSelectedWeapon = 0;
	selectedEnemy = NULL;
	g.playerUnits[g.selectedUnit]->calculateAttackRangeActive( movePos, &g.tMap );
}

GameStateSelectAttack::~GameStateSelectAttack()
{
	// ~~~
}


void GameStateSelectAttack::update()
{
	updateView();
	updateCursorPosition();
	int curpos = g.tMap.getInt( g.cursorpos );

	if ( g.occupied[curpos] == OCCUPIED_ENEMY )
	{
		// find enemy
		selectedEnemy = getUnit( g.cursorpos );

		// then check if he's in range
		//bool wasFound = false;
		std::vector<Point> * eir = g.playerUnits[g.selectedUnit]->getAttackRangeActive();
		/** /for ( unsigned i = 0; i < eir->size(); i++ )
		{
			if ( Tilemap::gridDistance( g.cursorpos, ( *eir )[i] ) == 0 )
			{
				wasFound = true;
				break;
			}
		}/**/
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
		while ( player->getInventory()[idxSelectedWeapon]->itemType != ITEM_WEAPON
			|| !player->canWield( (Weapon*)player->getInventory()[idxSelectedWeapon] )
			|| !pointIsInRange( g.cursorpos, Unit::getWeaponRange( movePos, 
				(Weapon*)player->getInventory()[idxSelectedWeapon], &g.tMap ) ) )
			/*( ( (Weapon*)player->getInventory()[idxSelectedWeapon] )->minRange
					> Tilemap::gridDistance( movePos, selectedEnemy->getPosition() )
					|| ( (Weapon*)player->getInventory()[idxSelectedWeapon] )->maxRange
					< Tilemap::gridDistance( movePos, selectedEnemy->getPosition() ) ) ) */
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
		} while ( player->getInventory()[idxSelectedWeapon]->itemType != ITEM_WEAPON
			|| !player->canWield( (Weapon*)player->getInventory()[idxSelectedWeapon] )
			|| !pointIsInRange( g.cursorpos, Unit::getWeaponRange( movePos,
				(Weapon*)player->getInventory()[idxSelectedWeapon], &g.tMap ) ) );
			/*( ( (Weapon*)player->getInventory()[idxSelectedWeapon] )->minRange
				> Tilemap::gridDistance( movePos, selectedEnemy->getPosition() )
				|| ( (Weapon*)player->getInventory()[idxSelectedWeapon] )->maxRange
				< Tilemap::gridDistance( movePos, selectedEnemy->getPosition() ) ) );*/
	}
	else if ( g.mouse.getMouseWheelDown() && selectedEnemy )
	{
		Unit * player = g.playerUnits[g.selectedUnit];
		do
		{
			idxSelectedWeapon = ( idxSelectedWeapon - 1 + player->getInventorySize() )
				% player->getInventorySize();
		} while ( player->getInventory()[idxSelectedWeapon]->itemType != ITEM_WEAPON
			|| !player->canWield( (Weapon*)player->getInventory()[idxSelectedWeapon] )
			|| !pointIsInRange( g.cursorpos, Unit::getWeaponRange( movePos,
				(Weapon*)player->getInventory()[idxSelectedWeapon], &g.tMap ) ) );
			/* ( ( (Weapon*)player->getInventory()[idxSelectedWeapon] )->minRange
				> Tilemap::gridDistance( movePos, selectedEnemy->getPosition() )
				|| ( (Weapon*)player->getInventory()[idxSelectedWeapon] )->maxRange
				< Tilemap::gridDistance( movePos, selectedEnemy->getPosition() ) ) );*/
	}
	else if ( g.mouse.getButtonRelease( LEFT_CLICK ) && selectedEnemy )
	{
		for ( int i = 0; i < (int)g.enemyUnits.size(); i++ )
		{
			Point enemyPos = g.enemyUnits[i]->getPosition();
			if ( curpos == g.tMap.getInt( enemyPos ) )
			{
				int griddist = Tilemap::gridDistance( enemyPos, movePos );
				if ( !( griddist >= g.playerUnits[g.selectedUnit]->getAttackRangeMin()
					&& griddist <= g.playerUnits[g.selectedUnit]->getAttackRangeMax() ) )
				{
					break; // can't attack
				}
				moveUnit( g.playerUnits[g.selectedUnit], movePos, true );
				g.playerUnits[g.selectedUnit]->equipWeapon( idxSelectedWeapon );
				g.playerUnits[g.selectedUnit]->setActive( false );

				g.gameState = new GameStateCombat( g, s, g.selectedUnit, i );
				delete this;
				return;

				break;
			}
		}
	}
}
void GameStateSelectAttack::drawToScreen()
{
	g.gfx.draw( &s.background, 0, 0 );

	drawEnemyRange();
	if ( g.selectedUnit >= 0 )
	{
		drawHighlights( g.playerUnits[g.selectedUnit]->getAttackRangeActive(), &s.redHighlight );
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
		drawBattleForecast( g.playerUnits[g.selectedUnit], selectedEnemy );
	}

	drawMouse();
}


void GameStateSelectAttack::drawSelectedUnit()
{
	g.gfx.draw( g.playerUnits[g.selectedUnit]->getSprite(), movePos, &g.tMap );
}
void GameStateSelectAttack::drawBattleForecast( Unit * ally, Unit * enemy )
{
	Point offset = { 1000, 20 };
	if ( g.mouse.getPositionWindowspace().y < 160 )
	{
		offset.y = SCREENHEIGHT - (s.battleForecastBackground.getHeight() + 20);
	}

	g.gfx.drawFixed( &s.battleForecastBackground, offset );

	Tile * terrainAlly = g.tMap.getTile( movePos );
	Tile * terrainEnemy = g.tMap.getTile( enemy->getPosition() );

	Weapon* allyWeapon = (Weapon*)ally->getInventory()[idxSelectedWeapon];

	int damageAlly, hitAlly, critAlly, damageEnemy, hitEnemy, critEnemy;
	getAttackStats( ally, enemy, allyWeapon, enemy->getWeapon(),
		terrainEnemy, damageAlly, hitAlly, critAlly );
	int weaponTriangle = getWeaponTriangleEffectiveness( allyWeapon->weaponType,
		enemy->getWeapon() ? enemy->getWeapon()->weaponType : WEAPON_NULL );

	std::string str = ally->getName();
	str += "\n" + allyWeapon->name;
	if ( weaponTriangle == 1 )
	{
		str += " /\\";
	}
	else if ( weaponTriangle == -1 )
	{
		str += " \\/";
	}
	str += "\n" + std::to_string( ally->getCurrentHP() ) + "/" + std::to_string( ally->getMaxHP() )
		+ "  HP  " + std::to_string( enemy->getCurrentHP() )
		+ "/" + std::to_string( enemy->getMaxHP() );

	int griddist = Tilemap::gridDistance( enemy->getPosition(), movePos );
	if ( griddist >= enemy->getAttackRangeMin()
		&& griddist <= enemy->getAttackRangeMax() )
	{
		// enemy can attack
		getAttackStats( enemy, ally, enemy->getWeapon(), allyWeapon,
			terrainAlly, damageEnemy, hitEnemy, critEnemy );

		str += "\n    " + std::to_string( damageAlly ) + ( ( ally->getSpeed() - enemy->getSpeed() >= 4 ) ? "x2" : "" )
			+ "  ATK  " + std::to_string( damageEnemy ) + ( ( enemy->getSpeed() - ally->getSpeed() >= 4 ) ? "x2" : "" );
		str += "\n   " + std::to_string( hitAlly )
			+ "  HIT  " + std::to_string( hitEnemy );
		str += "\n    " + std::to_string( critAlly )
			+ "  CRT  " + std::to_string( critEnemy );
	}
	else
	{
		str += "\n    " + std::to_string( damageAlly ) +
			( ( ally->getSpeed() - enemy->getSpeed() >= 4 ) ? "x2" : "" ) + "  ATK  --";
		str += "\n   " + std::to_string( hitAlly ) + "  HIT  --";
		str += "\n    " + std::to_string( critAlly ) + "  CRT  --";

	}
	str += "\n ";
	if ( weaponTriangle == -1 )
	{
		str += "/\\ ";
	}
	else if ( weaponTriangle == 1 )
	{
		str += "\\/ ";
	}
	else
	{
		str += "    ";
	}
	str += enemy->getWeapon()->name;
	str += "\n         " + enemy->getName();

	g.gfx.drawFixed( str, &s.menuFontSmall, (int)offset.x + 10, (int)offset.y, 10, 10, 10 );
}