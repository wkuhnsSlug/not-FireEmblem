#include "ShaderNoParam.h"
ShaderNoParam::ShaderNoParam( std::string shaderID )
{
	shaderName = shaderID;
}
ShaderNoParam::~ShaderNoParam()
{
	// ~~~
}

void ShaderNoParam::setShaderID( std::string shaderID )
{
	shaderName = shaderID;
}