// last updated 07/04/17
#pragma once
#include <SFML/Graphics.hpp>
#include "Extras.h"
#include "KeyboardBase.h"
#include "Viewport.h"

class MouseBase : public KeyboardBase
{
public:
	MouseBase();
	virtual ~MouseBase();

	void update() override;

	bool getMouseWheelUp() const;
	bool getMouseWheelDown() const;
	void setMouseWheelVert( bool isUp );

protected:
	Point pos;

	bool wheelUp;
	bool wheelDown;
};

class MouseInterface
{
public:
	MouseInterface( sf::RenderWindow & sfWindow, MouseBase & mBase, Viewport * view = NULL );
	~MouseInterface();

	bool getButton( MOUSE_BUTTON_TYPE type );
	bool getButtonClick( MOUSE_BUTTON_TYPE type );
	bool getButtonRelease( MOUSE_BUTTON_TYPE type );
	bool getMouseWheelUp() const;
	bool getMouseWheelDown() const;

	Point getPositionWorldspace();
	Point getPositionWindowspace();
	void setPositionWorldspace( Point position );
	void setPositionWindowspace( Point position );

	void update(); // Must be called at BEGINNING of each frame!
	void setNewViewport( Viewport * view );

private:
	sf::RenderWindow & window;
	MouseBase & base;
	Viewport * viewport;

	Point posWorld;
	Point posWindow;

	bool wheelUp;
	bool wheelDown;
	int scrollCounter;
	int scrollFrames;
};