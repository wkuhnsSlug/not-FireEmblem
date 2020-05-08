#pragma once
#include "Weapon.h"
#include <string>

// class Class lmao
class UnitClass
{
public:
	UnitClass();
	UnitClass( std::string filename );
	virtual ~UnitClass();

	void init( std::string filename );

	bool canWield( WeaponType wepType ) const;
	bool canWield( const Weapon * wep ) const;
	int getTerrainCost( TerrainType type ) { return movementPermissions[type]; }
	int getMove() const { return move; }
	bool isMounted() const { return ( classType & CLASS_MOUNTED ); }
	std::string getClassName() const { return className; }
	int getClassType() const { return classType; }

protected:
	bool weaponWieldPermissions[N_WEAPON_TYPES];
	int movementPermissions[N_TERRAIN_TYPES];
	int move;
	int classType;
	std::string className;
};