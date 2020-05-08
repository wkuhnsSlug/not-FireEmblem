#pragma once
#include "Extras.h"
#include "AuxMath.h"

template <typename T>
class Circle
{
public:
	Circle()
	{

	}
	Circle( T x, T y, T rad )
	{
		init( x, y, rad );
	}
	Circle( Point p, T rad )
	{
		init( p, rad );
	}
	~Circle()
	{

	}
	virtual void init( T x, T y, T rad )
	{
		xPosition = x;
		yPosition = y;
		radius = rad;
	}
	virtual void init( Point p, T rad )
	{
		xPosition = (T)p.x;
		yPosition = (T)p.y;
		radius = rad;
	}

	Point getPosition() const
	{
		return{ (double)xPosition, (double)yPosition };
	}
	virtual void setPosition( T x, T y )
	{
		xPosition = x;
		yPosition = y;
	}
	virtual void setPosition( Point p )
	{
		xPosition = (T)p.x;
		yPosition = (T)p.y;
	}
	T getRadius() const
	{
		return radius;
	}
	void setRadius( T rad )
	{
		radius = rad;
	}

	bool isColliding( const Circle<T> & other ) const
	{
		return isColliding( *this, other );
	}
	static bool isColliding( const Circle<T> & c1, const Circle<T> & c2 ) // just as accurate as double precision
	{
		if ( AuxMath::distance( (double)c1.xPosition, (double)c1.yPosition,
			(double)c2.xPosition, (double)c2.yPosition )
			< c1.radius + c2.radius )
		{
			return true;
		}
		return false;
	}

//private:
	T xPosition;
	T yPosition;
	T radius;
};

typedef Circle<float> CircleF;
typedef Circle<double> CircleD;
typedef Circle<int> CircleI;