#include "GameState.h"

GameState::GameState( GameData & gameData, CommonSprites& commonSprites )
	:
	g( gameData ),
	s( commonSprites )
{
	// ~~~
}
GameState::~GameState()
{
	// ~~~
}

Unit * GameState::getUnit( Point p, Tilemap * map, 
	OccupiedStatus * occupied, std::vector<Unit*> & playerUnits )
{
	int idx = map->getInt( p );
	OccupiedStatus occ = occupied[idx];
	if ( occ == OCCUPIED_ALLY )
	{
		for ( unsigned i = 0; i < playerUnits.size(); i++ )
		{
			if ( idx == map->getInt( playerUnits[i]->getPosition() ) && !playerUnits[i]->isRescued() )
			{
				return playerUnits[i];
			}
		}
	}
	
	return NULL;
}

// drawing functions
void GameState::drawHighlights( std::vector<Point>* v, Sprite* highlight, int opacity )
{
	for ( int i = 0; i < (int)v->size(); i++ )
	{
		g.gfx.draw( highlight, (float)( ( *v )[i].x * g.tMap.getTileWidth() ), (float)( ( *v )[i].y * g.tMap.getTileHeight() ),
			255, 255, 255, opacity );
	}
}
void GameState::drawEnemyRange()
{
	int mapSize = g.tMap.getMapWidth() * g.tMap.getMapHeight();
	int mapWidth = g.tMap.getMapWidth();
	int mapHeight = g.tMap.getMapHeight();

	bool * drawSquare = new bool[mapSize];
	for ( int i = 0; i < mapSize; i++ )
	{
		drawSquare[i] = false;
	}

	for ( unsigned i = 0; i < g.enemyUnits.size(); i++ )
	{
		if ( g.drawEnemyRange || g.enemyUnits[i]->isMarked() )
		{
			std::vector<Point>*v = g.enemyUnits[i]->getMovement();
			for ( unsigned j = 0; j < v->size(); j++ )
			{
				drawSquare[g.tMap.getInt( (*v)[j] )] = true;
			}

			v = g.enemyUnits[i]->getAttackRange();
			for ( unsigned j = 0; j < v->size(); j++ )
			{
				drawSquare[g.tMap.getInt( ( *v )[j] )] = true;
			}
			//drawHighlights( g.enemyUnits[i]->getMovement(), &s.redHighlight );
			//drawHighlights( g.enemyUnits[i]->getAttackRange(), &s.redHighlight );
		}
	}

	// now draw the squares
	for ( int i = 0; i < mapSize; i++ )
	{
		if ( drawSquare[i] )
		{
			Point p = g.tMap.getPoint( i );
			g.gfx.draw( &s.redSquare, (float)( p.x * g.tMap.getTileWidth() ), (float)( p.y * g.tMap.getTileHeight() ),
				255, 255, 255, 255 );//120);
		}
	}

	// and finally, draw the border
	//std::vector<Line> vLines;
	unsigned r, gc, b, a;
	r = 255;
	gc = 0;
	b = 0;
	a = 255;
	float thickness = 3;
	// top and left, literally edge cases
	// top
	for ( int x = 1; x < mapHeight; x++ )
	{
		Line l;
		int y = 0;
		l.changeStart( x * g.tMap.getTileWidth(), y * g.tMap.getTileHeight() );
		if ( drawSquare[g.tMap.getInt( { x, y } )] )
		{
			if ( !drawSquare[g.tMap.getInt( { x - 1, y } )] )
			{
				// draw line on screen left
				l.changeEnd( x * g.tMap.getTileWidth(), ( y + 1 ) * g.tMap.getTileHeight() );
				// vLines.push_back( l );
				g.gfx.draw( l, r, gc, b, a, thickness );
			}
		}
		else
		{
			if ( drawSquare[g.tMap.getInt( { x - 1, y } )] )
			{
				// draw line on screen left
				l.changeEnd( x * g.tMap.getTileWidth(), ( y + 1 ) * g.tMap.getTileHeight() );
				// vLines.push_back( l );
				g.gfx.draw( l, r, gc, b, a, thickness );
			}
		}
	}
	// right
	for ( int y = 1; y < mapHeight; y++ )
	{
		int x = 0;
		Line l;
		l.changeStart( x * g.tMap.getTileWidth(), y * g.tMap.getTileHeight() );
		if ( drawSquare[g.tMap.getInt( { x, y } )] )
		{
			if ( !drawSquare[g.tMap.getInt( { x, y - 1 } )] )
			{
				// draw line above
				l.changeEnd( ( x + 1 ) * g.tMap.getTileWidth(), y * g.tMap.getTileHeight() );
				// vLines.push_back( l );
				g.gfx.draw( l, r, gc, b, a, thickness );
			}
		}
		else
		{
			if ( drawSquare[g.tMap.getInt( { x, y - 1 } )] )
			{
				// draw line above
				l.changeEnd( ( x + 1 ) * g.tMap.getTileWidth(), y * g.tMap.getTileHeight() );
				// vLines.push_back( l );
				g.gfx.draw( l, r, gc, b, a, thickness );
			}
		}
	}


	// the rest
	for ( int x = 1; x < mapWidth; x++ )
	{
		for ( int y = 1; y < mapHeight; y++ )
		{
			Line l;
			l.changeStart( x * g.tMap.getTileWidth(), y * g.tMap.getTileHeight() );
			if ( drawSquare[g.tMap.getInt( { x, y } )] )
			{
				if ( !drawSquare[g.tMap.getInt( { x - 1, y } )] )
				{
					// draw line on screen left
					l.changeEnd( x * g.tMap.getTileWidth(), ( y + 1 ) * g.tMap.getTileHeight() );
					// vLines.push_back( l );
					g.gfx.draw( l, r, gc, b, a, thickness );
				}
				if ( !drawSquare[g.tMap.getInt( { x, y - 1 } )] )
				{
					// draw line above
					l.changeEnd( ( x + 1 ) * g.tMap.getTileWidth(), y * g.tMap.getTileHeight() );
					// vLines.push_back( l );
					g.gfx.draw( l, r, gc, b, a, thickness );
				}
			}
			else
			{
				if ( drawSquare[g.tMap.getInt( { x - 1, y } )] )
				{
					// draw line on screen left
					l.changeEnd( x * g.tMap.getTileWidth(), ( y + 1 ) * g.tMap.getTileHeight() );
					// vLines.push_back( l );
					g.gfx.draw( l, r, gc, b, a, thickness );
				}
				if ( drawSquare[g.tMap.getInt( { x, y - 1 } )] )
				{
					// draw line above
					l.changeEnd( ( x + 1 ) * g.tMap.getTileWidth(), y * g.tMap.getTileHeight() );
					// vLines.push_back( l );
					g.gfx.draw( l, r, gc, b, a, thickness );
				}
			}
		}
	}
	//g.gfx.drawContiguousLine( vLines, 5, 255, 0, 0, 200 );

	delete[] drawSquare;
}
void GameState::drawElevation()
{
	if ( g.drawElevation )
	{
		// this is gross but fuck it
		Color elevColors[10];
		elevColors[0] = { 231, 53, 28, 255 };
		elevColors[1] = { 246, 161, 33, 255 };
		elevColors[2] = { 255, 251, 49, 255 };
		elevColors[3] = { 94, 216, 44, 255 };
		elevColors[4] = { 32, 213, 216, 255 };
		elevColors[5] = { 25, 101, 225, 255 };
		elevColors[6] = { 28, 1, 186, 255 };
		elevColors[7] = { 145, 60, 219, 255 };
		elevColors[8] = { 253, 250, 255, 255 };
		elevColors[9] = { 17, 18, 18, 255 };

		for ( unsigned x = 0; x < g.tMap.getMapWidth(); x++ )
		{
			for ( unsigned y = 0; y < g.tMap.getMapHeight(); y++ )
			{
				Color c = elevColors[g.tMap.getElevation( x, y )];
				RectD rect( x * g.tMap.getTileWidth(), y * g.tMap.getTileHeight(), g.tMap.getTileWidth(), g.tMap.getTileHeight() );
				g.gfx.draw( rect, c.r, c.g, c.b, 170 );
			}
		}
	}
}
void GameState::drawCharactersOnTilemap()
{
	for ( unsigned i = 0; i < g.playerUnits.size(); i++ )
	{
		if ( g.playerUnits[i]->isRescued() ) continue;

		if ( g.selectedUnit == i )
		{
			drawSelectedUnit();
		}
		else
		{
			g.gfx.draw( g.playerUnits[i]->getSprite(), g.playerUnits[i]->getPosition(), &g.tMap );
		}
	}
	for ( unsigned i = 0; i < g.enemyUnits.size(); i++ )
	{
		g.gfx.draw( g.enemyUnits[i]->getSprite(), g.enemyUnits[i]->getPosition(), &g.tMap );
	}

}
void GameState::drawSelectedUnit()
{
	g.gfx.draw( g.playerUnits[g.selectedUnit]->getSprite(), g.playerUnits[g.selectedUnit]->getPosition(), &g.tMap );
}
void GameState::drawCursor()
{
	g.gfx.draw( &s.cursorBlue, (float)( g.cursorpos.x * g.tMap.getTileWidth() ), 
		(float)( g.cursorpos.y * g.tMap.getTileHeight() ) );
}
void GameState::drawMouse()
{
	g.gfx.drawFixed( &s.mouseCursor, (float)g.mouse.getPositionWindowspace().x,
		(float)g.mouse.getPositionWindowspace().y );
}
void GameState::drawUnitThumbnail( Unit * unit )
{
	Point offset = { 20, 20 };
	if ( g.mouse.getPositionWindowspace().y < 160
		&& g.mouse.getPositionWindowspace().x < 600 )
	{
		offset.y = 600;
	}

	RectD rect;
	rect.x = offset.x;
	rect.y = offset.y;
	rect.width = 420;
	rect.height = 80;
	g.gfx.drawFixed( rect, 208, 191, 157, 240 );


	std::string str = unit->getName();
	str += "    HP " + std::to_string( unit->getCurrentHP() )
		+ "/" + std::to_string( unit->getMaxHP() );
	str += "     " + (unit->getWeapon() ? unit->getWeapon()->name: "--");

	str += "\nATK " + ( unit->getWeapon() ? std::to_string( unit->getAttack() ) : "-" );
	str += "  DEF " + std::to_string( unit->getDefense() );
	str += "  AVO " + std::to_string( unit->getAvo() );
	str += "  HIT " + std::to_string( unit->getHit() );
	str += "  CRT " + std::to_string( unit->getCrit() );

	g.gfx.drawFixed( str, &s.menuFontSmall, (int)offset.x + 10, (int)offset.y, 10, 10, 10 );
}
void GameState::drawTerrainInfo()
{
	Tile * t = g.tMap.getTile( g.cursorpos );

	Point offset = { 1660, 920 };
	if ( g.mouse.getPositionWindowspace().y >= 880
		&& g.mouse.getPositionWindowspace().x >= 1600 )
	{
		offset.y = 20;
	}

	RectD rect;
	rect.x = offset.x;
	rect.y = offset.y;
	rect.width = 190;
	rect.height = 110;
	g.gfx.drawFixed( rect, 208, 191, 157, 240 );

	std::string str;
	str = t->getName();
	str += "\n(" + std::to_string( (int)g.cursorpos.x ) + ", " + std::to_string( (int)g.cursorpos.y )
		+ ")    Elv: " + std::to_string( g.tMap.getElevation( g.cursorpos ) );
	str += "\nAvo: " + std::to_string( t->getAvo() ) + "  Def:" + std::to_string( t->getDef() );

	g.gfx.drawFixed( str, &s.menuFontSmall, (int)offset.x + 10, (int)offset.y, 10, 10, 10 );
}
void GameState::drawGridlines()
{
	if ( !g.drawGrid ) return;
	int opacity = 200;

	for ( unsigned i = 0; i < g.tMap.getMapHeight(); i++ )
	{
		Line l( 0, i * g.tMap.getTileWidth(), g.tMap.getTileWidth() * g.tMap.getMapWidth(), i * g.tMap.getTileWidth() );
		g.gfx.draw( l, 0, 0, 0, opacity );
	}
	for ( unsigned i = 0; i < g.tMap.getMapWidth(); i++ )
	{
		Line l( i * g.tMap.getTileWidth(), 0, i * g.tMap.getTileWidth(), g.tMap.getTileHeight() * g.tMap.getMapHeight() );
		g.gfx.draw( l, 0, 0, 0, opacity );
	}
}


// utility functions
void GameState::updateView()
{
	Point pMouse, pViewport;
	pMouse = g.mouse.getPositionWindowspace();
	if ( pMouse.x <0 || pMouse.y < 0 || pMouse.x > SCREENWIDTH || pMouse.y > SCREENHEIGHT )
	{
		return;
	}


	pMouse = g.mouse.getPositionWindowspace();
	pViewport = g.viewport.getCenter();
	double tWid, tHei;
	tWid = (double)g.tMap.getTileWidth();
	tHei = (double)g.tMap.getTileHeight();
	int moveAmount = 8;
	if ( pMouse.x < tWid )
	{
		g.viewport.center( (float)(pViewport.x - moveAmount), (float)pViewport.y );
	}
	else if ( pMouse.x > SCREENWIDTH - tWid )
	{
		g.viewport.center( (float)( pViewport.x + moveAmount ), (float)pViewport.y );
	}

	pViewport = g.viewport.getCenter();
	if ( pMouse.y < tHei )
	{
		g.viewport.center( (float)pViewport.x, (float)( pViewport.y - moveAmount ) );
	}
	else if ( pMouse.y > SCREENHEIGHT - tHei )
	{
		g.viewport.center( (float)pViewport.x, (float)( pViewport.y + moveAmount ) );
	}
}
void GameState::updateCursorPosition()
{
	g.cursorpos.x = floor( g.mouse.getPositionWorldspace().x / g.tMap.getTileWidth() );
	g.cursorpos.y = floor( g.mouse.getPositionWorldspace().y / g.tMap.getTileHeight() );
	// clamp cursorPos
	if ( g.cursorpos.x < 0 ) g.cursorpos.x = 0;
	if ( g.cursorpos.y < 0 ) g.cursorpos.y = 0;
	if ( g.cursorpos.x >= g.tMap.getMapWidth() ) g.cursorpos.x = g.tMap.getMapWidth() - 1;
	if ( g.cursorpos.y >= g.tMap.getMapHeight() ) g.cursorpos.y = g.tMap.getMapHeight() - 1;
}
void GameState::updateStandardToggles()
{
	if ( g.kbd.getKeyPressedThisFrame( KBD_1 ) )
	{
		g.drawGrid = !g.drawGrid;
	}

	// mutually exclusive
	if ( g.kbd.getKeyPressedThisFrame( KBD_2 ) )
	{
		g.drawEnemyRange = !g.drawEnemyRange;
		if ( g.drawEnemyRange )
		{
			g.drawElevation = false;
		}
	}
	else if ( g.kbd.getKeyPressedThisFrame( KBD_3 ) )
	{
		g.drawElevation = !g.drawElevation;
		if ( g.drawElevation )
		{
			g.drawEnemyRange = false;
		}
	}
}
int GameState::getAllyUnitIndex( Point p )
{
	int idx = g.tMap.getInt( p );
	OccupiedStatus occ = g.occupied[idx];
	if ( occ == OCCUPIED_ALLY )
	{
		for ( unsigned i = 0; i < g.playerUnits.size(); i++ )
		{
			if ( idx == g.tMap.getInt( g.playerUnits[i]->getPosition() ) && !g.playerUnits[i]->isRescued() )
			{
				return i;
			}
		}
	}
	return -1;
}
int GameState::getEnemyUnitIndex( Point p )
{
	int idx = g.tMap.getInt( p );
	OccupiedStatus occ = g.occupied[idx];
	if ( occ == OCCUPIED_ENEMY )
	{
		for ( unsigned i = 0; i < g.enemyUnits.size(); i++ )
		{
			if ( idx == g.tMap.getInt( g.enemyUnits[i]->getPosition() ) )
			{
				return i;
			}
		}
	}
	return -1;
}
int GameState::getAllyUnitActiveIndex( Point p )
{
	int idx = g.tMap.getInt( p );
	OccupiedStatus occ = g.occupied[idx];
	if ( occ == OCCUPIED_ALLY )
	{
		for ( unsigned i = 0; i < g.playerUnits.size(); i++ )
		{
			if ( idx == g.tMap.getInt( g.playerUnits[i]->getPosition() )
				&& !g.playerUnits[i]->isRescued() && g.playerUnits[i]->isActive() )
			{
				return i;
			}
		}
	}
	return -1;
}
Unit * GameState::getUnit( Point p )
{
	int idx = g.tMap.getInt( p );
	OccupiedStatus occ = g.occupied[idx];
	if ( occ == OCCUPIED_ALLY )
	{
		for ( unsigned i = 0; i < g.playerUnits.size(); i++ )
		{
			if ( idx == g.tMap.getInt( g.playerUnits[i]->getPosition() ) && !g.playerUnits[i]->isRescued() )
			{
				return g.playerUnits[i];
			}
		}
	}
	else if ( occ == OCCUPIED_ENEMY )
	{
		for ( unsigned i = 0; i < g.enemyUnits.size(); i++ )
		{
			if ( idx == g.tMap.getInt( g.enemyUnits[i]->getPosition() ) )
			{
				return g.enemyUnits[i];
			}
		}
	}
	return NULL;
}
Unit * GameState::getUnitActive( Point p )
{
	int idx = g.tMap.getInt( p );
	OccupiedStatus occ = g.occupied[idx];
	if ( occ == OCCUPIED_ALLY )
	{
		for ( unsigned i = 0; i < g.playerUnits.size(); i++ )
		{
			if ( idx == g.tMap.getInt( g.playerUnits[i]->getPosition() ) 
				&& !g.playerUnits[i]->isRescued() && g.playerUnits[i]->isActive() )
			{
				return g.playerUnits[i];
			}
		}
	}
	return NULL;
}
void GameState::moveUnit( Unit * unit, Point moveTo, bool isAlly )
{
	g.occupied[g.tMap.getInt( unit->getPosition() )] = NOT_OCCUPIED;
	if ( isAlly )
	{
		g.occupied[g.tMap.getInt( moveTo )] = OCCUPIED_ALLY;
	}
	else
	{
		g.occupied[g.tMap.getInt( moveTo )] = OCCUPIED_ENEMY;
	}
	unit->setPosition( moveTo );

	setMoveRanges();
}
void GameState::setMoveRanges()
{
	for ( unsigned i = 0; i < g.playerUnits.size(); i++ )
	{
		g.playerUnits[i]->calculateMovement( &g.tMap, g.occupied );
	}
	for ( unsigned i = 0; i < g.enemyUnits.size(); i++ )
	{
		g.enemyUnits[i]->calculateMovement( &g.tMap, g.occupied, OCCUPIED_ENEMY );
	}
}
void GameState::getWeaponTriangleBonuses( WeaponType attacker, WeaponType defender, int& atkBonus, int& avoBonus )
{
	atkBonus = 0;
	avoBonus = 0;

	// attacker good
	if ( ( ( attacker == WEAPON_FIST || attacker == WEAPON_KNIFE ) && ( defender == WEAPON_CLUB || defender == WEAPON_AXE ) )
		|| ( attacker == WEAPON_CLUB || attacker == WEAPON_AXE ) && ( defender == WEAPON_SWORD || defender == WEAPON_LANCE )
		|| ( attacker == WEAPON_SWORD || attacker == WEAPON_LANCE ) && ( defender == WEAPON_FIST || defender == WEAPON_KNIFE )
		|| ( attacker == WEAPON_HANDGUN && defender == WEAPON_SHOTGUN )
		|| ( attacker == WEAPON_SHOTGUN && defender == WEAPON_RIFLE )
		|| ( attacker == WEAPON_RIFLE && defender == WEAPON_HANDGUN ) )
	{
		atkBonus = WEAPON_TRIANGLE_ATK_BONUS;
		avoBonus = WEAPON_TRIANGLE_AVO_BONUS;
	}
	// attacker bad
	else if ( ( attacker == WEAPON_FIST || attacker == WEAPON_KNIFE ) && ( defender == WEAPON_SWORD || defender == WEAPON_LANCE )
		|| ( attacker == WEAPON_SWORD || attacker == WEAPON_LANCE ) && ( defender == WEAPON_CLUB || defender == WEAPON_AXE )
		|| ( attacker == WEAPON_CLUB || attacker == WEAPON_AXE ) && ( defender == WEAPON_FIST || defender == WEAPON_KNIFE )
		|| ( attacker == WEAPON_HANDGUN && defender == WEAPON_RIFLE )
		|| ( attacker == WEAPON_SHOTGUN && defender == WEAPON_HANDGUN )
		|| ( attacker == WEAPON_RIFLE && defender == WEAPON_SHOTGUN ) )
	{
		atkBonus = -WEAPON_TRIANGLE_ATK_BONUS;
		avoBonus = -WEAPON_TRIANGLE_AVO_BONUS;
	}
}
void GameState::getAttackStats( Unit * attacker, Unit * defender, const Weapon * attackerWeapon, const Weapon * defenderWeapon,
	Tile * terrain, int& damage, int& hit, int& crit )
{
	int wepTriangleHit, wepTriangleDmg;
	getWeaponTriangleBonuses( attackerWeapon->weaponType,
		defenderWeapon ? defenderWeapon->weaponType : WEAPON_NULL,
		wepTriangleDmg, wepTriangleHit );


	hit = (int)fmin( 100, fmax( 0, attacker->getHit() + wepTriangleHit - ( defender->getAvo() + terrain->getAvo() ) ) );

	damage = (int)fmax( 0, ( attacker->getStrength()
		+ ( ( attackerWeapon->might + wepTriangleDmg ) *
			( attackerWeapon->effectiveAgainst & defender->getClassType() ? EFFECTIVE_WEAPON_MULTIPLIER : 1 ) ) )
		- ( defender->getDefense() + terrain->getDef() ) );
	crit = (int)fmin( 100, fmax( 0, attacker->getSkill() + attackerWeapon->crit - defender->getLuck() ) );
}
void GameState::getAttackStats( Unit * attacker, Unit * defender, int& damage, int& hit, int& crit )
{
	getAttackStats( attacker, defender, attacker->getWeapon(), defender->getWeapon(),
		g.tMap.getTile( defender->getPosition() ), damage, hit, crit );
}
int GameState::getWeaponTriangleEffectiveness( WeaponType attacker, WeaponType defender )
{
	// attacker good
	if ( ( ( attacker == WEAPON_FIST || attacker == WEAPON_KNIFE ) && ( defender == WEAPON_CLUB || defender == WEAPON_AXE ) )
		|| ( attacker == WEAPON_CLUB || attacker == WEAPON_AXE ) && ( defender == WEAPON_SWORD || defender == WEAPON_LANCE )
		|| ( attacker == WEAPON_SWORD || attacker == WEAPON_LANCE ) && ( defender == WEAPON_FIST || defender == WEAPON_KNIFE )
		|| ( attacker == WEAPON_HANDGUN && defender == WEAPON_SHOTGUN )
		|| ( attacker == WEAPON_SHOTGUN && defender == WEAPON_RIFLE )
		|| ( attacker == WEAPON_RIFLE && defender == WEAPON_HANDGUN ) )
	{
		return 1;
	}
	// attacker bad
	else if ( ( attacker == WEAPON_FIST || attacker == WEAPON_KNIFE ) && ( defender == WEAPON_SWORD || defender == WEAPON_LANCE )
		|| ( attacker == WEAPON_SWORD || attacker == WEAPON_LANCE ) && ( defender == WEAPON_CLUB || defender == WEAPON_AXE )
		|| ( attacker == WEAPON_CLUB || attacker == WEAPON_AXE ) && ( defender == WEAPON_FIST || defender == WEAPON_KNIFE )
		|| ( attacker == WEAPON_HANDGUN && defender == WEAPON_RIFLE )
		|| ( attacker == WEAPON_SHOTGUN && defender == WEAPON_HANDGUN )
		|| ( attacker == WEAPON_RIFLE && defender == WEAPON_SHOTGUN ) )
	{
		return -1;
	}
	return 0;
}
void GameState::centerViewport( Point positionInGrid )
{
	Point newPos;
	newPos.x = ( positionInGrid.x * g.tMap.getTileWidth() ) + ( g.tMap.getTileWidth() / 2 );
	newPos.y = ( positionInGrid.y * g.tMap.getTileHeight() ) + ( g.tMap.getTileHeight() / 2 );
	
	g.viewport.center( newPos );
	g.mouse.setPositionWorldspace( newPos );
}
int GameState::getRN()
{
	return (int)( rand() % 100 );
}
int GameState::getDoubleRN()
{
	return (int)( ( ( rand() % 100 ) + ( rand() % 100 ) ) / 2 );
}
void GameState::removeUnit( int index, bool isAlly )
{
	if ( isAlly )
	{
		g.occupied[g.tMap.getInt( g.playerUnits[index]->getPosition() )] = NOT_OCCUPIED;
		delete g.playerUnits[index];
		g.playerUnits.erase( g.playerUnits.begin() + index );
		setEnemyRanges();
	}
	else
	{
		g.occupied[g.tMap.getInt( g.enemyUnits[index]->getPosition() )] = NOT_OCCUPIED;
		delete g.enemyUnits[index];
		g.enemyUnits.erase( g.enemyUnits.begin() + index );
		setAllyRanges();
	}
}
void GameState::setAllyRanges()
{
	for ( unsigned i = 0; i < g.playerUnits.size(); i++ )
	{
		g.playerUnits[i]->calculateMovement( &g.tMap, g.occupied, OCCUPIED_ALLY );
	}
}
void GameState::setEnemyRanges()
{
	for ( unsigned i = 0; i < g.enemyUnits.size(); i++ )
	{
		g.enemyUnits[i]->calculateMovement( &g.tMap, g.occupied, OCCUPIED_ENEMY );
	}
}
bool GameState::pointIsInRange( Point p, const std::vector<Point> & vec )
{
	for ( unsigned i = 0; i < vec.size(); i++ )
	{
		if ( p == vec[i] ) return true;
	}
	return false;
}