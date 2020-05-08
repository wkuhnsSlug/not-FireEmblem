#include "Font.h"

Font::Font()
{

}

Font::Font( std::string filename, int pointSize )
{
	init( filename, pointSize );
}

Font::~Font()
{

}

void Font::init( std::string filename, int pointSize )
{
	font.loadFromFile( filename );
	text.setFont( font );
	text.setCharacterSize( pointSize );
}

sf::Text * Font::getSFText()
{
	return &text;
}