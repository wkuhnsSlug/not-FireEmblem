#include "GraphicsInterface.h"
#include "ShaderContainer.h"
#include "SFLine.h"
#include <cassert>

#define DEBUG_DRAW_ENABLED

GraphicsInterface::GraphicsInterface( sf::RenderWindow &Window, Viewport * view )
	:
	renderWindow(Window),
	viewport(view)
{
	assert( window.create( SCREENWIDTH, SCREENHEIGHT ) );
}

void GraphicsInterface::setView( Viewport * view )
{
	viewport = view;
}

void GraphicsInterface::startFrame()
{
	window.setView( *( viewport->getSFView() ) );
}
void GraphicsInterface::display()
{
	//window.display();
	// get the target texture (where the stuff has been drawn)
	const sf::Texture& texture = window.getTexture();

	// draw it to the window
	sf::Sprite sprite( texture );
	// sprite's upside-down lol
	sprite.setScale( 1.0f, -1.0f );
	sprite.setPosition( 0.0f, SCREENHEIGHT );
	renderWindow.draw( sprite );
	renderWindow.display();
}
void GraphicsInterface::clear( const unsigned r, const unsigned g, const unsigned b )
{
	window.clear( sf::Color( r, g, b, 255 ) );
}
Sprite * GraphicsInterface::getLastTexture() // returns new sprite
{
	const sf::Texture& texture = window.getTexture();
	sf::Image img( texture.copyToImage() );
	img.flipVertically();
	sf::Texture * tex = new sf::Texture;
	tex->loadFromImage( img );
	Sprite * spr = new Sprite( tex );
	return spr;
}


void GraphicsInterface::draw( Sprite * sprite, const float x, const  float y,
	const unsigned r, const unsigned g, const unsigned b, const unsigned a, 
	bool isMirror )
{
	float xPos, yPos;
	sf::Sprite * sfs = sprite->getSFSprite();
	sfs->setColor( sf::Color( r, g, b, a ) );
	Shader* shader = sprite->getShader();
	xPos = x + sprite->getOffsetX();
	yPos = y + sprite->getOffsetY();

	if ( isMirror )
	{
		sfs->setScale( -1.0f, 1.0f );
		xPos += sprite->getWidth();
	}
	sfs->setPosition( xPos, yPos );

	if ( shader )
	{
		sf::Shader * s = ShaderContainer::getShader( shader->getName() );
		if ( s )
		{
			shader->setParams( s );
			window.draw( *sfs, s );
		}
		//sf::Shader s = sprite->getShader()->getName()
		//window.draw( *sfs, sprite->getShader()->getSFShader() );
	}
	else
	{
		window.draw( *sfs );
	}

	if ( isMirror )
	{
		// reset scale
		sfs->setScale( 1.0f, 1.0f );
	}
}
void GraphicsInterface::draw( Sprite * sprite, const Point pos,
	const unsigned r, const unsigned g, const unsigned b, const unsigned a, 
	bool isMirror )
{
	draw( sprite, (float)pos.x, (float)pos.y, r, g, b, a, isMirror );
}
void GraphicsInterface::draw( const Line l,
	const unsigned r, const unsigned g, const unsigned b, const unsigned a, float thickness )
{
	if ( thickness == 1 ) // normal line
	{
		sf::Vertex line[] =
		{
			sf::Vertex( sf::Vector2f( (float)l.getStartX(), (float)l.getStartY() ), sf::Color( r, g, b, a ) ),
			sf::Vertex( sf::Vector2f( (float)l.getEndX(), (float)l.getEndY() ), sf::Color( r, g, b, a ) )
		};

		window.draw( line, 2, sf::Lines );
	}
	else // use sfLine
	{
		Line l2( l );
		l2.changeMagnitude( l2.getMagnitude() + ( thickness / 2.0 ) );
		Line l3( l );
		l3.changeMagnitude( thickness / 2.0 );
		VectorD vec = VectorD::rotateBy( M_PI, l3.getVector() );
		l3.changeVector( vec );
		l2.changeStart( l3.getEndX(), l3.getEndY() );
		sfLine line( sf::Vector2f( (float)l2.getStartX(), (float)l2.getStartY() ),
			sf::Vector2f( (float)l2.getEndX(), (float)l2.getEndY() ), sf::Color( r, g, b, a ), thickness );
		window.draw( line );
	}
}
void GraphicsInterface::draw( const RectD & rect,
	const unsigned r, const unsigned g, const unsigned b, const unsigned a,
	const bool isHollow )
{
	sf::RectangleShape rectangle( sf::Vector2f( (float)rect.width, (float)rect.height ) );
	if ( isHollow )
	{
		rectangle.setOutlineColor( sf::Color( r, g, b, a ) );
		rectangle.setOutlineThickness( -2 );
		rectangle.setFillColor( sf::Color( 0, 0, 0, 0 ) );
	}
	else
	{
		rectangle.setFillColor( sf::Color( r, g, b, a ) );
	}
	rectangle.setPosition( (float)rect.x, (float)rect.y );
	window.draw( rectangle );
}

void GraphicsInterface::drawFixed( std::string text, Font * font, const int xPos, const int yPos, 
	const unsigned r, const unsigned g, const unsigned b, const unsigned a )
{
	window.setView( window.getDefaultView() ); // temporarily change view to screenspace
	sf::Text * t = font->getSFText();
	sf::Vector2f vec = window.mapPixelToCoords( { xPos, yPos } );
	t->setPosition( vec.x, vec.y );
	t->setFillColor( sf::Color( r, g, b, a ) );
	t->setString( text );
	window.draw( *t );
	window.setView( *( viewport->getSFView() ) ); // and change it back
}
//void GraphicsInterface::drawFixed( Sprite * sprite, const float x, const float y )
//{
//	window.setView( window.getDefaultView() ); // temporarily change view to screenspace
//	sf::Sprite * sfs = sprite->getSFSprite();
//	sfs->setPosition( x, y );
//	window.draw( *sfs );
//	window.setView( *( viewport->getSFView() ) ); // and change it back
//}

void GraphicsInterface::drawFixed( Sprite * sprite, const float x, const float y,
	const unsigned r, const unsigned g, const unsigned b, const unsigned a, bool isMirror )
{
	window.setView( window.getDefaultView() ); // temporarily change view to screenspace
	draw( sprite, x, y, r, g, b, a, isMirror );
	window.setView( *( viewport->getSFView() ) ); // and change it back
}
void GraphicsInterface::drawFixed( Sprite * sprite, const Point pos,
	const unsigned r, const unsigned g, const unsigned b, const unsigned a, bool isMirror )
{
	drawFixed( sprite, (float)pos.x, (float)pos.y, r, g, b, a, isMirror );
}
void GraphicsInterface::drawFixed( const RectD & rect,
	const unsigned r, const unsigned g, const unsigned b, const unsigned a, const bool isHollow )
{
	window.setView( window.getDefaultView() ); // temporarily change view to screenspace
	sf::RectangleShape rectangle( sf::Vector2f( (float)rect.width, (float)rect.height ) );
	if ( isHollow )
	{
		rectangle.setOutlineColor( sf::Color( r, g, b, a ) );
		rectangle.setOutlineThickness( -2 );
		rectangle.setFillColor( sf::Color( 0, 0, 0, 0 ) );
	}
	else
	{
		rectangle.setFillColor( sf::Color( r, g, b, a ) );
	}
	rectangle.setPosition( (float)rect.x, (float)rect.y );
	window.draw( rectangle );
	window.setView( *( viewport->getSFView() ) ); // and change it back
}

void GraphicsInterface::debugDraw( const CircleD * circle,
	const unsigned r, const unsigned g, const unsigned b, const unsigned a )
{
#ifdef DEBUG_DRAW_ENABLED
	float rad = (float)circle->getRadius();
	sf::CircleShape shape( rad );
	shape.setFillColor( sf::Color( r, g, b, a ) );
	shape.setOrigin( rad, rad );
	shape.setPosition( (float)circle->getPosition().x, (float)circle->getPosition().y );
	window.draw( shape );
#endif
}

void GraphicsInterface::debugDraw( const RectD & rect, 
	const unsigned r , const unsigned g , const unsigned b , const unsigned a , 
	const bool isHollow )
{
#ifdef DEBUG_DRAW_ENABLED
	draw( rect, r, g, b, a, isHollow );
#endif
}

/******************************************************************************************************/

void GraphicsInterface::draw( Sprite * sprite, Point position, Tilemap * tMap )
{
	draw( sprite, (float)position.x * tMap->getTileWidth(), (float)position.y * tMap->getTileHeight() );
}

/******************************************************************************************************/