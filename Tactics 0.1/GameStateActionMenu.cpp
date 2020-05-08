#include "GameStateActionMenu.h"
#include "GameStateMap.h"
#include "GameStateSelectAttack.h"
#include "GameStateTrade.h"
#include "GameStateSelectHeal.h"
#include "Potion.h"

GameStateActionMenu::GameStateActionMenu( GameData & gameData, CommonSprites& commonSprites, 
	Point movePosition, Point cursorPosition, bool canUndo, bool isCantoMove )
	:
	GameState( gameData, commonSprites )
{
	amState = AM_NORMAL;
	canReturn = canUndo;
	menuPosition = cursorPosition; // g.mouse.getPositionWindowspace();
	movePos = movePosition;
	currentMenuElement = 0;
	g.playerUnits[g.selectedUnit]->calculateAvailableActions( movePos, g.enemyUnits, g.playerUnits, &g.tMap, g.occupied );
	actionMenu = ( *g.playerUnits[g.selectedUnit]->getAvailableActions() )[0];
	numMenuElements = ( g.playerUnits[g.selectedUnit]->getAvailableActions() )->size();
}

GameStateActionMenu::~GameStateActionMenu()
{
	// ~~~
}


void GameStateActionMenu::update()
{
	updateView();
	actionMenu = ( *g.playerUnits[g.selectedUnit]->getAvailableActions() )[currentMenuElement];
	updateCursorPosition();

	menuPosition = g.mouse.getPositionWindowspace();

	switch ( amState )
	{
	case AM_NORMAL:
		if ( g.mouse.getButtonRelease( RIGHT_CLICK ) )
		{
			if ( canReturn )
			{
				g.gameState = new GameStateMap( g, s );
				delete this;
				return;
			}
		}
		else if ( g.mouse.getMouseWheelUp() )
		{
			currentMenuElement = ( currentMenuElement - 1 + numMenuElements ) % numMenuElements;
		}
		else if ( g.mouse.getMouseWheelDown() )
		{
			currentMenuElement = ( currentMenuElement + 1 ) % numMenuElements;
		}
		else if ( g.mouse.getButtonRelease( LEFT_CLICK ) )
		{
			switch ( actionMenu )
			{
			case ACTION_WAIT:
				moveUnit( g.playerUnits[g.selectedUnit], movePos, true );
				g.playerUnits[g.selectedUnit]->calculateMovement( &g.tMap, g.occupied );
				g.playerUnits[g.selectedUnit]->setActive( false );
				g.selectedUnit = -1;

				g.gameState = new GameStateMap( g, s );
				delete this;
				return;

				break;
			case ACTION_ATTACK:
				g.gameState = new GameStateSelectAttack( g, s, movePos, menuPosition );
				delete this;
				return;

				break;
			case ACTION_HEAL:
				g.gameState = new GameStateSelectHeal( g, s, movePos, menuPosition );
				delete this;
				return;

				break;
			case ACTION_ITEM:
				amState = AM_ITEM;
				subMenuElement = 0;
				numSubMenuElements = g.playerUnits[g.selectedUnit]->getInventorySize();
				break;
			case ACTION_TRADE:
				amState = AM_TRADE;
				break;
			case ACTION_RESCUE:
				amState = AM_RESCUE;
				break;
			case ACTION_DROP:
				amState = AM_DROP;
				break;
			case ACTION_TRANSFER:
				amState = AM_TRANSFER;
				break;
			}
		}
		break;
	case AM_ITEM:
		if ( g.mouse.getButtonRelease( RIGHT_CLICK ) )
		{
			amState = AM_NORMAL;
		}
		else if ( g.mouse.getMouseWheelUp() )
		{
			subMenuElement = ( subMenuElement - 1 + numSubMenuElements ) % numSubMenuElements;
		}
		else if ( g.mouse.getMouseWheelDown() )
		{
			subMenuElement = ( subMenuElement + 1 ) % numSubMenuElements;
		}
		else if ( g.mouse.getButtonRelease( LEFT_CLICK ) )
		{
			Item * item = (g.playerUnits[g.selectedUnit]->getInventory() )[subMenuElement];
			Unit * player = g.playerUnits[g.selectedUnit];
			switch ( item->itemType )
			{
			case ITEM_WEAPON:
				player->equipWeapon( subMenuElement );
				break;
			case ITEM_HEAL_SELF:
			{
				if ( player->getCurrentHP() < player->getMaxHP() )
				{
					int newHP = player->getCurrentHP() + ( (Potion*)item )->recoveryAmount;
					if ( newHP > player->getMaxHP() ) newHP = player->getMaxHP();
					player->setCurrentHP( newHP );

					moveUnit( g.playerUnits[g.selectedUnit], movePos, true );
					g.playerUnits[g.selectedUnit]->calculateMovement( &g.tMap, g.occupied );
					g.playerUnits[g.selectedUnit]->setActive( false );
					g.selectedUnit = -1;

					g.gameState = new GameStateMap( g, s );
					delete this;
					return;
				}
			}
				break;
			}
		}
		break;
	case AM_TRADE:
		if ( g.mouse.getButtonRelease( RIGHT_CLICK ) )
		{
			amState = AM_NORMAL;
		}
		else if ( g.mouse.getButtonRelease( LEFT_CLICK ) )
		{
			int selUnitIndex;
			int curpos = g.tMap.getInt( g.cursorpos );
			bool wasFound = false;

			if ( g.occupied[curpos] == OCCUPIED_ALLY )
			{
				std::vector<Unit*> *tradePartners = g.playerUnits[g.selectedUnit]->getTradePartners();
				for ( unsigned i = 0; i < tradePartners->size(); i++ )
				{
					if ( Tilemap::gridDistance( g.cursorpos, ( *tradePartners )[i]->getPosition() ) == 0 )
					{
						selUnitIndex = i;
						wasFound = true;
						break;
					}
				}
			}
			else if ( curpos == g.tMap.getInt( movePos ) )
			{
				if ( g.cur()->getRescuedUnit() )
				{
					selUnitIndex = g.cur()->getTradePartners()->size() - 1; // it's always the last unit
					wasFound = true;
				}
			}

			if ( wasFound )
			{
				// do trade
				g.gameState = new GameStateTrade( g, s, movePos, menuPosition, selUnitIndex );
				delete this;
				return;
			}
		}
		break;
	case AM_RESCUE:
		if ( g.mouse.getButtonRelease( RIGHT_CLICK ) )
		{
			amState = AM_NORMAL;
		}
		else if ( g.mouse.getButtonRelease( LEFT_CLICK ) )
		{
			Unit * rescuedUnit;
			int curpos = g.tMap.getInt( g.cursorpos );

			if ( g.occupied[curpos] == OCCUPIED_ALLY )
			{
				// find unit
				rescuedUnit = getUnit( g.cursorpos );

				// then check if he's in range
				bool wasFound = false;
				std::vector<Point> * eir = g.playerUnits[g.selectedUnit]->getRescuableAllies();
				for ( unsigned i = 0; i < eir->size(); i++ )
				{
					if ( Tilemap::gridDistance( g.cursorpos, ( *eir )[i] ) == 0 )
					{
						wasFound = true;
						break;
					}
				}
				if ( !wasFound )
				{
					rescuedUnit = NULL;
				}
			}
			else
			{
				rescuedUnit = NULL;
			}

			if ( rescuedUnit )
			{
				// do rescue
				Unit * selUnit = g.playerUnits[g.selectedUnit];
				selUnit->setRescuedUnit( rescuedUnit );
				rescuedUnit->setRescued( true );
				g.occupied[g.tMap.getInt( rescuedUnit->getPosition() )] = NOT_OCCUPIED;
				rescuedUnit->setPosition( movePos );

				// and move unit
				moveUnit( selUnit, movePos, true );
				selUnit->calculateMovement( &g.tMap, g.occupied );
				g.playerUnits[g.selectedUnit]->setActive( false );
				g.selectedUnit = -1;

				g.gameState = new GameStateMap( g, s );
				delete this;
				return;
			}
		}
		break;
	case AM_DROP:
		if ( g.mouse.getButtonRelease( RIGHT_CLICK ) )
		{
			amState = AM_NORMAL;
		}
		else if ( g.mouse.getButtonRelease( LEFT_CLICK ) )
		{
			Unit * selUnit = g.playerUnits[g.selectedUnit];
			int curpos = g.tMap.getInt( g.cursorpos );



			if ( g.occupied[curpos] != OCCUPIED_ENEMY )
			{
				// check if in range
				bool wasFound = false;
				std::vector<Point> * eir = g.playerUnits[g.selectedUnit]->getDropRange();
				for ( unsigned i = 0; i < eir->size(); i++ )
				{
					if ( Tilemap::gridDistance( g.cursorpos, ( *eir )[i] ) == 0 )
					{
						wasFound = true;
						break;
					}
				}
				if ( wasFound )
				{
					// set the carried unit free
					selUnit->getRescuedUnit()->setRescued( false );
					selUnit->getRescuedUnit()->setActive( false );
					moveUnit( selUnit->getRescuedUnit(), g.cursorpos, true );
					selUnit->setRescuedUnit( NULL );

					// and move selected unit
					moveUnit( selUnit, movePos, true );
					selUnit->calculateMovement( &g.tMap, g.occupied );
					g.playerUnits[g.selectedUnit]->setActive( false );
					g.selectedUnit = -1;

					g.gameState = new GameStateMap( g, s );
					delete this;
					return;
				}
			}

			
		}
		break;
	case AM_TRANSFER:
		if ( g.mouse.getButtonRelease( RIGHT_CLICK ) )
		{
			amState = AM_NORMAL;
		}
		else if ( g.mouse.getButtonRelease( LEFT_CLICK ) )
		{
			Unit * foundUnit;
			int curpos = g.tMap.getInt( g.cursorpos );

			if ( g.occupied[curpos] == OCCUPIED_ALLY )
			{
				// find unit
				foundUnit = getUnit( g.cursorpos );

				// then check if he's in range
				bool wasFound = false;
				std::vector<Point> * eir = g.playerUnits[g.selectedUnit]->getTransferableAllies();
				for ( unsigned i = 0; i < eir->size(); i++ )
				{
					if ( Tilemap::gridDistance( g.cursorpos, ( *eir )[i] ) == 0 )
					{
						wasFound = true;
						break;
					}
				}
				if ( !wasFound )
				{
					foundUnit = NULL;
				}
			}
			else
			{
				foundUnit = NULL;
			}

			if ( foundUnit )
			{
				// do transfer
				Unit * selUnit = g.playerUnits[g.selectedUnit];
				Unit * temp = foundUnit->getRescuedUnit();
				foundUnit->setRescuedUnit( selUnit->getRescuedUnit() );
				if ( foundUnit->getRescuedUnit() )
					foundUnit->getRescuedUnit()->setPosition( foundUnit->getPosition() );
				selUnit->setRescuedUnit( temp );
				if ( selUnit->getRescuedUnit() )
					selUnit->getRescuedUnit()->setPosition( movePos );

				// and move unit
				selUnit->setTransfered();
				//moveUnit( selUnit, movePos, true );
				//selUnit->calculateMovement( &g.tMap, g.occupied );
				//selUnit->setActive( false );
				//g.selectedUnit = -1;

				g.gameState = new GameStateActionMenu( g, s, movePos, menuPosition, false );
				delete this;
				return;
				
			}
		}
		break;
	}
}
void GameStateActionMenu::drawToScreen()
{
	g.gfx.draw( &s.background, 0, 0 );

	drawEnemyRange();
	if ( amState == AM_NORMAL || amState == AM_ITEM )
	{
		if ( canReturn )
		{
			drawHighlights( g.playerUnits[g.selectedUnit]->getHealRange(), &s.greenHighlight );
			drawHighlights( g.playerUnits[g.selectedUnit]->getAttackRange(), &s.redHighlight );
			drawHighlights( g.playerUnits[g.selectedUnit]->getMovement(), &s.blueHighlight );
			g.gfx.draw( &s.cursorRed, (float)( g.playerUnits[g.selectedUnit]->getPosition().x * g.tMap.getTileWidth() ),
				(float)( g.playerUnits[g.selectedUnit]->getPosition().y * g.tMap.getTileHeight() ) );
		}
		else
		{
			drawHighlights( g.playerUnits[g.selectedUnit]->getEnemiesInRange(), &s.redHighlight );
		}

		g.gfx.draw( &s.cursorBlue, (float)( movePos.x * g.tMap.getTileWidth() ),
			(float)( movePos.y * g.tMap.getTileHeight() ) );
	}
	else if ( amState == AM_TRADE )
	{
		drawHighlights( g.playerUnits[g.selectedUnit]->getTradablePositions(), &s.blueHighlight );
		drawCursor();
	}
	else if ( amState == AM_RESCUE )
	{
		drawHighlights( g.playerUnits[g.selectedUnit]->getRescuableAllies(), &s.blueHighlight );
		drawCursor();
	}
	else if ( amState == AM_DROP )
	{
		drawHighlights( g.playerUnits[g.selectedUnit]->getDropRange(), &s.blueHighlight );
		drawCursor();
	}
	else if ( amState == AM_TRANSFER )
	{
		drawHighlights( g.playerUnits[g.selectedUnit]->getTransferableAllies(), &s.blueHighlight );
		drawCursor();
	}

	drawCharactersOnTilemap();
	Unit * u = getUnit( g.cursorpos );
	if ( u )
	{
		drawUnitThumbnail( u );
	}
	drawTerrainInfo();
	switch ( amState )
	{
	case AM_NORMAL:
		drawContextMenu();
		break;
	case AM_ITEM:
		drawContextMenu();
		drawWeaponsList();
		break;
	case AM_TRADE:
		drawMouse();
		break;
	}

	drawMouse();
}

void GameStateActionMenu::drawSelectedUnit()
{
	g.gfx.draw( g.playerUnits[g.selectedUnit]->getSprite(), movePos, &g.tMap );
}
void GameStateActionMenu::drawContextMenu()
{
	// draw context menu
	int menuElementHeight = 40;

	RectD rect, highlight;
	rect.width = 150;
	rect.height = numMenuElements * menuElementHeight;
	rect.x = menuPosition.x;
	rect.y = menuPosition.y - rect.height;
	if ( menuPosition.y < rect.height )
	{
		rect.y += rect.height;
	}
	if ( menuPosition.x > SCREENWIDTH - rect.width )
	{
		rect.x -= rect.width;
	}
	highlight.x = rect.x;
	highlight.width = rect.width;
	highlight.y = rect.y + ( currentMenuElement * menuElementHeight );
	highlight.height = menuElementHeight;

	g.gfx.drawFixed( rect, 208, 191, 157, 240 );
	g.gfx.drawFixed( highlight, 255, 255, 255, 100 );

	for ( int i = 0; i < numMenuElements; i++ )
	{
		std::string str;
		switch ( ( *g.playerUnits[g.selectedUnit]->getAvailableActions() )[i] )
		{
		case ACTION_ATTACK:
			str = "Attack";
			break;
		case ACTION_HEAL:
			str = "Heal";
			break;
		case ACTION_TRADE:
			str = "Trade";
			break;
		case ACTION_RESCUE:
			str = "Rescue";
			break;
		case ACTION_DROP:
			str = "Drop";
			break;
		case ACTION_TRANSFER:
			str = "Transfer";
			break;
		case ACTION_ITEM:
			str = "Item";
			break;
		case ACTION_CONVOY:
			str = "Convoy";
			break;
		case ACTION_WAIT:
			str = "Wait";
			break;
		}
		g.gfx.drawFixed( str, &s.menuFont, (int)rect.x + 8, (int)rect.y + ( menuElementHeight*i ), 20, 20, 20 );
	}
}
void GameStateActionMenu::drawWeaponsList()
{
	int menuElementHeight = 40;

	RectD rect, highlight;
	rect.width = 310;
	rect.height = numSubMenuElements * menuElementHeight;
	rect.x = menuPosition.x + 160;
	rect.y = menuPosition.y - rect.height;
	if ( menuPosition.y < rect.height )
	{
		rect.y += rect.height;
	}
	if ( menuPosition.x > SCREENWIDTH - rect.width )
	{
		rect.x -= rect.width;
	}
	highlight.x = rect.x;
	highlight.width = rect.width;
	highlight.y = rect.y + ( subMenuElement * menuElementHeight );
	highlight.height = menuElementHeight;

	g.gfx.drawFixed( rect, 208, 191, 157, 240 );
	g.gfx.drawFixed( highlight, 255, 255, 255, 100 );

	for ( int i = 0; i < numSubMenuElements; i++ )
	{
		Item * item = g.playerUnits[g.selectedUnit]->getInventory()[i];
		std::string str = item->name;
		if ( i == subMenuElement )
		{
			switch ( item->itemType )
			{
			case ITEM_WEAPON:
				str += "  -> Equip";
				break;
			case ITEM_HEAL_SELF:
				if ( g.playerUnits[g.selectedUnit]->getCurrentHP() < g.playerUnits[g.selectedUnit]->getMaxHP() )
				{
					str += "  -> Use";
				}
				break;
			}
		}
		g.gfx.drawFixed( str, &s.menuFont, (int)rect.x + 8, (int)rect.y + ( menuElementHeight*i ), 20, 20, 20 );
	}
}