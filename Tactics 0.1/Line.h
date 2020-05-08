// last updated 05/01/17
#pragma once
#include "VectorMath2D.h"

class Line
{
public:
	Line();
	Line( double xSt, double ySt, double xEd, double yEd );
	Line( Point start, Point end );

	void initialize( double xSt, double ySt, double xEd, double yEd );
	void changeStart( double x, double y );
	void changeEnd( double x, double y );
	void changeVector( VectorD vector );
	void changeMagnitude( double mag ); // What was I thinking, how would that work?
	void move( double newX, double newY ); // changes starting point while preserving vector
	void addVector( VectorD vector );
	bool isOnLine( Point p );
	bool isOnLine( double x, double y );
	bool hasNoLength();

	double getStartX() const;
	double getStartY() const;
	double getEndX() const;
	double getEndY() const;
	Point getStart() const;
	Point getEnd() const;

	VectorD getVector();
	double getMagnitude();
	double getDirection(); // could be useful, I guess
	// mag and direction are not const b/c they'll calculate the mag/dir if it doesn't exist
	// already and store it. there'll be a flag determining if it needs to be recalculated.
	double getSlope(); // returns NaN if vertical (slope undefined)
	double getYIntercept(); // returns Nan if vertical (slope undefined)

	static Point getIntersection( Line l1, Line l2 ); // assumes slopes are valid and lines not coincident/parallel
	static bool isIntersecting( Line l1, Line l2 );
	static bool lineCircleCollision( Line l1, Point center, double rad );


private:
	double xStart;
	double yStart;
	double xEnd;
	double yEnd;

	VectorD vec;
	double mag;
	double dir;

	bool dirtyMag;
	bool dirtyDir;

	void setDirty(); // sets all flags to dirty
	void setVec();
};