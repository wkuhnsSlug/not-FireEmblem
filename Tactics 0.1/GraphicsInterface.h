// last edited 08/12/17
#pragma once
#include <SFML/Graphics.hpp>
#include "Sprite.h"
#include "Shader.h"
#include "Extras.h"
#include "Line.h"
#include "Viewport.h"
#include "Circle.h"
#include "Font.h"

#include "Tilemap.h"

class GraphicsInterface
{
public:
	GraphicsInterface( sf::RenderWindow &Window, Viewport * view );

	void setView( Viewport * view );

	void startFrame();
	void display();
	Sprite * getLastTexture(); // returns new sprite
	void clear( const unsigned r, const unsigned g, const unsigned b );
	void draw( Sprite * sprite, const float x, const float y, 
		const unsigned r = 255, const unsigned g = 255, const unsigned b = 255, const unsigned a = 255, 
		bool isMirror = false );
	void draw( Sprite * sprite, const Point pos, 
		const unsigned r = 255, const unsigned g = 255, const unsigned b = 255, const unsigned a = 255, 
		bool isMirror = false );
	void draw( const Line l, 
		const unsigned r = 255, const unsigned g = 255, const unsigned b = 255, const unsigned a = 255, float thickness = 1 );
	void draw( const RectD & rect,
		const unsigned r = 255, const unsigned g = 255, const unsigned b = 255, const unsigned a = 255, const bool isHollow = false );
	
	void drawFixed( std::string text, Font * font, const int xPos, const int yPos, 
		const unsigned r = 255, const unsigned g = 255, const unsigned b = 255, const unsigned a = 255 );


	void drawFixed( Sprite * sprite, const float x, const float y, 
		const unsigned r = 255, const unsigned g = 255, const unsigned b = 255, const unsigned a = 255,
		bool isMirror = false );
	void drawFixed( Sprite * sprite, const Point pos,
		const unsigned r = 255, const unsigned g = 255, const unsigned b = 255, const unsigned a = 255,
		bool isMirror = false );
	void drawFixed( const RectD & rect,
		const unsigned r = 255, const unsigned g = 255, const unsigned b = 255, const unsigned a = 255, const bool isHollow = false );

	/******************************************************************************************************/

	void draw( Sprite*sprite, Point position, Tilemap*tMap );

	/******************************************************************************************************/

	void debugDraw( const CircleD * circle, 
		const unsigned r = 255, const unsigned g = 255, const unsigned b = 255, const unsigned a = 255 );
	void debugDraw( const RectD & rect, 
		const unsigned r = 255, const unsigned g = 255, const unsigned b = 255, const unsigned a = 255, const bool isHollow = false );

private:
	sf::RenderWindow &renderWindow;
	sf::RenderTexture window;
	Viewport * viewport;
};