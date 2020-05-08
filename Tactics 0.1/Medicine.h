#pragma once
#include "Item.h"
#include "LevelDefines.h"

class Medicine : public Item
{
public:
	int recoveryAmount;
	WeaponRank rank;
};

Medicine loadMedicine( std::string filename, int uses = -1 );