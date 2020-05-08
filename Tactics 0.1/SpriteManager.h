#pragma once
#include "Sprite.h"
#include <SFML\Graphics.hpp>

class SpriteManager
{
public:
	SpriteManager();
	~SpriteManager();

protected:
	void initSprite( Sprite* s );
	void setSpriteDimensions( Sprite* s, int x, int y );
	void setTexture( Sprite * s, const sf::Texture * tex );
};