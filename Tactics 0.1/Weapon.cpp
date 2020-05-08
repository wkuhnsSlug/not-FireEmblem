#include "Weapon.h"
#include "Utility.h"
#include <fstream>
#include <sstream>

Weapon loadWeapon( std::string filename, int uses )
{
	Weapon ret;
	ret.itemType = ITEM_WEAPON;
	ret.classCanWield = 0;
	ret.effectiveAgainst = 0;

	std::ifstream infile( filename );
	std::string str, token;

	while ( !infile.eof() )
	{
		std::getline( infile, str );
		std::stringstream sstream( str );
		if ( str.empty() )continue;

		sstream >> token;

		if ( token == "name" )
		{
			int x = str.find( " " );
			ret.name = str.substr( x + 1 );
		}
		else if ( token == "uses" )
		{
			sstream >> ret.maxUses;
		}
		else if ( token == "might" )
		{
			sstream >> ret.might;
		}
		else if ( token == "accuracy" )
		{
			sstream >> ret.accuracy;
		}
		else if ( token == "crit" )
		{
			sstream >> ret.crit;
		}
		else if ( token == "weight" )
		{
			sstream >> ret.weight;
		}
		else if ( token == "minRange" )
		{
			sstream >> ret.minRange;
		}
		else if ( token == "maxRange" )
		{
			sstream >> ret.maxRange;
		}
		else if ( token == "type" )
		{
			sstream >> token;
			ret.weaponType = util::getWeaponType( token );
		}
		else if ( token == "rank" )
		{
			char c;
			sstream >> c;
			ret.rank = util::getWeaponRank( c );
		}
		else if ( token == "wield" )
		{
			sstream >> token;
			if ( token == "mounted" )
			{
				ret.classCanWield |= CLASS_MOUNTED;
			}
			else if ( token == "armored" )
			{
				ret.classCanWield |= CLASS_ARMORED;
			}
			else if ( token == "infantry" )
			{
				ret.classCanWield |= CLASS_INFANTRY;
			}
			else if ( token == "all" )
			{
				ret.classCanWield |= CLASS_ALL;
			}
		}
		else if ( token == "effective" )
		{
			sstream >> token;
			if ( token == "mounted" )
			{
				ret.effectiveAgainst |= CLASS_MOUNTED;
			}
			else if ( token == "armored" )
			{
				ret.effectiveAgainst |= CLASS_ARMORED;
			}
			else if ( token == "infantry" )
			{
				ret.effectiveAgainst |= CLASS_INFANTRY;
			}
			else if ( token == "all" )
			{
				ret.effectiveAgainst |= CLASS_ALL;
			}
		}
	}

	if ( uses == -1 )
	{
		ret.currentUses = ret.maxUses;
	}
	else
	{
		ret.currentUses = uses;
	}

	infile.close();
	return ret;
}