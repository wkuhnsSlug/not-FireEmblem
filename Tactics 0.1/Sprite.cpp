#include "Sprite.h"

#include <iostream>

Sprite::Sprite()
{
	sfTexture = NULL;
	sfSprite = NULL;
	shader = NULL;
	deleteShader = true;
}
Sprite::Sprite( sf::Texture * tex )
{
	shader = NULL;
	deleteShader = true;
	offsetX = 0;
	offsetY = 0;

	tex->setSmooth( true );
	width = tex->getSize().x;
	height = tex->getSize().y;

	sfSprite = new sf::Sprite;
	sfSprite->setTexture( *tex );

	sfTexture = tex;
}
Sprite::Sprite( std::string filename )
{
	initialize( filename );
}
Sprite::Sprite( std::string filename, int xOffset, int yOffset )
{
	initialize( filename, xOffset, yOffset );
}
Sprite::~Sprite()
{
	clear();
	if ( deleteShader )
	{
		delete shader;
	}
}
void Sprite::initialize( std::string filename, int xOffset, int yOffset )
{
	shader = NULL;
	deleteShader = true;
	offsetX = xOffset;
	offsetY = yOffset;

	sf::Texture * tex = new sf::Texture;
	if ( tex->loadFromFile( filename ) )
	{
		//std::cout << "Initialized " << filename << std::endl;
	}
	else
	{
		std::cout << "Failed to initialize " << filename << std::endl;
	}
	tex->setSmooth( true );
	width = tex->getSize().x;
	height = tex->getSize().y;

	sfSprite = new sf::Sprite;
	sfSprite->setTexture( *tex );

	sfTexture = tex;
}
void Sprite::clear() // deletes any dynamically allocated resources; called in dtor
{
	delete sfSprite;
	delete sfTexture;
}


int Sprite::getWidth() const
{
	return width;
}
int Sprite::getHeight() const
{
	return height;
}
sf::Sprite * Sprite::getSFSprite() const
{
	return sfSprite;
}


void Sprite::setShader( Shader * shdr )
{
	shader = shdr;
}
Shader * Sprite::getShader()
{
	return shader;
}
void Sprite::setDeleteShader( bool del )
{
	deleteShader = del;
}
bool Sprite::getDeleteShader() const
{
	return deleteShader;
}
int Sprite::getOffsetX() const
{
	return offsetX;
}
int Sprite::getOffsetY() const
{
	return offsetY;
}