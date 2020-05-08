#include "SpriteManager.h"

SpriteManager::SpriteManager()
{

}
SpriteManager::~SpriteManager()
{

}

void SpriteManager::initSprite( Sprite* s )
{
	s->sfSprite = new sf::Sprite;
}
void SpriteManager::setSpriteDimensions( Sprite* s, int x, int y )
{
	s->width = x;
	s->height = y;
}
void SpriteManager::setTexture( Sprite * s, const sf::Texture * tex )
{
	s->sfTexture = tex;
	s->sfSprite->setTexture( *s->sfTexture );
}