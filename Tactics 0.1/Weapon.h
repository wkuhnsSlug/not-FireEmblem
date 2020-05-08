#pragma once
#include <string>
#include "LevelDefines.h"
#include "Item.h"

class Weapon : public Item
{
public:
	int might;
	int accuracy;
	int crit;
	int weight;
	int minRange;
	int maxRange;
	WeaponType weaponType;
	WeaponRank rank;
	int classCanWield;
	int effectiveAgainst;
};

Weapon loadWeapon( std::string filename, int uses = -1 );