// last updated 05/16/17
#pragma once
#include "SFML\Graphics.hpp"
#include "Rectangle.h"
#include "Extras.h"

class Viewport
{
public:
	Viewport();
	Viewport( RectF dimensions );
	Viewport( float x, float y, float width, float height );

	void init( RectF dimensions );
	void init( float x, float y, float width, float height );

	void setBounds( float left, float top, float right, float bottom );
	void setBounds( RectF boundaries );
	void freeBounds();
	void setZoom( double modifier );
	void center( float x, float y ); // centers viewport
	void center( Point p ); // centers viewport
	sf::View * getSFView();
	Point getTopRightCorner() const;
	Point getCenter() const;
	double getCurrentZoom() const;

private:
	sf::View view;
	RectF pos;
	RectF bounds;
	bool isBounded;
	double currentZoom;
	float startWidth;
	float startHeight;
	float maxWidth;
	float maxHeight;
};