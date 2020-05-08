#include "GameStateTrade.h"
#include "GameStateActionMenu.h"

GameStateTrade::GameStateTrade( GameData & gameData, CommonSprites& commonSprites,
	Point movePosition, Point mousePosition, int tradePartnerIndex )
	:
	GameState( gameData, commonSprites )
{
	mousePos = mousePosition;
	movePos = movePosition;
	otherUnitIndex = tradePartnerIndex;
	otherUnit = ( *g.cur()->getTradePartners() )[otherUnitIndex];

	isDragging = false;
	wasTraded = false;
	isLeftHandMenu = false;
	itemHeight = 50;

	// set up rects
	leftRect.width = 300;
	leftRect.height = 282;
	leftRect.x = 260;
	leftRect.y = 180;

	rightRect.width = 300;
	rightRect.height = 282;
	rightRect.x = 680;
	rightRect.y = 180;
}

GameStateTrade::~GameStateTrade()
{
	g.playerUnits[g.selectedUnit]->calculateMovement( &g.tMap, g.occupied ); // recalculate attack range
}


void GameStateTrade::update()
{
	updateCursorPosition();
	int curpos = g.tMap.getInt( g.cursorpos );

	if ( g.mouse.getButtonRelease( RIGHT_CLICK ) )
	{
		g.gameState = new GameStateActionMenu( g, s, movePos, mousePos, !wasTraded );
		delete this;
		return;
	}
	else
	{
		Point mousePos = g.mouse.getPositionWindowspace();

		menuIndex = (int)( mousePos.y - leftRect.y ) / itemHeight;

		if ( leftRect.isColliding( mousePos, 0, 0 ) )
		{
			if ( menuIndex > g.playerUnits[g.selectedUnit]->getInventorySize() - ( ( isDragging && cameFromLeft ) || !isDragging ? 1 : 0 ) )
			{
				menuIndex = -1;
			}
			isLeftHandMenu = true;
		}
		else if ( rightRect.isColliding( mousePos, 0, 0 ) )
		{
			if ( menuIndex > otherUnit->getInventorySize() - ( ( isDragging && !cameFromLeft ) || !isDragging ? 1 : 0 ) )
			{
				menuIndex = -1;
			}
			isLeftHandMenu = false;
		}
		else // out of bounds
		{
			menuIndex = -1;
		}

		if ( g.mouse.getButtonClick( LEFT_CLICK ) && menuIndex >= 0 )
		{
			isDragging = true;
			cameFromLeft = isLeftHandMenu;
			originalIndex = menuIndex;
		}
		else if ( g.mouse.getButtonRelease( LEFT_CLICK ) && isDragging )
		{
			isDragging = false;
			if ( menuIndex >= 0 )
			{
				if ( cameFromLeft == isLeftHandMenu && menuIndex == originalIndex ) return; // no change
				if ( cameFromLeft != isLeftHandMenu ) wasTraded = true;

				Item ** lhsItems = g.playerUnits[g.selectedUnit]->getInventory();
				Item ** rhsItems = otherUnit->getInventory();

				if ( isLeftHandMenu )
				{
					if ( cameFromLeft ) // is left, came from left
					{
						Item * temp = lhsItems[menuIndex];
						lhsItems[menuIndex] = lhsItems[originalIndex];
						lhsItems[originalIndex] = temp;
					}
					else // is left, came from right
					{
						Item * temp = lhsItems[menuIndex];
						lhsItems[menuIndex] = rhsItems[originalIndex];
						rhsItems[originalIndex] = temp;
					}
				}
				else // is right
				{
					if ( cameFromLeft ) // is right, came from left
					{
						Item * temp = rhsItems[menuIndex];
						rhsItems[menuIndex] = lhsItems[originalIndex];
						lhsItems[originalIndex] = temp;
					}
					else // is right, came from right
					{
						Item * temp = rhsItems[menuIndex];
						rhsItems[menuIndex] = rhsItems[originalIndex];
						rhsItems[originalIndex] = temp;
					}
				}

				g.playerUnits[g.selectedUnit]->setTraded();
				g.playerUnits[g.selectedUnit]->consolidateInventory();
				otherUnit->consolidateInventory();
			}
		}
		else if ( !g.playerUnits[g.selectedUnit]->getHasTraded() && !isDragging )
		{
			int numElements = g.cur()->getTradePartners()->size();
			if ( g.mouse.getMouseWheelUp() )
			{
				otherUnitIndex = ( otherUnitIndex - 1 + numElements ) % numElements;
			}
			else if ( g.mouse.getMouseWheelDown() )
			{
				otherUnitIndex = ( otherUnitIndex + 1 ) % numElements;
			}
			otherUnit = ( *g.cur()->getTradePartners() )[otherUnitIndex];
		}
	}
	
}
void GameStateTrade::drawToScreen()
{
	g.gfx.draw( &s.background, 0, 0 );

	drawEnemyRange();
	if ( g.selectedUnit >= 0 )
	{
		drawHighlights( g.playerUnits[g.selectedUnit]->getEnemiesInRange(), &s.redHighlight );
	}
	drawCharactersOnTilemap();

	drawInventory( g.playerUnits[g.selectedUnit], leftRect, isLeftHandMenu ? menuIndex : -1 );
	drawInventory( otherUnit, rightRect, isLeftHandMenu ? -1 : menuIndex );
	
	drawMouse();
}

void GameStateTrade::drawSelectedUnit()
{
	g.gfx.draw( g.playerUnits[g.selectedUnit]->getSprite(), movePos, &g.tMap );
}
void GameStateTrade::drawInventory( Unit * unit, RectD & rect, int index )
{
	int highlightOffset = 16;

	RectD highlight;
	
	highlight.x = rect.x;
	highlight.width = rect.width;
	highlight.y = rect.y + ( index * itemHeight ) + highlightOffset;
	highlight.height = itemHeight;
	
	g.gfx.drawFixed( rect, 208, 191, 157, 240 );
	if ( index >= 0 )
	{
		g.gfx.drawFixed( highlight, 255, 255, 255, 100 );
	}

	g.gfx.drawFixed( unit->getName(), &s.menuFont, (int)rect.x + 40, (int)rect.y - 40, 10, 10, 10 );

	for ( int i = 0; i < unit->getInventorySize(); i++ )
	{
		unsigned colorAdd = unit->getInventory()[i]->itemType == ITEM_WEAPON
			&& !unit->canWield( (Weapon*)unit->getInventory()[i] ) ? 100 : 0;
		g.gfx.drawFixed( unit->getInventory()[i]->name, &s.menuFont, 
			(int)rect.x + highlightOffset, (int)rect.y + highlightOffset + ( itemHeight*i ),
			20 + colorAdd, 20 + colorAdd, 20 + colorAdd );
	}
}