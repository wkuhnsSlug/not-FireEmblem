#include "Medicine.h"
#include "Utility.h"
#include <fstream>
#include <sstream>

Medicine loadMedicine( std::string filename, int uses )
{
	Medicine ret;
	ret.itemType = ITEM_HEAL_OTHER;


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
		else if ( token == "recoveryAmount" )
		{
			sstream >> ret.recoveryAmount;
		}
		else if ( token == "rank" )
		{
			char c;
			sstream >> c;
			ret.rank = util::getWeaponRank( c );
		}
		else if ( token == "uses" )
		{
			sstream >> ret.maxUses;
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