#pragma once
#include "Shader.h"
#include "Sprite.h"

class ShaderNoParam : public Shader
{
public:
	ShaderNoParam( std::string shaderID );
	~ShaderNoParam();

	void setShaderID( std::string shaderID );
};