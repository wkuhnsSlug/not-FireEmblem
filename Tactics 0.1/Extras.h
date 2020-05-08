#pragma once

#define SCREENWIDTH 1920
#define SCREENHEIGHT 1080

struct Point
{
	double x;
	double y;

	bool operator==( const Point& other )
	{
		return this->x == other.x && this->y == other.y;
	}
	Point()
	{
		x = 0;
		y = 0;
	}
	Point( double x2, double y2 )
	{
		x = x2;
		y = y2;
	}
	Point( int x2, int y2 )
	{
		x = (double)x2;
		y = (double)y2;
	}
	Point( unsigned x2, unsigned y2 )
	{
		x = (double)x2;
		y = (double)y2;
	}
	Point( float x2, float y2 )
	{
		x = (double)x2;
		y = (double)y2;
	}
};

struct Color
{
	unsigned short r;
	unsigned short g;
	unsigned short b;
	unsigned short a;
};