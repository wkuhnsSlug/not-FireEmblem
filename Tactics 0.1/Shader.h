#pragma once
#include <SFML\Graphics.hpp>
#include <string>

class Shader
{
public:
	Shader();
	virtual ~Shader();

	virtual void setParams( sf::Shader * s ) {} // default: does nothing
	virtual void update() {}; // default: does nothing

	std::string getName();

protected:
	std::string shaderName;
};