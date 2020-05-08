#pragma once

#include <string>
#include "Extras.h"
#include "Tile.h"

class Tilemap
{
public:
	Tilemap();
	Tilemap( std::string filename);
	~Tilemap();

	void init( std::string filename );
	void clear();
	bool isInBounds( int x, int y ) const;
	bool isInBounds( Point p ) const;

	Tile* getTile( unsigned x, unsigned y );
	Tile* getTile( int x, int y );
	Tile* getTile( Point p );

	int getElevation( unsigned x, unsigned y ) const;
	int getElevation( int x, int y ) const;
	int getElevation( Point p ) const;
	int getElevation( int arrayOffset ) const;

	unsigned getTileWidth() const;
	unsigned getTileHeight() const;
	unsigned getMapWidth() const;
	unsigned getMapHeight() const;
	unsigned getMapSize() const;

	int getInt( Point p ) const;
	Point getPoint( int i ) const;

	static int gridDistance( Point p1, Point p2 );

private:
	unsigned tileWidth;
	unsigned tileHeight;
	unsigned mapWidth;
	unsigned mapHeight;

	Tile** tileset;
	int * mapArray;
	int * elevationArray;
	int numTiles;

	void loadElevationMap( std::string filename );
};