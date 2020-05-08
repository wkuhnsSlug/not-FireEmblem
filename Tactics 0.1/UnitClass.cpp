#include "UnitClass.h"
#include "Utility.h"
#include <fstream>
#include <sstream>

UnitClass::UnitClass()
{
	// ~~
}
UnitClass::UnitClass( std::string filename )
{
	init( filename );
}
UnitClass::~UnitClass()
{
	// ~~
}
void UnitClass::init( std::string filename )
{
	movementPermissions[TERRAIN_IMPASSABLE] = 100;
	classType = 0;

	for ( int i = 0; i < N_WEAPON_TYPES; i++ )
	{
		weaponWieldPermissions[i] = false;
	}
	
	std::ifstream infile( filename );
	std::string token, str;

	while ( !infile.eof() )
	{
		std::getline( infile, str );
		std::stringstream sstream( str );
		if ( str.empty() )continue;

		sstream >> token;

		if ( token == "name" )
		{
			int x = str.find( " " );
			className = str.substr( x + 1 );
		}
		else if ( token == "move" )
		{
			sstream >> move;
		}
		else if ( token == "fist" )
		{
			weaponWieldPermissions[WEAPON_FIST] = true;
		}
		else if ( token == "knife" )
		{
			weaponWieldPermissions[WEAPON_KNIFE] = true;
		}
		else if ( token == "sword" )
		{
			weaponWieldPermissions[WEAPON_SWORD] = true;
		}
		else if ( token == "lance" )
		{
			weaponWieldPermissions[WEAPON_LANCE] = true;
		}
		else if ( token == "axe" )
		{
			weaponWieldPermissions[WEAPON_AXE] = true;
		}
		else if ( token == "club" )
		{
			weaponWieldPermissions[WEAPON_CLUB] = true;
		}
		else if ( token == "handgun" )
		{
			weaponWieldPermissions[WEAPON_HANDGUN] = true;
		}
		else if ( token == "shotgun" )
		{
			weaponWieldPermissions[WEAPON_SHOTGUN] = true;
		}
		else if ( token == "rifle" )
		{
			weaponWieldPermissions[WEAPON_RIFLE] = true;
		}
		else if ( token == "medicine" )
		{
			weaponWieldPermissions[WEAPON_MEDICINE] = true;
		}
		else if ( token == "terrain" )
		{
			sstream >> token;
			sstream >> movementPermissions[util::getTerrainType( token )];
		}
		else if ( token == "mounted" )
		{
			classType = classType | CLASS_MOUNTED;
		}
		else if ( token == "armored" )
		{
			classType = classType | CLASS_ARMORED;
		}
		else if ( token == "infantry" )
		{
			classType = classType | CLASS_INFANTRY;
		}
	}


	infile.close();
}

bool UnitClass::canWield( WeaponType wepType ) const
{
	return weaponWieldPermissions[wepType];
}
bool UnitClass::canWield( const Weapon * wep ) const
{
	return weaponWieldPermissions[wep->weaponType] && ( classType & wep->classCanWield );
}