// last edited 10/25/16
#pragma once
#include "VectorMath2D.h"

class AuxMath
{
public:
	static Point midpoint( Point p1, Point p2 );
	static double distance( double x1, double y1, double x2, double y2 );
	static double distance( Point p1, Point p2 );
	static bool fpApproxEquals( double x1, double x2 );
	static double degreeFromRadian( double radian );
	static double radianFromDegree( double degree );
	static double mirrorAngleOffXAxis_Radians( double angleInRadians );
	static double mirrorAngleOffXAxis_Degrees( double angleInDegrees );
	static double mirrorAngleOffYAxis_Radians( double angleInRadians );
	static double mirrorAngleOffYAxis_Degrees( double angleInDegrees );

	static const double threePiOverTwo;
	static const double twoPi;
};