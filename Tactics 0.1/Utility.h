#pragma once
#include "LevelDefines.h"
#include <string>

class Utility
{
public:
	static WeaponRank getWeaponRank( char c );
	static WeaponType getWeaponType( std::string str );
	static char getWeaponLetter( WeaponRank rank );
	static TerrainType getTerrainType( std::string str );
	static bool isGun( WeaponType wep );
};
typedef Utility util;