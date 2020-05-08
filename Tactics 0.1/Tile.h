#pragma once
#include "LevelDefines.h"
#include <string>

class Tile
{
public:
	Tile();
	Tile( std::string filename );
	void init( std::string filename );
	TerrainType getTerrain() const;
	int getDef() const;
	int getAvo() const;
	std::string getName() const;

private:
	TerrainType terrainType;
	int def;
	int avo;
	std::string name;
};