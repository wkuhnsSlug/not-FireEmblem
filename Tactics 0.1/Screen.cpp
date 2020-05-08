#include "Screen.h"

Screen::Screen( KeyboardBase& Kbds, MouseInterface& m, GraphicsInterface& Gfx, Screen ** ptr )
	:
	kbd( Kbds ),
	mouse(m),
	gfx( Gfx ),
	levelPointer( ptr )
{
	// intentionally left blank
}
Screen::~Screen()
{
	// intentionally left blank
}