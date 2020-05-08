#define _USE_MATH_DEFINES
#include <math.h>
// last edited 10-13-16
#pragma once
#include "Extras.h"

template<class T>

class VectorMath // couldn't name it Vector b/c vector was taken
{
public:
	VectorMath() {}
	VectorMath( T X, T Y ) { x = X; y = Y; }

	T magnitude() const
	{
		return sqrt( x * x + y * y );
	}
	T lengthSquared() const
	{
		return ( x * x + y * y );
	}
	T direction() const
	{
		return atan( y / x );
	}

	static VectorMath<T>	add( const VectorMath<T> & lhs, const VectorMath<T> & rhs )
	{
		VectorMath<T> ret;
		ret.x = lhs.x + rhs.x;
		ret.y = lhs.y + rhs.y;
		return ret;
	}
	static VectorMath<T>	subtract( const VectorMath<T> & lhs, const VectorMath<T> & rhs )
	{
		VectorMath<T> ret;
		ret.x = lhs.x - rhs.x;
		ret.y = lhs.y - rhs.y;
		return ret;
	}
	static VectorMath<T>	scalarMultiply( T scalar, const VectorMath<T> & v )
	{
		return{ v.x * scalar, v.y * scalar };
	}
	static VectorMath<T>	scalarDivide( T scalar, const VectorMath<T> & v )
	{
		return{ v.x / scalar, v.y / scalar };
	}
	/*
	static VectorMath<T>	crossProduct( const VectorMath<T> & lhs, const VectorMath<T> & rhs )
	{

	}
	*/
	static T				dotProduct( const VectorMath<T> & lhs, const VectorMath<T> & rhs )
	{
		return lhs.x * rhs.x + lhs.y * rhs.y;
	}

	static T				scalarProjection( const VectorMath<T> & from, const VectorMath<T> & onto )
	{
		return dotProduct( from, changeMagnitude( 1, onto ) );
	}
	static VectorMath<T>	vectorProjection( const VectorMath<T> & from, const VectorMath<T> & onto )
	{
		VectorMath<T> vec, ret;
		vec = changeMagnitude( 1, onto ); // saving this b/c it's used twice
		// vector projection = [scalar projection; a dot ^b] * ^b
		// where ^b is the unit vector of b
		ret = scalarMultiply( dotProduct( from, vec ), vec );
		return ret;
	}
	/*static Position			getPoint( const VectorMath<T> & vec, T scalar )
	{
	Position ret;
	}*/

	// returns between 0 and pi
	static T				angleBetween( const VectorMath<T> & vec1, const VectorMath<T> & vec2 )
	{
		return acos( dotProduct( vec1, vec2 ) / ( vec1.magnitude() * vec2.magnitude() ) );

		// the following is another algorithm to try in case the above doesn't work (though I think it does)
		// got it from the same place I got the other angle between algorithms
		// can't really tell which one is faster; I think the first one??
		//VectorMath<T> v1, v2;
		//// normalize vectors
		//v1 = changeMagnitude( 1, vec1 );
		//v2 = changeMagnitude( 1, vec2 );
		//return acos( dotProduct( v1, v2 ) );
	}
	// returns between -pi and pi
	static T				angleBetween_Relative( const VectorMath<T> & vec1, const VectorMath<T> & vec2 )
	{
		VectorMath<T> v1, v2;
		// normalize vectors
		v1 = changeMagnitude( 1, vec1 );
		v2 = changeMagnitude( 1, vec2 );
		// I copied this off the internet, so it better work
		T ret =  atan2( v2.y, v2.x ) - atan2( v1.y, v1.x );
		if ( ret > M_PI )
		{
			ret -= ( M_PI + M_PI );
		}
		else if ( ret < -M_PI )
		{
			ret += M_PI + M_PI;
		}
		return ret;
	}
	// returns between 0 and 2pi
	static T				angleBetween_Absolute( const VectorMath<T> & vec1, const VectorMath<T> & vec2 )
	{
		T ret = angleBetween_Relative( vec1, vec2 );
		if ( ret < 0 )
		{
			ret += M_PI + M_PI;
		}
		return ret;
	}

	static VectorMath<T>	getVector( T x1, T y1, T x2, T y2 )
	{
		VectorMath<T> ret = { x2 - x1, y2 - y1 };
		return ret;
	}
	static VectorMath<T>	getVector( T mag, T dir )
	{
		return{ mag * cos( dir ), mag * sin( dir ) };
	}
	static VectorMath<T>	getNormalLeft( const VectorMath<T> & vec )
	{
		return{ -vec.y, vec.x };
	}
	static VectorMath<T>	getNormalRight( const VectorMath<T> & vec )
	{
		return{ vec.y, -vec.x };
	}
	static VectorMath<T>	negate( const VectorMath<T> & vec )
	{
		return{ -vec.x, -vec.y };
	}
	static VectorMath<T>	changeMagnitude( const T mag, const VectorMath<T> & vec )
	{
		return VectorMath<T>::scalarMultiply( mag / vec.magnitude(), vec );
	}
	static VectorMath<T>	reflectionOffOf( const VectorMath<T> & from, const VectorMath<T> onto )
	{
		//VectorMath<T> normal = getNormalLeft( onto );
		// should this be		
		VectorMath<T> normal = changeMagnitude( 1, onto );	//???
		return subtract( from, scalarMultiply( 2 * dotProduct( from, normal ), normal ) );

	}
	static VectorMath<T>	getVectorFromAngle( T angleInRadians, T magnitude )
	{
		VectorMath<T> ret;
		ret.y = sin( angleInRadians ) * magnitude;
		ret.x = cos( angleInRadians ) * magnitude;
		return ret;
	}
	static T				getAngleFromVector( const VectorMath<T> & vec )
	{
		return angleBetween_Absolute( vec, { 1, 0 } );
	}
	static VectorMath<T>	rotateBy( T angleInRadians, const VectorMath<T> &vec )
	{
		// Stack Overflow, don't fail me now!
		VectorMath<T> ret;
		double cs = cos( angleInRadians );
		double sn = sin( angleInRadians );

		ret.x = vec.x * cs - vec.y * sn;
		ret.y = vec.x * sn + vec.y * cs;

		return ret;
	}


	T x;
	T y;

private:

};
typedef VectorMath<int> VectorI;
typedef VectorMath<float> VectorF;
typedef VectorMath<double> VectorD;