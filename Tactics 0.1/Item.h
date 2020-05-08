#pragma once
#include <string>

enum ItemType
{
	ITEM_WEAPON,
	ITEM_HEAL_SELF,
	ITEM_HEAL_OTHER,
	N_ITEM_TYPES
};

class Item
{
public:
	std::string name;
	ItemType itemType;
	int currentUses;
	int maxUses;
};