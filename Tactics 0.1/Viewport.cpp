#include "Viewport.h"
#include <cassert>

Viewport::Viewport()
{
	isBounded = false;
}
Viewport::Viewport( RectF dimensions )
{
	init( dimensions );
}
Viewport::Viewport( float x, float y, float width, float height )
{
	init( x, y, width, height );
}


void Viewport::init( RectF dimensions )
{
	init( dimensions.x, dimensions.y, dimensions.width, dimensions.height );
}
void Viewport::init( float x, float y, float width, float height )
{
	pos.x = x;
	pos.y = y;
	pos.width = width;
	pos.height = height;

	startWidth = width;
	startHeight = height;
	currentZoom = 1;

	isBounded = false;
	view.reset( sf::FloatRect( pos.x, pos.y, pos.width, pos.height ) );
}
void Viewport::setBounds( float left, float top, float right, float bottom )
{
	//assert( fabs( left - right ) >= pos.width && fabs( top - bottom ) >= pos.height );
	bounds.x = left;
	bounds.width = right;
	bounds.y = top;
	bounds.height = bottom;

	maxWidth = bounds.width - bounds.x;
	maxHeight = bounds.height - bounds.y; // this may or may not need to be reversed

	if ( maxWidth / maxHeight > (float)startWidth / startHeight )
	{
		maxWidth = ( maxHeight * startWidth ) / startHeight;
	}
	else
	{
		maxHeight = ( maxWidth * startHeight ) / startWidth;
	}


	isBounded = true;
}
void Viewport::setBounds( RectF boundaries )
{
	setBounds( boundaries.x, boundaries.y, boundaries.width, boundaries.height );
}
void Viewport::freeBounds()
{
	isBounded = false;
}
void Viewport::setZoom( double modifier )
{
	if ( modifier <= 0 || modifier == currentZoom ) return;
	float newWidth, newHeight;
	newWidth = (float)( startWidth * modifier );
	newHeight = (float)( startHeight * modifier );

	if ( isBounded )
	{
		if ( newWidth > maxWidth )
		{
			newWidth = maxWidth;
		}
		if ( newHeight > maxHeight )
		{
			newHeight = maxHeight;
		}
	}

	pos.width = newWidth;
	pos.height = newHeight;
	view.setSize( newWidth, newHeight );
	center( getCenter() );
	currentZoom = modifier;
}
void Viewport::center( float x, float y ) // centers viewport
{
	float newX = x, newY = y;
	if ( isBounded )
	{
		if ( x - ( pos.width / 2 ) < bounds.x ) // too far left
		{
			newX = bounds.x + ( pos.width / 2 );
		}
		else if ( x + ( pos.width / 2 ) > bounds.width ) // too far right
		{
			newX = bounds.width - ( pos.width / 2 );
		}

		if ( y - ( pos.height / 2 ) < bounds.y ) // too far up
		{
			newY = bounds.y + ( pos.height / 2 );
		}
		else if ( y + ( pos.height / 2 ) > bounds.height ) // too far down
		{
			newY = bounds.height - ( pos.height / 2 );
		}
	}
	view.setCenter( newX, newY );
}
void Viewport::center( Point p ) // centers viewport
{
	center( (float)p.x, (float)p.y );
}

sf::View * Viewport::getSFView()
{
	return &view;
}
Point Viewport::getTopRightCorner() const
{
	Point ret;
	ret.x = view.getCenter().x - ( bounds.width / 2.f );
	ret.y = view.getCenter().y - ( bounds.height / 2.f );
	return ret;
}
Point Viewport::getCenter() const
{
	Point ret;
	ret.x = view.getCenter().x;
	ret.y = view.getCenter().y;
	return ret;
}
double Viewport::getCurrentZoom() const
{
	return currentZoom;
}