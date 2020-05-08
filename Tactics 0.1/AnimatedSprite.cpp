// last edited 05/10/17
#include "AnimatedSprite.h"
#include <fstream>
#include <cassert>
//#define ANIMATED_SPRITE_DEBUGGING_OUTPUT_ENABLED
#ifdef ANIMATED_SPRITE_DEBUGGING_OUTPUT_ENABLED
#include <iostream>
#endif

AnimatedSprite::AnimatedSprite()
{

}
AnimatedSprite::AnimatedSprite( std::string filename )
{
	initialize( filename );
}
AnimatedSprite::AnimatedSprite( std::string filename, int columns, int rows, int pxCol, int pxRow, int numFrm, int step )
{
	initialize( filename, columns, rows, pxCol, pxRow, numFrm, step );
}
AnimatedSprite::~AnimatedSprite()
{

}
void AnimatedSprite::initialize( std::string filename )
{
	std::ifstream infile( filename );
	std::string str;
	infile >> numColumns >> numRows >> pixelsPerColumn >> pixelsPerRow >> numFrames >> step >> offsetX >> offsetY;
	infile.close();

	str = filename;
	str[str.length() - 3] = 'p';
	str[str.length() - 2] = 'n';
	str[str.length() - 1] = 'g';
	init( str );
}
void AnimatedSprite::initialize( std::string filename, int columns, int rows, int pxCol, int pxRow, int numFrm, int step )
{
	this->step = step;
	numColumns = columns;
	numRows = rows;
	pixelsPerColumn = pxCol;
	pixelsPerRow = pxRow;
	numFrames = numFrm;
	init( filename );
}

int AnimatedSprite::getWidth() const
{
	return pixelsPerColumn;
}
int AnimatedSprite::getHeight() const
{
	return pixelsPerRow;
}

void AnimatedSprite::update()
{
	if ( stepCounter % step == 0 )
	{
		stepCounter = 1;
		curFrame++;
		curFrame %= numFrames;
		
		// update image
		int curRow = 0, curCol = 0;
		if ( curFrame > 0 )
		{
			curRow = ( curFrame / numColumns ) * pixelsPerRow;
			curCol = ( curFrame % numColumns ) * pixelsPerColumn;
		}
		sfSprite->setTextureRect( sf::IntRect( curCol, curRow, pixelsPerColumn - 1, pixelsPerRow - 1 ) );
	}
	else
	{
		stepCounter++;
	}
}
int AnimatedSprite::getFrame() const
{
	return curFrame;
}
int AnimatedSprite::getNumFrames() const
{
	return numFrames;
}
void AnimatedSprite::setFrame( int frame )
{
	curFrame = numFrames + frame - 1;
	stepCounter = step;
	update();
}

int AnimatedSprite::getStep() const
{
	return step;
}
void AnimatedSprite::setStep( int newStep )
{
	step = newStep;
}

// private
void AnimatedSprite::init( std::string filename )
{
	curFrame = 0;
	stepCounter = 0;
	assert( numColumns > 0 && numRows > 0 );
	//numFrames = numColumns * numRows;

	sf::Texture * tex = new sf::Texture;
	if ( tex->loadFromFile( filename ) )
	{
#ifdef ANIMATED_SPRITE_DEBUGGING_OUTPUT_ENABLED
		std::cout << "Initialized " << filename << std::endl;
	}
	else
	{
		std::cout << "Failed to initialize " << filename << std::endl;
#endif
	}
	tex->setSmooth( true );
	sfTexture = tex;
	width = sfTexture->getSize().x;
	height = sfTexture->getSize().y;
	offsetX = 0;
	offsetY = 0;

	sfSprite = new sf::Sprite;
	sfSprite->setTexture( *sfTexture );
	sfSprite->setTextureRect( sf::IntRect( 0, 0, pixelsPerColumn - 1, pixelsPerRow - 1 ) );
}