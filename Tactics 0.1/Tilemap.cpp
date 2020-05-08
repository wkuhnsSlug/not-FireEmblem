#include "Tilemap.h"
#include "Utility.h"
#include <fstream>
#include <cassert>


Tilemap::Tilemap()
{
	tileset = NULL;
	mapArray = NULL;
	elevationArray = NULL;
}
Tilemap::Tilemap( std::string filename )
{
	init( filename );
}

Tilemap::~Tilemap()
{
	clear();
}


void Tilemap::init( std::string filename )
{
	std::ifstream infile;
	infile.open( filename );
	infile >> mapWidth >> mapHeight >> numTiles >> tileWidth >> tileHeight;

	tileset = new Tile*[numTiles];
	std::string tilename;
	for ( int i = 0; i < numTiles; i++ )
	{
		std::string s;
		infile >> tilename;
		tileset[i] = new Tile( tilename );
	}

	mapArray = new int[mapWidth * mapHeight];
	int x;
	for ( unsigned i = 0; !infile.eof() && i < mapWidth * mapHeight; i++ )
	{
		infile >> x;
		mapArray[i] = x;
	}
	infile.close();

	loadElevationMap( filename.substr( 0, filename.length() - 4 ) + "Elevation.txt" );
}
void Tilemap::clear()
{
	if ( tileset )
	{
		for ( int i = 0; i < numTiles; i++ )
		{
			delete tileset[i];
		}
		delete[] tileset;
	}

	if ( mapArray )
	{
		delete[] mapArray;
	}
	if ( elevationArray )
	{
		delete[] elevationArray;
	}

	tileset = NULL;
	mapArray = NULL;
	elevationArray = NULL;
}
bool Tilemap::isInBounds( int x, int y ) const
{
	return ( x >= 0 && y >= 0 && x < (int)mapWidth && y < (int)mapHeight );
}
bool Tilemap::isInBounds( Point p ) const
{
	return isInBounds( (int)p.x, (int)p.y );
}

Tile* Tilemap::getTile( unsigned x, unsigned y )
{
	int index = x + ( y * mapWidth );
	return tileset[mapArray[index]];
}
Tile* Tilemap::getTile( int x, int y )
{
	return getTile( (unsigned)x, (unsigned)y );
}
Tile* Tilemap::getTile( Point p )
{
	return getTile( (unsigned)p.x, (unsigned)p.y );
}

int Tilemap::getElevation( unsigned x, unsigned y ) const
{
	return getElevation( (int)x, (int)y );
}
int Tilemap::getElevation( int x, int y ) const
{
	assert( x >= 0 && y >= 0 && x < (int)mapWidth && y < (int)mapHeight );
	return getElevation( x + ( y * mapWidth ) );
}
int Tilemap::getElevation( Point p ) const
{
	return getElevation( (int)p.x, (int)p.y );
}
int Tilemap::getElevation( int arrayOffset ) const
{
	assert( arrayOffset < (int)mapWidth * (int)mapHeight );
	return elevationArray[arrayOffset];
}

unsigned Tilemap::getTileWidth() const
{
	return tileWidth;
}
unsigned Tilemap::getTileHeight() const
{
	return tileHeight;
}
unsigned Tilemap::getMapWidth() const
{
	return mapWidth;
}
unsigned Tilemap::getMapHeight() const
{
	return mapHeight;
}
unsigned Tilemap::getMapSize() const
{
	return mapWidth * mapHeight;
}

int Tilemap::getInt( Point p ) const
{
	return (int)p.x + (int)p.y * getMapWidth();
}
Point Tilemap::getPoint( int i ) const
{
	Point ret;
	ret.x = i % getMapWidth();
	ret.y = i / getMapWidth();
	return ret;
}

int Tilemap::gridDistance( Point p1, Point p2 )
{
	return ( (int)fabs( p1.x - p2.x ) + (int)fabs( p1.y - p2.y ) );
}


void Tilemap::loadElevationMap( std::string filename )
{
	elevationArray = new int[mapWidth * mapHeight];

	std::ifstream infile( filename );
	int x;
	for ( unsigned i = 0; !infile.eof() && i < mapWidth * mapHeight; i++ )
	{
		infile >> x;
		elevationArray[i] = x;
	}
	infile.close();
}