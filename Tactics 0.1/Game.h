#pragma once

#include "SFML\Graphics.hpp"
#include "KeyboardBase.h"
#include "Mouse.h"
#include "GraphicsInterface.h"
#include "Viewport.h"
#include "Screen.h"

class Game
{
public:
	Game( sf::RenderWindow& window, KeyboardBase & Kbd, MouseBase & mBase );
	~Game();
	bool execute();

private:

	KeyboardBase &kbd;
	MouseInterface mouse;
	GraphicsInterface gfx;
	Viewport viewport;

	Screen * currentScreen;
};