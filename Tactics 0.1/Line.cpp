// updated 09-25-26
#include "Line.h"
#include "AuxMath.h"
#include <cassert>

// it's ridiculous I need this, but I can't find the proper #include
#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

Line::Line()
{
	setDirty();
}
Line::Line( double xSt, double ySt, double xEd, double yEd )
	: xStart( xSt ),
	yStart( ySt ),
	xEnd( xEd ),
	yEnd( yEd )
{
	setDirty();
	setVec();
}
Line::Line( Point start, Point end )
	: xStart( start.x ),
	yStart( start.y ),
	xEnd( end.x ),
	yEnd( end.y )
{
	setDirty();
	setVec();
}

void Line::initialize( double xSt, double ySt, double xEd, double yEd )
{
	xStart = xSt;
	yStart = ySt;
	xEnd = xEd;
	yEnd = yEd;
	setDirty();
	setVec();
}
void Line::changeStart( double x, double y )
{
	xStart = x;
	yStart = y;
	setVec();
	setDirty();
}
void Line::changeEnd( double x, double y )
{
	xEnd = x;
	yEnd = y;
	setVec();
	setDirty();
}
void Line::changeVector( VectorD vector )
{
	changeEnd( xStart + vector.x, yStart + vector.y );
}
void Line::changeMagnitude( double mag )
{
	changeVector( VectorD::changeMagnitude( mag, vec ) );
}
void Line::move( double newX, double newY )
{
	Line ret;
	ret.initialize( newX, newY, newX + vec.x, newY + vec.y );
	*this = ret;
}
void Line::addVector( VectorD vector )
{
	changeVector( VectorD::add( vector, vec ) );
}
bool Line::isOnLine( Point p )
{
	return isOnLine( p.x, p.y );
}
bool Line::isOnLine( double x, double y )
{
	double slope = this->getSlope();
	if ( isnan( slope ) )
	{
		return x == xStart;
	}
	return AuxMath::fpApproxEquals( y, ( slope * x ) + this->getYIntercept() );
}
bool Line::hasNoLength()
{
	return xStart == xEnd && yStart == yEnd;
}

double Line::getStartX() const
{
	return xStart;
}
double Line::getStartY() const
{
	return yStart;
}
double Line::getEndX() const
{
	return xEnd;
}
double Line::getEndY() const
{
	return yEnd;
}
Point Line::getStart() const
{
	Point ret = { xStart, yStart };
	return ret;
}
Point Line::getEnd() const
{
	Point ret = { xEnd, yEnd };
	return ret;
}

VectorD Line::getVector()
{
	setVec();
	return vec;
}
double Line::getMagnitude()
{
	//if ( dirtyMag )
	//{
	mag = vec.magnitude();
	dirtyMag = false;
	//}
	return mag;
}
double Line::getDirection()
{
	//if ( dirtyDir )
	//{
	dir = vec.direction();
	dirtyDir = false;
	//}
	return dir;
}
double Line::getSlope()
{
	if ( xStart == xEnd ) // vertical line, undefined slope
	{
		return NAN;
	}

	if ( yStart == yEnd ) // horizontal line, zero slope. Frickin -0.
	{
		return 0;
	}

	return ( ( yEnd - yStart ) / ( xEnd - xStart ) );
}
double Line::getYIntercept()
{
	double m = getSlope();
	if ( isnan( m ) )
	{
		return NAN;
	}

	return yStart - ( m * xStart );
}
Point Line::getIntersection( Line l1, Line l2 )
{
	Point ret;
	double m1, m2, b1, b2;
	m1 = l1.getSlope();
	m2 = l2.getSlope();
	b1 = l1.getYIntercept();
	b2 = l2.getYIntercept();

	// checking that lines are not parallel
	assert( ( m1 != m2 ) && !( isnan( m1 ) && isnan( m2 ) ) );
	//if ( ( m1 != m2 ) && !( isnan( m1 ) && isnan( m2 ) ) ) return{ 0, 0 };


	if ( isnan( m1 ) )
	{
		ret.x = l1.getStartX();
		ret.y = ( m2 * ret.x ) + b2;
	}
	else if ( isnan( m2 ) )
	{
		ret.x = l2.getStartX();
		ret.y = ( m1 * ret.x ) + b1;
	}
	else
	{
		ret.x = ( b2 - b1 ) / ( m1 - m2 );
		ret.y = m1 * ret.x + b1;
	}
	return ret;
}
bool Line::isIntersecting( Line l1, Line l2 )
{
	// God bless StackOverflow, for allowing me to not write code
	double p0_x, p1_x, p2_x, p3_x, p0_y, p1_y, p2_y, p3_y;
	p0_x = l1.getStartX();
	p0_y = l1.getStartY();
	p1_x = l1.getEndX();
	p1_y = l1.getEndY();
	p2_x = l2.getStartX();
	p2_y = l2.getStartY();
	p3_x = l2.getEndX();
	p3_y = l2.getEndY();

	//if ( ( p0_x == p2_x && p0_y == p2_y )
	//	|| ( p0_x == p2_x && p0_y == p2_y ) ) return false; // ignore if lines have same endpoint

	double s1_x, s1_y, s2_x, s2_y;
	s1_x = p1_x - p0_x;     s1_y = p1_y - p0_y;
	s2_x = p3_x - p2_x;     s2_y = p3_y - p2_y;

	double s, t;
	s = ( -s1_y * ( p0_x - p2_x ) + s1_x * ( p0_y - p2_y ) ) / ( -s2_x * s1_y + s1_x * s2_y );
	t = ( s2_x * ( p0_y - p2_y ) - s2_y * ( p0_x - p2_x ) ) / ( -s2_x * s1_y + s1_x * s2_y );

	if ( s >= 0 && s <= 1 && t >= 0 && t <= 1 )
	{
		// Collision detected

		// stuff for returning variables
		/*
		if ( i_x != NULL )
		*i_x = p0_x + ( t * s1_x );
		if ( i_y != NULL )
		*i_y = p0_y + ( t * s1_y );
		*/
		return true;
	}

	return false; // No collision
}
bool Line::lineCircleCollision( Line l1, Point center, double rad )
{
	/*Line l2;
	l2.initialize( l1.getStartX(), l2.getStartY(), center.x, center.y );
	double projMag = VectorD::projection( l2.getVector(), l1.getVector() );
	double lineMag = l1.getMagnitude();
	if ( projMag > lineMag || projMag < 0 )
	{
		return false;
	}
	l2 = l1;
	l2.changeMagnitude( projMag );
	if ( AuxMath::distance( l2.getEndX(), l2.getEndY(), center.x, center.y ) <= rad )
	{
		return true;
	}
	return false;*/

	Line l2;
	l2.initialize( l1.getStartX(), l2.getStartY(), center.x, center.y );
	VectorD projection = VectorD::vectorProjection( l2.getVector(), l1.getVector() );
	double distance = AuxMath::distance( projection.x, projection.y, center.x, center.y );
	if ( distance >= rad )
	{
		// vectors pointing opposite directions
		if ( ( VectorD::angleBetween( l2.getVector(), l1.getVector() ) > M_PI_2 )
			// and the distance between the start of the vector and the center of the circle > rad
			&& ( AuxMath::distance( center.x, center.y, l1.getStartX(), l1.getStartY() ) > rad ) )
		{
			return false;
		}
		return true;
	}
	return false;
}

// private
void Line::setDirty()
{
	dirtyDir = true;
	dirtyMag = true;
}
void Line::setVec()
{
	vec = VectorMath<double>::getVector( xStart, yStart, xEnd, yEnd );
}

