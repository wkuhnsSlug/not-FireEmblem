#pragma once
#include "Shader.h"
#include "Sprite.h"

class ShaderParamTexture : public Shader
{
public:
	ShaderParamTexture( std::string shaderID );
	~ShaderParamTexture();

	void setParams( sf::Shader * s ) override;

	void setTexture( Sprite * spr );
	Sprite * getTexture();
	void setDeleteSprite( bool del );
	bool getDeleteSprite() const;
	
protected:
	Sprite * sprite;
	bool deleteSprite;
};