#include "ShaderContainer.h"
#define DEBUG_OUTPUT_SHADER
#ifdef DEBUG_OUTPUT_SHADER
#include <iostream>
#endif

std::unordered_map<std::string, sf::Shader*> ShaderContainer::shaderMap;

ShaderContainer::ShaderContainer()
{

}
ShaderContainer::~ShaderContainer()
{

}

bool ShaderContainer::loadShader( std::string filename, std::string shaderID )
{
	if ( shaderMap.count( shaderID ) == 0 )
	{
		// load shader
		sf::Shader * s = new sf::Shader;
		if ( !s->loadFromFile( filename, sf::Shader::Fragment ) )
		{
#ifdef DEBUG_OUTPUT_SHADER
			std::cout << "Error loading shader at " << filename << std::endl;
#endif
			delete s;
			return false;
		}
		s->setUniform( "texture", sf::Shader::CurrentTexture ); // every shader needs this; may as well do it here

		// then add it to the map
		shaderMap.insert( { shaderID, s } );

		return true;
	}
	return true;
}
sf::Shader * ShaderContainer::getShader( std::string shaderID )
{
	if ( shaderMap.count( shaderID ) > 0 )
	{
		return shaderMap[shaderID];
	}
	return NULL;
}
void ShaderContainer::clearAllShaders()
{
	while ( !shaderMap.empty() )
	{
		delete shaderMap.begin()->second; // delete the pointer
		shaderMap.erase( shaderMap.begin() ); // then remove the entry
	}
}