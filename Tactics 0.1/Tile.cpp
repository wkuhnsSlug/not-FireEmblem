#include "Tile.h"
#include "Utility.h"
#include <fstream>

Tile::Tile()
{

}
Tile::Tile( std::string filename )
{
	init( filename );
}

void Tile::init( std::string filename )
{
	std::ifstream infile( filename );
	std::string token;

	while ( !infile.eof() )
	{
		infile >> token;

		if ( token == "name" )
		{
			infile >> name;
		}
		else if ( token == "avo" )
		{
			infile >> avo;
		}
		else if ( token == "def" )
		{
			infile >> def;
		}
		else if ( token == "terrain" )
		{
			infile >> token;
			terrainType = Utility::getTerrainType( token );
		}
	}

}

TerrainType Tile::getTerrain() const
{
	return terrainType;
}
int Tile::getDef() const
{
	return def;
}
int Tile::getAvo() const
{
	return avo;
}
std::string Tile::getName() const
{
	return name;
}