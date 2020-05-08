#include "GameStateOptionsMenu.h"
#include "GameStateMap.h"

GameStateOptionsMenu::GameStateOptionsMenu( GameData & gameData, CommonSprites& commonSprites )
	:
	GameState( gameData, commonSprites )
{
	itemHeight = 50;

	rect.x = 400;
	rect.y = 200;
	rect.width = 400;
	rect.height = 400;
}
GameStateOptionsMenu::~GameStateOptionsMenu()
{
	// ~~~
}

void GameStateOptionsMenu::update()
{
	if ( g.mouse.getButtonRelease( RIGHT_CLICK ) )
	{
		updateCursorPosition();
		g.gameState = new GameStateMap( g, s );
		delete this;
		return;
	}

	else
	{
		Point mousePos = g.mouse.getPositionWindowspace();

		menuIndex = (int)( mousePos.y - rect.y ) / itemHeight;

		if ( rect.isColliding( mousePos, 0, 0 ) )
		{
			if ( menuIndex >= N_MENU_OPTIONS )
			{
				menuIndex = -1;
			}
		}
		else // out of bounds
		{
			menuIndex = -1;
		}

		if ( g.mouse.getButtonClick( LEFT_CLICK ) && menuIndex >= 0 )
		{
			leftClickIndex = menuIndex;
		}
		else if ( g.mouse.getButtonRelease( LEFT_CLICK ) && menuIndex >= 0 && menuIndex == leftClickIndex )
		{
			switch ( (MenuOption)menuIndex )
			{
			case OPTION_END_TURN:
				g.shouldEndTurn = true;
				updateCursorPosition();
				g.gameState = new GameStateMap( g, s );
				delete this;
				return;
				break;
			case OPTION_RETURN:
				updateCursorPosition();
				g.gameState = new GameStateMap( g, s );
				delete this;
				return;
				break;
			}
		}
	}
}
void GameStateOptionsMenu::drawToScreen()
{
	g.gfx.draw( &s.background, 0, 0 );
	drawEnemyRange();
	drawCharactersOnTilemap();

	drawMenu();

	drawMouse();
}


void GameStateOptionsMenu::drawMenu()
{
	int highlightOffset = 16;

	RectD highlight;
	

	highlight.x = rect.x;
	highlight.width = rect.width;
	highlight.y = rect.y + ( menuIndex * itemHeight ) + highlightOffset;
	highlight.height = itemHeight;

	g.gfx.drawFixed( rect, 208, 191, 157, 240 );
	if ( menuIndex >= 0 )
	{
		g.gfx.drawFixed( highlight, 255, 255, 255, 100 );
	}

	
	for ( int i = 0; i < N_MENU_OPTIONS; i++ )
	{
		
		g.gfx.drawFixed( getMenuOptionName( (MenuOption)i ), &s.menuFont, 
			(int)rect.x + highlightOffset, (int)rect.y + highlightOffset + ( itemHeight*i ),
			20, 20, 20 );
	}
}

std::string GameStateOptionsMenu::getMenuOptionName( GameStateOptionsMenu::MenuOption option )
{
	switch ( option )
	{
	case OPTION_END_TURN:
		return "End Turn";
	case OPTION_RETURN:
		return "Return";
	default:
		return "";
	}
}