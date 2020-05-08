#include "Game.h"
#include "ScreenLevel.h"

Game::Game( sf::RenderWindow& window, KeyboardBase & Kbd, MouseBase & mBase )
	:
	kbd( Kbd ),
	gfx( window, &viewport ),
	mouse( window, mBase, &viewport )
{
	currentScreen = new ScreenLevel( "", kbd, mouse, gfx, &currentScreen );
}
Game::~Game()
{
	delete currentScreen;
}
bool Game::execute()
{
	mouse.update();

	currentScreen->logic();

	gfx.startFrame();
	currentScreen->render();
	gfx.display();

	return true;
}
