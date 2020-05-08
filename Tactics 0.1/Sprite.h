// updated 08/12/17
#pragma once
#include "SFML/Graphics.hpp"
#include <string>
#include "Shader.h"

class SpriteManager;

class Sprite
{
	friend class SpriteManager;
public:
	Sprite();
	Sprite( sf::Texture * tex );
	Sprite( std::string filename );
	Sprite( std::string filename, int xOffset, int yOffset );
	virtual ~Sprite();
	virtual void initialize( std::string filename, int xOffset = 0, int yOffset = 0 );
	virtual void clear();

	virtual sf::Sprite * getSFSprite() const;
	virtual int getWidth() const;
	virtual int getHeight() const;

	void setShader( Shader * shdr );
	Shader * getShader();
	void setDeleteShader( bool del );
	bool getDeleteShader() const;
	int getOffsetX() const;
	int getOffsetY() const;


protected:
	int width;
	int height;
	int size;
	int offsetX;
	int offsetY;

	Shader * shader;
	bool deleteShader;

	const sf::Texture * sfTexture;
	sf::Sprite * sfSprite;
};