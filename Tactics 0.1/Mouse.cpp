#include "Mouse.h"
#include <cassert>

MouseBase::MouseBase()
	: KeyboardBase(1)
{
	arrPressedThisFrame = (bool*)calloc( N_MOUSE_BUTTON_TYPES, sizeof( bool ) );
	arrReleasedThisFrame = (bool*)calloc( N_MOUSE_BUTTON_TYPES, sizeof( bool ) );
	arrCurrentKeyState = (bool*)calloc( N_MOUSE_BUTTON_TYPES, sizeof( bool ) );
	assert( arrPressedThisFrame && arrCurrentKeyState && arrReleasedThisFrame );

	wheelUp = false;
	wheelDown = false;
}

MouseBase::~MouseBase()
{

}


void MouseBase::update()
{
	memset( arrPressedThisFrame, 0, sizeof( bool ) * N_MOUSE_BUTTON_TYPES );
	memset( arrReleasedThisFrame, 0, sizeof( bool ) * N_MOUSE_BUTTON_TYPES );
	wheelUp = false;
	wheelDown = false;
}

bool MouseBase::getMouseWheelUp() const
{
	return wheelUp;
}
bool MouseBase::getMouseWheelDown() const
{
	return wheelDown;
}
void MouseBase::setMouseWheelVert( bool isUp )
{
	if ( isUp )
	{
		wheelUp = true;
	}
	else
	{
		wheelDown = true;
	}
}

//--------------------------------------------------------------------------------------


MouseInterface::MouseInterface( sf::RenderWindow & sfWindow, MouseBase & mBase, Viewport * view )
	: window( sfWindow ),
	base( mBase ),
	viewport( view )
{
	scrollCounter = 0;
	scrollFrames = 6;
}
MouseInterface::~MouseInterface()
{
	// ~~~~
}

bool MouseInterface::getButton( MOUSE_BUTTON_TYPE type )
{
	return base.getKey( (KEYTYPE)type );
}
bool MouseInterface::getButtonClick( MOUSE_BUTTON_TYPE type )
{
	return base.getKeyPressedThisFrame( (KEYTYPE)type );
}
bool MouseInterface::getButtonRelease( MOUSE_BUTTON_TYPE type )
{
	return base.getKeyReleasedThisFrame( (KEYTYPE)type );
}

Point MouseInterface::getPositionWorldspace()
{
	return posWorld;
}
Point MouseInterface::getPositionWindowspace()
{
	return posWindow;
}
void MouseInterface::setPositionWorldspace( Point position )
{
	if ( viewport )
	{
		sf::Vector2i vec = window.mapCoordsToPixel( 
			sf::Vector2f( (float)position.x, (float)position.y ), *viewport->getSFView() );
		sf::Mouse::setPosition( vec, window );
	}
}
void MouseInterface::setPositionWindowspace( Point position )
{
	sf::Vector2i vec;
	vec.x = (int)position.x;
	vec.y = (int)position.y;
	sf::Mouse::setPosition( vec, window );
}


bool MouseInterface::getMouseWheelUp() const
{
	return wheelUp;
}
bool MouseInterface::getMouseWheelDown() const
{
	return wheelDown;
}

void MouseInterface::update() // Must be called at BEGINNING of each frame!
{
	sf::Vector2i vec = sf::Mouse::getPosition( window );
	sf::Vector2f vec2;
	if ( viewport )
	{
		vec2 = window.mapPixelToCoords( vec, *viewport->getSFView() );
	}
	else
	{
		vec2 = window.mapPixelToCoords( vec );
	}
	posWindow.x = (double)vec.x;
	posWindow.y = (double)vec.y;
	posWorld.x = (double)vec2.x;
	posWorld.y = (double)vec2.y;


	if ( base.getMouseWheelUp() && scrollCounter > scrollFrames )
	{
		wheelUp = true;
		scrollCounter = 0;
	}
	else
	{
		wheelUp = false;
	}
	if ( base.getMouseWheelDown() && scrollCounter > scrollFrames )
	{
		wheelDown = true;
		scrollCounter = 0;
	}
	else
	{
		wheelDown = false;
	}
	scrollCounter++;
}
void MouseInterface::setNewViewport( Viewport * view )
{
	viewport = view;
}