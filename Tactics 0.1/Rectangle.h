#pragma once

#include "Extras.h"

//template <class T> class Rect;

template <class T>
class Rect
{
public:
	T x;
	T y;
	T width;
	T height;
	Rect( void ) {}

	Rect( T X, T Y, T Width, T Height ) :
		x( X ),
		y( Y ),
		width( Width ),
		height( Height )
	{}

	void setVal( T X, T Y, T Width, T Height )
	{
		x = X;
		y = Y;
		width = Width;
		height = Height;
	}

	bool isColliding( Rect<T> rect2 ) const
	{
		return ( x < rect2.x + rect2.width &&
			x + width > rect2.x &&
			y < rect2.y + rect2.height &&
			y + height > rect2.y );
	}

	bool isColliding( T x2, T y2, T width2, T height2 ) const
	{
		return ( x < x2 + width2 &&
			x + width > x2 &&
			y < y2 + height2 &&
			y + height > y2 );
	}

	bool isColliding( Point p2, T width2, T height2 ) const
	{
		return ( x < p2.x + width2 &&
			x + width > p2.x &&
			y < p2.y + height2 &&
			y + height > p2.y );
	}
};
typedef Rect<int> RectI;
typedef Rect<float> RectF;
typedef Rect<double> RectD;