#pragma once
#include <string>
#include <unordered_map>
#include "SFML\Graphics.hpp"

class ShaderContainer
{
public:
	ShaderContainer();
	~ShaderContainer();

	static bool loadShader( std::string filename, std::string shaderID );
	static sf::Shader * getShader( std::string shaderID );
	static void clearAllShaders();

private:
	static std::unordered_map<std::string, sf::Shader*> shaderMap;
};