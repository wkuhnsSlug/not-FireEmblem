#pragma once
#include "SFML\Graphics.hpp"
#include <string>

class Font
{
public:
	Font();
	Font( std::string filename, int pointSize );
	~Font();
	void init( std::string filename, int pointSize );

	sf::Text * getSFText();

private:
	sf::Text text;
	sf::Font font;
};