// last edited 06/30/17
#pragma once
#include "Sprite.h"

class AnimatedSprite : public Sprite
{
public:
	AnimatedSprite();
	AnimatedSprite( std::string filename );
	AnimatedSprite( std::string filename, int columns, int rows, int pxCol, int pxRow, int numFrm,
		int step = 1/*used to slow animation, e.g. set to 2 to take twice as long*/ );
	~AnimatedSprite();
	void initialize( std::string filename );
	void initialize( std::string filename, int columns, int rows, int pxCol, int pxRow, int numFrm,
		int step = 1/*used to slow animation, e.g. set to 2 to take twice as long*/ );

	int getWidth() const override;
	int getHeight() const override;

	void update();
	int getFrame() const;
	int getNumFrames() const;
	void setFrame( int frame );
	int getStep() const;
	void setStep( int newStep );

private:
	int step; /*used to slow animation, e.g. set to 2 to take twice as long*/ 
	int stepCounter;
	int numColumns;
	int numRows;
	int pixelsPerColumn;
	int pixelsPerRow;
	int curFrame;
	int numFrames; // just a precalculated rows * columns

	void init( std::string filename );
};