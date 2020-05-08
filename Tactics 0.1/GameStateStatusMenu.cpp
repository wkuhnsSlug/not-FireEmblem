#include "GameStateStatusMenu.h"
#include "GameStateMap.h"
#include "Utility.h"

GameStateStatusMenu::GameStateStatusMenu( GameData & gameData, CommonSprites& commonSprites,
	int infoSelection, bool enemyInformation )
	:
	GameState( gameData, commonSprites )
{
	unitInfoSelection = infoSelection;
	isEnemyInformation = enemyInformation;
}

GameStateStatusMenu::~GameStateStatusMenu()
{
	// ~~~
}


void GameStateStatusMenu::update()
{
	if ( g.mouse.getButtonRelease( RIGHT_CLICK ) )
	{
		centerViewport( ( isEnemyInformation ? g.enemyUnits[unitInfoSelection]->getPosition()
			: g.playerUnits[unitInfoSelection]->getPosition() ) );

		g.gameState = new GameStateMap( g, s );
		delete this;
		return;
	}
	else if ( g.mouse.getMouseWheelUp() )
	{
		if ( !isEnemyInformation )
		{
			unitInfoSelection = ( unitInfoSelection - 1 + g.playerUnits.size() ) % g.playerUnits.size();
		}
		else
		{
			unitInfoSelection = ( unitInfoSelection - 1 + g.enemyUnits.size() ) % g.enemyUnits.size();
		}
	}
	else if ( g.mouse.getMouseWheelDown() )
	{
		if ( !isEnemyInformation )
		{
			unitInfoSelection = ( unitInfoSelection + 1 ) % g.playerUnits.size();
		}
		else
		{
			unitInfoSelection = ( unitInfoSelection + 1 ) % g.enemyUnits.size();
		}
	}
}
void GameStateStatusMenu::drawToScreen()
{
	if ( !isEnemyInformation )
	{
		g.gfx.drawFixed( &s.menuBackgroundBlue, 0, 0 );
		drawUnitInformation( g.playerUnits[unitInfoSelection] );
	}
	else
	{
		g.gfx.drawFixed( &s.menuBackgroundRed, 0, 0 );
		drawUnitInformation( g.enemyUnits[unitInfoSelection] );
	}
	drawMouse();
}


void GameStateStatusMenu::drawUnitInformation( Unit * unit )
{
	g.gfx.drawFixed( unit->getName(), &s.menuFont, 100, 100, 10, 10, 10 );
	g.gfx.drawFixed( unit->getClassName(), &s.menuFontSmall, 100, 125, 10, 10, 10 );
	std::string str = "HP: " + std::to_string( unit->getCurrentHP() )
		+ "/" + std::to_string( unit->getMaxHP() );
	g.gfx.drawFixed( str, &s.menuFont, 100, 150, 10, 10, 10 );

	str = "Lv " + std::to_string( unit->getLevel() ) + "  EXP: "
		+ std::to_string( unit->getExp() ) + "/100";
	str += "\nMOV: " + std::to_string( unit->getMaxMovement() );
	str += "\nBuild: " + std::to_string( unit->getBuild() ) 
		+ "   Aid: " + std::to_string( unit->getAid() );
	g.gfx.drawFixed( str, &s.menuFontSmall, 120, 220, 10, 10, 10 );

	str = "STR: " + std::to_string( unit->getStrength() );
	str += "\nDEF: " + std::to_string( unit->getDefense() );
	str += "\nSPD: " + std::to_string( unit->getSpeed() );
	str += "\nACC: " + std::to_string( unit->getAccuracy() );
	str += "\nAGI: " + std::to_string( unit->getAgility() );
	str += "\nSKL: " + std::to_string( unit->getSkill() );
	str += "\nLCK: " + std::to_string( unit->getLuck() );
	g.gfx.drawFixed( str, &s.menuFontSmall, 120, 300, 10, 10, 10 );


	str = "Weapon Ranks: ";
	str += "\nFist: ";
	str += util::getWeaponLetter( unit->getWeaponRank( WEAPON_FIST ) );
	str += "            Knife: ";
	str += util::getWeaponLetter( unit->getWeaponRank( WEAPON_KNIFE ) );
	str += "\nSword: ";
	str += util::getWeaponLetter( unit->getWeaponRank( WEAPON_SWORD ) );
	str += "        Lance: ";
	str += util::getWeaponLetter( unit->getWeaponRank( WEAPON_LANCE ) );
	str += "\nClub: ";
	str += util::getWeaponLetter( unit->getWeaponRank( WEAPON_CLUB ) );
	str += "           Axe: ";
	str += util::getWeaponLetter( unit->getWeaponRank( WEAPON_AXE ) );
	str += "\nHandgun: ";
	str += util::getWeaponLetter( unit->getWeaponRank( WEAPON_HANDGUN ) );
	str += "    Shotgun: ";
	str += util::getWeaponLetter( unit->getWeaponRank( WEAPON_SHOTGUN ) );
	str += "\nRifle: ";
	str += util::getWeaponLetter( unit->getWeaponRank( WEAPON_RIFLE ) );
	str += "          Medicine: ";
	str += util::getWeaponLetter( unit->getWeaponRank( WEAPON_MEDICINE ) );
	g.gfx.drawFixed( str, &s.menuFontSmall, 1200, 760, 10, 10, 10 );

	Item * curItem;
	Weapon * curWep;
	for ( int i = 0; i < unit->getInventorySize(); i++ )
	{
		curItem = ( unit->getInventory()[i] );
		if ( curItem->itemType == ITEM_WEAPON )
		{
			curWep = (Weapon*)curItem;

			unsigned colorAdd = unit->getInventory()[i]->itemType == ITEM_WEAPON
				&& !unit->canWield( (Weapon*)unit->getInventory()[i] ) ? 100 : 0;
			str = curWep->name;
			g.gfx.drawFixed( str, &s.menuFontSmall, 520, 180 + ( 30 * i ),
				20 + colorAdd, 20 + colorAdd, 20 + colorAdd );
			str = "  MT " + std::to_string( curWep->might );
			str += "  HIT " + std::to_string( curWep->accuracy );
			str += "  CRT " + std::to_string( curWep->crit );
			str += "  WT " + std::to_string( curWep->weight );
			str += "  RNG " + std::to_string( curWep->minRange ) +
				( curWep->minRange < curWep->maxRange ? "-" + std::to_string( curWep->maxRange ) : "" );
			str += "  RNK ";
			str +=( util::getWeaponLetter( curWep->rank ) );
			str += "  Uses " + std::to_string( curWep->currentUses ) + "/" + std::to_string( curWep->maxUses );

			g.gfx.drawFixed( str, &s.menuFontSmall, 780, 180 + ( 30 * i ),
				20 + colorAdd, 20 + colorAdd, 20 + colorAdd );
		}
		else
		{
			unsigned colorAdd = ( unit->getInventory()[i]->itemType == ITEM_HEAL_OTHER
				&& !unit->canWield( (Medicine*)unit->getInventory()[i] ) )
				|| ( ( unit->getInventory()[i]->itemType == ITEM_HEAL_SELF
				&& unit->getCurrentHP() == unit->getMaxHP() ) ) ? 100 : 0;
			str = curItem->name;
			g.gfx.drawFixed( str, &s.menuFontSmall, 520, 180 + ( 30 * i ),
				20 + colorAdd, 20 + colorAdd, 20 + colorAdd );

			str = "Uses " + std::to_string( curItem->currentUses ) + "/" + std::to_string( curItem->maxUses );
			g.gfx.drawFixed( str, &s.menuFontSmall, 780, 180 + ( 30 * i ),
				20 + colorAdd, 20 + colorAdd, 20 + colorAdd );
		}
	}
}