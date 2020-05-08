#include "Utility.h"

WeaponRank Utility::getWeaponRank( char c )
{
	switch ( c )
	{
	case 'A':
	case 'a':
		return RANK_A;
		break;
	case 'B':
	case 'b':
		return RANK_B;
		break;
	case 'C':
	case 'c':
		return RANK_C;
		break;
	case 'D':
	case 'd':
		return RANK_D;
		break;
	case 'F':
	case 'f':
		return RANK_F;
		break;
	case 'S':
	case 's':
		return RANK_S;
		break;
	default:
		return RANK_NONE;
		break;
	}
}
WeaponType Utility::getWeaponType( std::string str )
{
	if ( str == "fist" )
	{
		return WEAPON_FIST;
	}
	else if ( str == "knife" )
	{
		return WEAPON_KNIFE;
	}
	else if ( str == "sword" )
	{
		return WEAPON_SWORD;
	}
	else if ( str == "lance" )
	{
		return WEAPON_LANCE;
	}
	else if ( str == "club" )
	{
		return WEAPON_CLUB;
	}
	else if ( str == "axe" )
	{
		return WEAPON_AXE;
	}
	else if ( str == "handgun" )
	{
		return WEAPON_HANDGUN;
	}
	else if ( str == "shotgun" )
	{
		return WEAPON_SHOTGUN;
	}
	else if ( str == "rifle" )
	{
		return WEAPON_RIFLE;
	}
	else if ( str == "medicine" )
	{
		return WEAPON_MEDICINE;
	}
	else
	{
		return WEAPON_NULL;
	}
}

char Utility::getWeaponLetter( WeaponRank rank )
{
	switch ( rank )
	{
	case RANK_A:
		return 'A';
		break;
	case RANK_B:
		return 'B';
		break;
	case RANK_C:
		return 'C';
		break;
	case RANK_D:
		return 'D';
		break;
	case RANK_F:
		return 'F';
		break;
	case RANK_S:
		return 'S';
		break;
	case RANK_NONE:
	default:
		return '-';
		break;
	}
}
TerrainType Utility::getTerrainType( std::string str )
{
	if ( str == "floor" ) return TERRAIN_FLOOR;
	if ( str == "grass" ) return TERRAIN_GRASS;
	if ( str == "scrub" ) return TERRAIN_SCRUB;
	if ( str == "woods" ) return TERRAIN_WOODS;
	if ( str == "cliff" ) return TERRAIN_CLIFF;
	if ( str == "water" ) return TERRAIN_WATER;
	return TERRAIN_IMPASSABLE;
}
bool Utility::isGun( WeaponType wep )
{
	return wep == WEAPON_HANDGUN || wep == WEAPON_SHOTGUN || wep == WEAPON_RIFLE;
}