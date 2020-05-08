#include "ShaderParamTexture.h"


ShaderParamTexture::ShaderParamTexture( std::string shaderID )
{
	shaderName = shaderID;
	deleteSprite = true;
}
ShaderParamTexture::~ShaderParamTexture()
{
	if ( deleteSprite )
	{
		delete sprite;
	}
}


void ShaderParamTexture::setParams( sf::Shader * s ) 
{
	s->setUniform( "texture02", *( sprite->getSFSprite()->getTexture() ) );
}

void ShaderParamTexture::setTexture( Sprite * spr )
{
	//sfShader.setParameter( "clipping", *( sprite.getSFSprite()->getTexture() ) );
	sprite = spr;
	//sfShader.setUniform( "texture02", *( sprite->getSFSprite()->getTexture() ) );
}
Sprite * ShaderParamTexture::getTexture()
{
	return sprite;
}
void ShaderParamTexture::setDeleteSprite( bool del )
{
	deleteSprite = del;
}
bool ShaderParamTexture::getDeleteSprite() const
{
	return deleteSprite;
}