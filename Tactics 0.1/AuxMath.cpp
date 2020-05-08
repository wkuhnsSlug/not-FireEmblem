#define _USE_MATH_DEFINES
#include <cmath>
#include "AuxMath.h"


Point AuxMath::midpoint( Point p1, Point p2 )
{
	Point ret;
	ret.x = ( p1.x + p2.x ) / 2.0;
	ret.y = ( p1.y + p2.y ) / 2.0;
	return ret;
}
double AuxMath::distance( double x1, double y1, double x2, double y2 )
{
	return sqrt( pow( x2 - x1, 2 ) + pow( y2 - y1, 2 ) );
}
double AuxMath::distance( Point p1, Point p2 )
{
	return distance( p1.x, p1.y, p2.x, p2.y );
}
bool AuxMath::fpApproxEquals( double x1, double x2 )
{
	return fabs( x1 - x2 ) < 0.0001;
}
double AuxMath::degreeFromRadian( double radian )
{
	return radian * 180.0 / M_PI;
}
double AuxMath::radianFromDegree( double degree )
{
	return degree / 180.0 * M_PI;
}

double AuxMath::mirrorAngleOffXAxis_Radians( double angleInRadians )
{
	return twoPi - angleInRadians;
}
double AuxMath::mirrorAngleOffXAxis_Degrees( double angleInDegrees )
{
	return 360 - angleInDegrees;
}
double AuxMath::mirrorAngleOffYAxis_Radians( double angleInRadians )
{
	double angle = angleInRadians;
	if ( angle == 0 )
	{
		angle = M_PI;
	}
	else if ( angle > M_PI_2 && angle < AuxMath::threePiOverTwo ) // facing left
	{
		//angle = M_PI - ( angle - M_PI );
		angle = fmod( ( ( AuxMath::threePiOverTwo - angle ) + AuxMath::threePiOverTwo ), AuxMath::twoPi );
	}
	else // facing right
	{
		//angle = M_PI + ( M_PI - angle );
		if ( angle < M_PI ) // covering my bases here, really need angle < M_PI_2 
		{
			angle = M_PI - angle;
		}
		else // angle between 3pi/2 and 2pi
		{
			angle = M_PI + AuxMath::twoPi - angle;
		}
	}
	return angle;
}
double AuxMath::mirrorAngleOffYAxis_Degrees( double angleInDegrees )
{
	double angle = angleInDegrees;
	if ( angle >= 180 )
	{
		angle = 180 - angle;
	}
	else
	{
		angle = ( 360 - angle ) + 180;
	}
	return angle;
}

const double AuxMath::threePiOverTwo = M_PI + M_PI_2;
const double AuxMath::twoPi = M_PI + M_PI;