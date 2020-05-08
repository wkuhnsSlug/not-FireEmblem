#include "Unit.h"
#include "Utility.h"
#include "GameState.h"
#include "ShaderNoParam.h"
#include <cstdlib>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <cmath>
#include <thread>

Unit::Unit()
{
	sprite = NULL;
	pos = { 0, 0 };
}
Unit::Unit( std::string filename, int xPos, int yPos )
{
	init( filename, xPos, yPos );
}
Unit::~Unit()
{
	if ( sprite ) delete sprite;
	for ( int i = 0; i < currentInventorySize; i++ )
	{
		delete inventory[i];
	}
}

void Unit::init( std::string filename, int xPos, int yPos )
{
	pos.x = xPos;
	pos.y = yPos;
	currentInventorySize = 0;
	for ( int i = 0; i < INVENTORY_SIZE; i++ )
	{
		inventory[i] = NULL;
	}
	activeStatus = true;
	isCurrentlyMarked = false;
	rescuedUnit = NULL;
	isBeingRescued = false;
	hasTraded = false;
	hasTransfered = false;

	std::ifstream infile( filename );
	std::string str, token;
	int xOff, yOff;
	infile >> str >> xOff >> yOff;
	sprite = new Sprite( str, xOff, yOff );
	getSprite()->setShader( new ShaderNoParam( "null" ) );

	while ( !infile.eof() )
	{
		std::getline( infile, str );
		std::stringstream sstream( str );
		if ( str.empty() )continue;

		sstream >> token;

		if ( token == "class" )
		{
			int x = str.find( " " );
			myClass.init( str.substr( x + 1 ) );
		}
		else if ( token == "name" )
		{
			int x = str.find( " " );
			name = str.substr( x + 1 );
		}
		else if ( token == "level" )
		{
			sstream >> currentLevel;
		}
		else if ( token == "exp" )
		{
			sstream >> currentEXP;
		}
		/*else if ( token == "move" )
		{
			sstream >> maxMovement;
		}*/
		else if ( token == "hp" )
		{
			sstream >> maxHP;
			currentHP = maxHP;
		}
		else if ( token == "strength" )
		{
			sstream >> strength;
		}
		else if ( token == "defense" )
		{
			sstream >> defense;
		}
		else if ( token == "speed" )
		{
			sstream >> speed;
		}
		else if ( token == "accuracy" )
		{
			sstream >> accuracy;
		}
		else if ( token == "agility" )
		{
			sstream >> agility;
		}
		else if ( token == "skill" )
		{
			sstream >> skill;
		}
		else if ( token == "luck" )
		{
			sstream >> luck;
		}
		else if ( token == "build" )
		{
			sstream >> build;
		}
		else if ( token == "rank" )
		{
			std::string type;
			char c;
			sstream >> type >> c;
			wepRank[util::getWeaponType( type )] = util::getWeaponRank( c );
		}
	}


	infile.close();
}
void Unit::calculateMovement( Tilemap * map, OccupiedStatus * occupied,
	OccupiedStatus self, int movementRemaining )
{
	movement.clear();
	attackRange.clear();
	healRange.clear();
	int movRem = ( movementRemaining >= 0 ? movementRemaining : getMaxMovement() );

	// Dijkstra's
	// fucking STL Jesus Christ why can't I even implement a simple Dijkstra's search without
	//		spending a million years searching for a third-party template that doesn't even work
	//		fucking STL fuck you fuck fuck kill me kill YOU why is this hard AHHHHHH
	int mapSize = map->getMapWidth() * map->getMapHeight();
	int mapWidth = map->getMapWidth();
	int mapHeight = map->getMapHeight();
	
	// init arrays
	std::vector<int> toEvaluate;
	dItem* data = new dItem[mapSize];
	vec = &data;

	int currentIdx = map->getInt( pos );
	data[currentIdx].distance = 0;
	toEvaluate.push_back( currentIdx );
	data[currentIdx].wasAdded = true;


	while ( !toEvaluate.empty() )
	{
		currentIdx = toEvaluate[toEvaluate.size() - 1]; // get current item
		toEvaluate.pop_back(); // remove current from heap

		data[currentIdx].shouldEvaluate = false;
		Point current = map->getPoint( currentIdx );

		// go through all neighbors
		dijkstraHelper( { current.x + 1, current.y }, currentIdx, data, &toEvaluate, map, occupied, self );
		dijkstraHelper( { current.x - 1, current.y }, currentIdx, data, &toEvaluate, map, occupied, self );
		dijkstraHelper( { current.x, current.y + 1 }, currentIdx, data, &toEvaluate, map, occupied, self );
		dijkstraHelper( { current.x, current.y - 1 }, currentIdx, data, &toEvaluate, map, occupied, self );


		// now sort!
		if ( !toEvaluate.empty() )
		{
			qsort( &toEvaluate[0], toEvaluate.size(), sizeof( int ), Unit::compare );
		}
	}

	bool * canStand = new bool[mapSize];
	bool * canAttack = new bool[mapSize];
	bool * canHeal = new bool[mapSize];

	for ( int i = 0; i < mapSize; i++ )
	{
		canAttack[i] = false;
		canHeal[i] = false;
	}

	for ( int i = 0; i < mapSize; i++ )
	{
		//canAttack[i] = false;
		canStand[i] = false;
		if ( /*i != map->getInt( pos ) && */
			data[i].distance <= movRem )
			//&& occupied[i] == NOT_OCCUPIED  )
		{
			movement.push_back( map->getPoint( i ) );
			canStand[i] = true;

			// solve for attack range from tile
			/**/for ( int j = 0; j < currentInventorySize; j++ )
			{
				if ( inventory[j]->itemType == ITEM_WEAPON && canWield( (Weapon*)inventory[j] ) )
				{
					Weapon * w = (Weapon*)inventory[j];
					if ( Utility::isGun( w->weaponType ) )
					{
						/*std::vector<Point> vec;
						getGunRange( map->getPoint( i ), *w, map, vec );

						for ( unsigned k = 0; k < vec.size(); k++ )
						{
							canAttack[map->getInt( vec[k] )] = true;
						}*/
					}
					else // "melee" weapon
					{
						getWeaponRange( map->getPoint( i ), w->minRange, w->maxRange, canAttack, map );
					}
				}
				else if ( inventory[j]->itemType == ITEM_HEAL_OTHER && canWield( (Medicine*)inventory[j] ) )
				{
					Medicine * m = (Medicine*)inventory[j];
					getWeaponRange( map->getPoint( i ), 1, 1, canHeal, map );
				}
			}/**/
		}
	}

	if ( getWeapon() )
	{
		// solve for guns
		std::vector<Point> vec, vec2;
		if ( movement.size() <= 3 ) // not worth it on small arrays, and division would be weird
		{
			solveForAttackRange( movement, 0, movement.size(), inventory, currentInventorySize,
				map, &vec, this );
			for ( unsigned i = 0; i < vec.size(); i++ )
			{
				canAttack[map->getInt( vec[i] )] = true;
			}
		}
		else
		{
			std::thread thr( solveForAttackRange, movement, movement.size() / 2, movement.size(),
				inventory, currentInventorySize, map, &vec2, this );

			solveForAttackRange( movement, 0, movement.size() / 2, inventory, currentInventorySize,
				map, &vec, this );
			for ( unsigned i = 0; i < vec.size(); i++ )
			{
				canAttack[map->getInt( vec[i] )] = true;
			}

			thr.join();
			for ( unsigned i = 0; i < vec2.size(); i++ )
			{
				canAttack[map->getInt( vec2[i] )] = true;
			}

		}

		// add to attack range vector
		for ( int i = 0; i < mapSize; i++ )
		{
			if ( canAttack[i] )
			{
				attackRange.push_back( map->getPoint( i ) );
			}
		}
	}

	if ( myClass.canWield( WEAPON_MEDICINE ) )
	{
		for ( int i = 0; i < mapSize; i++ )
		{
			if ( canHeal[i] )
			{
				healRange.push_back( map->getPoint( i ) );
			}
		}
	}


	delete[] canAttack;
	delete[] canStand;
	delete[] canHeal;
	delete[] data;
}
void Unit::calculateAvailableActions( Point position, std::vector<Unit*>& enemyVector,
	std::vector<Unit*>& allyVector, Tilemap * map, OccupiedStatus * occupied )
{
	actionTypes.clear();

	rescuableAllies.clear();
	transferableAllies.clear();
	dropRange.clear();
	tradablePositions.clear();
	potentialTradePartners.clear();

	//calculateEnemiesInRange( position, enemyVector );
	calculateAttackRangeActive( position, map );
	calculateAdjacentAllies( position, allyVector );
	if ( !attackRangeActive.empty() )
	{
		bool exitLoop = false;
		for ( unsigned i = 0; i < enemyVector.size() && !exitLoop; i++ )
		{
			for ( unsigned j = 0; j < attackRangeActive.size() && !exitLoop; j++ )
			{
				if ( enemyVector[i]->getPosition() == attackRangeActive[j] )
				{
					actionTypes.push_back( ACTION_ATTACK );
					exitLoop = true;
				}
			}
		}
	}
	if ( !adjacentAllies.empty() )
	{
		if ( myClass.canWield( WEAPON_MEDICINE ) )
		{
			bool injuredAlly = false;
			// walk adjacent allies to see if anyone's hurt
			for ( unsigned i = 0; i < adjacentAllies.size() && !injuredAlly; i++ )
			{
				Unit * u = GameState::getUnit( adjacentAllies[i], map, occupied, allyVector );
				if ( u && u->getCurrentHP() < u->getMaxHP() )
				{
					injuredAlly = true;
				}
			}

			if ( injuredAlly )
			{
				// walk items
				for ( int i = 0; i < currentInventorySize; i++ )
				{
					if ( inventory[i]->itemType == ITEM_HEAL_OTHER
						&& canWield( (Medicine*)inventory[i] ) )
					{
						// push medicine
						actionTypes.push_back( ACTION_HEAL );
						break;
					}
				}
			}
		}

		if ( !hasTraded )
		{
			Point newPoint;
			newPoint = position;
			newPoint.x++;
			if ( map->isInBounds( newPoint ) && map->getInt( newPoint ) != map->getInt( this->getPosition() )
				&& ( occupied[map->getInt( newPoint )] == OCCUPIED_ALLY || map->getInt( newPoint ) == map->getInt( getPosition() ) )
				&& canStand( map->getTile( newPoint )->getTerrain() ) )
			{
				tradablePositions.push_back( newPoint );
			}
			newPoint = position;
			newPoint.x--;
			if ( map->isInBounds( newPoint ) && map->getInt( newPoint ) != map->getInt( this->getPosition() )
				&& ( occupied[map->getInt( newPoint )] == OCCUPIED_ALLY || map->getInt( newPoint ) == map->getInt( getPosition() ) )
				&& canStand( map->getTile( newPoint )->getTerrain() ) )
			{
				tradablePositions.push_back( newPoint );
			}
			newPoint = position;
			newPoint.y++;
			if ( map->isInBounds( newPoint ) && map->getInt( newPoint ) != map->getInt( this->getPosition() )
				&& ( occupied[map->getInt( newPoint )] == OCCUPIED_ALLY || map->getInt( newPoint ) == map->getInt( getPosition() ) )
				&& canStand( map->getTile( newPoint )->getTerrain() ) )
			{
				tradablePositions.push_back( newPoint );
			}
			newPoint = position;
			newPoint.y--;
			if ( map->isInBounds( newPoint ) && map->getInt( newPoint ) != map->getInt( this->getPosition() )
				&& ( occupied[map->getInt( newPoint )] == OCCUPIED_ALLY || map->getInt( newPoint ) == map->getInt( getPosition() ) )
				&& canStand( map->getTile( newPoint )->getTerrain() ) )
			{
				tradablePositions.push_back( newPoint );
			}
			for ( unsigned i = 0; i < allyVector.size(); i++ )
			{
				if ( this->pos.x == allyVector[i]->getPosition().x
					&& this->pos.y == allyVector[i]->getPosition().y ) continue; // don't add yourself!
				if ( Tilemap::gridDistance( position, allyVector[i]->getPosition() ) == 1 )
				{
					potentialTradePartners.push_back( allyVector[i] );
				}
			}
		}

		for ( unsigned i = 0; i < adjacentAllies.size(); i++ )
		{
			for ( unsigned j = 0; j < allyVector.size(); j++ )
			{
				if ( adjacentAllies[i].x == allyVector[j]->getPosition().x
					&& adjacentAllies[i].y == allyVector[j]->getPosition().y )
				{
					if ( allyVector[j]->getBuild() <= getAid()
						&& !allyVector[j]->isRescued() && !allyVector[j]->getRescuedUnit() )
					{
						rescuableAllies.push_back( adjacentAllies[i] );
					}
					break; // ally found!
				}
			}
		}

		if ( !rescuableAllies.empty() && !rescuedUnit )
		{
			actionTypes.push_back( ACTION_RESCUE );
		}
	}

	if ( rescuedUnit )
	{
		tradablePositions.push_back( position );
		potentialTradePartners.push_back( rescuedUnit );

		Point newPoint;
		newPoint = position;
		newPoint.x++;
		if ( map->isInBounds( newPoint )
			&& ( occupied[map->getInt( newPoint )] == NOT_OCCUPIED || map->getInt( newPoint ) == map->getInt( getPosition() ) )
			&& canStand( map->getTile( newPoint )->getTerrain() ) )
		{
			dropRange.push_back( newPoint );
		}
		newPoint = position;
		newPoint.x--;
		if ( map->isInBounds( newPoint )
			&& ( occupied[map->getInt( newPoint )] == NOT_OCCUPIED || map->getInt( newPoint ) == map->getInt( getPosition() ) )
			&& canStand( map->getTile( newPoint )->getTerrain() ) )
		{
			dropRange.push_back( newPoint );
		}
		newPoint = position;
		newPoint.y++;
		if ( map->isInBounds( newPoint )
			&& ( occupied[map->getInt( newPoint )] == NOT_OCCUPIED || map->getInt( newPoint ) == map->getInt( getPosition() ) )
			&& canStand( map->getTile( newPoint )->getTerrain() ) )
		{
			dropRange.push_back( newPoint );
		}
		newPoint = position;
		newPoint.y--;
		if ( map->isInBounds( newPoint )
			&& ( occupied[map->getInt( newPoint )] == NOT_OCCUPIED || map->getInt( newPoint ) == map->getInt( getPosition() ) )
			&& canStand( map->getTile( newPoint )->getTerrain() ) )
		{
			dropRange.push_back( newPoint );
		}

		if ( !dropRange.empty() )
		{
			actionTypes.push_back( ACTION_DROP );
		}

		// check for transfers
		if ( !adjacentAllies.empty() )
		{
			for ( unsigned i = 0; i < adjacentAllies.size(); i++ )
			{
				Unit * u = GameState::getUnit( adjacentAllies[i], map, occupied, allyVector );
				if ( u && u->getAid() >= getRescuedUnit()->build
					&& ( u->getRescuedUnit() ? getAid() >= u->getRescuedUnit()->getBuild() : true ) )
				{
					transferableAllies.push_back( adjacentAllies[i] );
				}
			}
			if ( !transferableAllies.empty() && !hasTransfered )
			{
				actionTypes.push_back( ACTION_TRANSFER );
			}
		}
	}
	else // no rescued unit
	{
		// check for transfers
		if ( !adjacentAllies.empty() )
		{
			for ( unsigned i = 0; i < adjacentAllies.size(); i++ )
			{
				Unit * u = GameState::getUnit( adjacentAllies[i], map, occupied, allyVector );
				if ( u && u->getRescuedUnit() && getAid() >= u->getRescuedUnit()->getBuild() )
				{
					transferableAllies.push_back( adjacentAllies[i] );
				}
			}
			if ( !transferableAllies.empty() && !hasTransfered )
			{
				actionTypes.push_back( ACTION_TRANSFER );
			}
		}
	}


	if ( currentInventorySize > 0 )
	{
		actionTypes.push_back( ACTION_ITEM );
	}

	if ( !potentialTradePartners.empty() )
	{
		actionTypes.push_back( ACTION_TRADE );
	}

	actionTypes.push_back( ACTION_WAIT ); // always

	std::sort( actionTypes.begin(), actionTypes.end() );
}
void Unit::calculateEnemiesInRange( Point position, std::vector<Unit*>& enemyVector )
{
	enemies.clear();

	const Weapon * w = getWeapon();
	if ( !w ) return;

	int attackRangeMin, attackRangeMax;
	attackRangeMin = w->minRange;
	attackRangeMax = w->maxRange;

	for ( int i = 1; i < currentInventorySize; i++ )
	{
		if ( inventory[i]->itemType == ITEM_WEAPON && canWield( (Weapon*)inventory[i] ) )
		{
			w = (Weapon*)inventory[i];
			if ( w->minRange < attackRangeMin )
				attackRangeMin = w->minRange;
			if ( w->maxRange > attackRangeMax )
				attackRangeMax = w->maxRange;
		}
	}

	for ( unsigned i = 0; i < enemyVector.size(); i++ )
	{
		if ( Tilemap::gridDistance( position, enemyVector[i]->getPosition() ) >= attackRangeMin
			&& Tilemap::gridDistance( position, enemyVector[i]->getPosition() ) <= attackRangeMax )
		{
			enemies.push_back( enemyVector[i]->getPosition() );
		}
	}
}
void Unit::calculateAttackRangeActive( Point position, Tilemap * map )
{
	attackRangeActive.clear();
	bool * isInRange = new bool[map->getMapSize()];
	int sdlfsds = map->getMapSize();
	for ( unsigned i = 0; i < map->getMapSize(); i++ )
	{
		isInRange[i] = false;
	}

	for ( int i = 0; i < currentInventorySize; i++ )
	{
		if ( inventory[i]->itemType == ITEM_WEAPON && canWield( (Weapon*)inventory[i] ) )
		{
			Weapon * w = (Weapon*)inventory[i];
			if ( Utility::isGun( w->weaponType ) )
			{
				std::vector<Point> vec;
				getGunRange( position, *w, map, vec );
				for ( unsigned j = 0; j < vec.size(); j++ )
				{
					isInRange[map->getInt( vec[j] )] = true;
				}
			}
			else // "melee" weapon
			{
				getWeaponRange( position, w->minRange, w->maxRange, isInRange, map );
			}
		}
	}

	for ( unsigned i = 0; i < map->getMapSize(); i++ )
	{
		if ( isInRange[i] )
		{
			attackRangeActive.push_back( map->getPoint( i ) );
		}
	}

	delete[] isInRange;
}
void Unit::calculateAdjacentAllies( Point position, std::vector<Unit*>& allyVector )
{
	adjacentAllies.clear();


	for ( unsigned i = 0; i < allyVector.size(); i++ )
	{
		if ( this->pos.x == allyVector[i]->getPosition().x 
			&& this->pos.y == allyVector[i]->getPosition().y ) continue; // don't add yourself!
		if ( Tilemap::gridDistance( position, allyVector[i]->getPosition() ) == 1 
			&& !(allyVector[i]->isBeingRescued) )
		{
			adjacentAllies.push_back( allyVector[i]->getPosition() );
		}
	}
}
void Unit::getGunRange( Point startPos, Weapon wep, Tilemap* map, std::vector<Point> & ret )
{
	getLOS( startPos, wep.minRange, wep.maxRange, map, ret, false );
}
void Unit::getWeaponRange( Point pos, Weapon * wep, Tilemap * map, std::vector<Point>& ret )
{
	if ( Utility::isGun( wep->weaponType ) )
	{
		getGunRange( pos, *wep, map, ret );
	}
	else // "melee" weapon
	{
		bool * isInRange = new bool[map->getMapSize()];
		for ( unsigned i = 0; i < map->getMapSize(); i++ )
		{
			isInRange[i] = false;
		}
		getWeaponRange( pos, wep->minRange, wep->maxRange, isInRange, map );
		for ( unsigned i = 0; i < map->getMapSize(); i++ )
		{
			if ( isInRange[i] )
			{
				ret.push_back( map->getPoint( i ) );
			}
		}

		delete[] isInRange;
	}
}
std::vector<Point> Unit::getWeaponRange( Point pos, Weapon * wep, Tilemap * map )
{
	std::vector<Point> ret;
	getWeaponRange( pos, wep, map, ret );
	return ret;
}

int Unit::getAttack() const
{
	return strength + (getWeapon() ? getWeapon()->might : 0);
}
int Unit::getHit() const
{
	return ( accuracy * 2 ) + ( getWeapon() ? getWeapon()->accuracy +
		( getWeaponRank( getWeapon()->weaponType ) == RANK_S ? S_RANK_HIT_BONUS : 0 ) : 0 );
}
int Unit::getAvo() const
{
	return (int)fmax( 0, ( agility * 2 ) - ( getOverburden() * 4 ) );
}
int Unit::getCrit() const
{
	return skill + ( getWeapon() ? getWeapon()->crit +
		( getWeaponRank( getWeapon()->weaponType ) == RANK_S ? S_RANK_CRIT_BONUS : 0 ) : 0 );
}
int Unit::getOverburden() const
{
	return getWeapon() ? (int)fmax( 0, getWeapon()->weight - build ) : 0;
}
int Unit::getAttackRangeMin() const
{
	const Weapon * w = getWeapon();
	if ( !w ) return 0;

	int ret = w->minRange;

	for ( int i = 1; i < currentInventorySize; i++ )
	{
		if ( inventory[i]->itemType == ITEM_WEAPON )
		{
			w = (Weapon*)inventory[i];
			if ( w->minRange < ret )
				ret = w->minRange;
		}
	}
	return ret;
}
int Unit::getAttackRangeMax() const
{
	const Weapon * w = getWeapon();
	if ( !w ) return 0;

	int ret = w->maxRange;

	for ( int i = 1; i < currentInventorySize; i++ )
	{
		if ( inventory[i]->itemType == ITEM_WEAPON )
		{
			w = (Weapon*)inventory[i];
			if ( w->maxRange > ret )
				ret = w->maxRange;
		}
	}
	return ret;
}
bool Unit::canStand( TerrainType terrain )
{
	return myClass.getTerrainCost( terrain ) <= getMaxMovement();
}

void Unit::setActive( bool active ) 
{
	if ( active )
	{
		( (ShaderNoParam*)getSprite()->getShader() )->setShaderID( "null" );
		hasTraded = false;
		hasTransfered = false;
	}
	else
	{
		( (ShaderNoParam*)getSprite()->getShader() )->setShaderID( "grayscale" );
	}
	activeStatus = active; 
}
int Unit::getAid() const
{
	return build + 1; // for now, will add mounted aid later
}
const Weapon * Unit::getWeapon() const
{
	for ( int i = 0; i < currentInventorySize; i++ )
	{
		if ( inventory[i]->itemType == ITEM_WEAPON && canWield( (Weapon*)inventory[i] ) )
		{
			return (Weapon*)inventory[i];
		}
	}
	return NULL;
}
Weapon * Unit::getWeapon() // the non-const version
{
	for ( int i = 0; i < currentInventorySize; i++ )
	{
		if ( inventory[i]->itemType == ITEM_WEAPON && canWield( (Weapon*)inventory[i] ) )
		{
			return (Weapon*)inventory[i];
		}
	}
	return NULL;
}
bool Unit::canWield( const Weapon * weapon ) const
{
	return myClass.canWield( weapon ) && ( weapon->rank <= wepRank[weapon->weaponType] );
}
bool Unit::canWield( const Medicine * med ) const
{
	return myClass.canWield( WEAPON_MEDICINE ) && ( med->rank <= wepRank[WEAPON_MEDICINE] );
}
WeaponRank Unit::getWeaponRank( WeaponType wepType ) const
{
	return myClass.canWield( wepType ) ? wepRank[wepType] : RANK_NONE;
}
void Unit::addItem( Item * item )
{
	if ( currentInventorySize < INVENTORY_SIZE )
	{
		inventory[currentInventorySize] = item;
		currentInventorySize++;
	}
	else
	{
		// else we'd add to convoy, but that doesn't exist yet
		delete item; // for safety, for now
	}
}
void Unit::equipWeapon( int index )
{
	if ( index == 0 || index >= currentInventorySize )
	{
		return; // don't want to do anything
	}

	Item * temp = inventory[0];
	inventory[0] = inventory[index];
	inventory[index] = temp;
}
void Unit::removeItem( int index )
{
	delete inventory[index];
	inventory[index] = NULL;
	consolidateInventory();
}
Item * Unit::extractItem( int index )
{
	Item * ret = inventory[index];
	inventory[index] = NULL;
	consolidateInventory();
	return ret;
}
void Unit::consolidateInventory()
{
	int newInventorySize = 0;

	for ( int i = 0; i < INVENTORY_SIZE; i++ )
	{
		if ( inventory[i] )
		{
			newInventorySize++;
		}
		else // null
		{
			for ( int j = i + 1; j < INVENTORY_SIZE; j++ )
			{
				if ( inventory[j] )
				{
					inventory[newInventorySize] = inventory[j];
					inventory[j] = NULL;
					newInventorySize++;
					break;
				}
			}
		}
	}
	currentInventorySize = newInventorySize;
}

int Unit::addExperience( int exp )
{
	currentEXP += exp;
	int ret = currentEXP / 100;
	currentEXP %= 100;
	return ret;
}


void Unit::solveForAttackRange( std::vector<Point> positions, int startPos, int endPos,
	Item** items, int itemLength, Tilemap * map, std::vector<Point>* ret, Unit * u )
{
	for ( int i = startPos; i < endPos; i++ )
	{
		for ( int j = 0; j < itemLength; j++ )
		{
			if ( items[j]->itemType == ITEM_WEAPON && u ? u->canWield( (Weapon*)items[j] ) : true )
			{
				Weapon * w = (Weapon*)items[j];
				if ( Utility::isGun( w->weaponType ) )
				{
					getGunRange( positions[i], *w, map, *ret );
				}
				else // "melee" weapon
				{
					//getWeaponRange( map->getPoint( i ), w->minRange, w->maxRange, canAttack, map );
				}
			}
		}
	}
}
void Unit::getLOS( Point startPos, int minRange, int maxRange, Tilemap * map,
	std::vector<Point>& ret, bool includePoint )
{
	int mapSize = map->getMapSize();
	int mapWidth = map->getMapWidth();
	int mapHeight = map->getMapHeight();
	bool * canSee = new bool[mapSize];
	for ( int i = 0; i < mapSize; i++ )
	{
		canSee[i] = true;
	}

	getLOS( startPos, minRange, maxRange, map, canSee, includePoint );

	for ( int x = 0; x < mapWidth; x++ )
	{
		for ( int y = 0; y < mapHeight; y++ )
		{
			if ( canSee[x + ( y * mapWidth )] )
			{
				ret.push_back( { (double)x, (double)y } );
			}
		}
	}

	delete[] canSee;
}
void Unit::getLOS( Point startPos, int minRange, int maxRange, Tilemap * map,
	bool * canSee, bool includePoint )
{
	int mapSize = map->getMapSize();
	int mapWidth = map->getMapWidth();
	int mapHeight = map->getMapHeight();


	for ( int x = 0; x < mapWidth; x++ )
	{
		for ( int y = 0; y < mapHeight; y++ )
		{
			if ( map->getTile( x, y )->getTerrain() == TERRAIN_IMPASSABLE
				&& map->gridDistance( startPos, { (double)x, (double)y } ) <= maxRange
				&& canSee[map->getInt( { (double)x, (double)y } )]
				&& !( startPos.x == x && startPos.y == y) )
			{
				LOShelper( canSee, (int)startPos.x, (int)startPos.y, x, y, mapWidth, mapHeight, maxRange );
			}
		}
	}

	if ( !includePoint )
	{
		canSee[map->getInt( startPos )] = false;
	}

	// range checking
	/**/for ( int i = 0; i < mapSize; i++ )
	{
		int dist = map->gridDistance( map->getPoint( i ), startPos );
		if ( dist < minRange || dist > maxRange )
		{
			canSee[i] = false;
		}
	}/**/

}
void Unit::LOShelper( bool * canSee, int originX, int originY, int obstructionX, int obstructionY,
	int mapWidth, int mapHeight, int maxRange )
{
	bool vert, horz;
	vert = horz = false;
	int offsetX, offsetY;
	offsetX = obstructionX - originX;
	offsetY = obstructionY - originY;
	
	// establish diagonal quadrant
	if ( fabs( (float)offsetX ) <= fabs( (float)offsetY ) )// top, bottom
	{
		vert = true;
		//if ( offsetY < 0 ) // top
		//{
		//	isTopQuadrant = true;
		//}
		//else
		//{
		//	isTopQuadrant = false;
		//}
	}
	if ( fabs( (float)offsetX ) >= fabs( (float)offsetY ) ) // left, right
	{
		horz = true;
		//if ( offsetX > 0 ) // right
		//{
		//	isRightQuadrant = true;
		//}
		//else
		//{
		//	isRightQuadrant = false;
		//}
	}
	int ofX, ofY;
	ofX = ( offsetY < 0 ? -1 : ( offsetY > 0 ? 1 : 0 ) );
	ofY = ( offsetX > 0 ? 1 : ( offsetX < 0 ? -1 : 0 ) );
	if ( offsetY > 0 || offsetX > 0 )
	{
		int temp = ofX;
		ofX = ofY;
		ofY = temp;
	}
	LOShelperHelper( canSee, obstructionX, obstructionY, ofX, ofY,
		mapWidth, mapHeight, vert, horz, originX, originY, maxRange );
}
void Unit::LOShelperHelper( bool * canSee, int originX, int originY, int offsetX, int offsetY,
	int mapWidth, int mapHeight, bool isVertical, bool isHorizontal, int beginX, int beginY, int maxRange )
{
	// base case
	if ( originX < 0 || originX >= mapWidth || originY < 0 || originY >= mapHeight
		|| Tilemap::gridDistance( { originX, originY }, { beginX, beginY } ) > maxRange ) return;

	// call recursion
	LOShelperHelper( canSee, originX + offsetX, originY + offsetY, offsetX, offsetY,
		mapWidth, mapHeight, isVertical, isHorizontal, beginX, beginY, maxRange );

	// do work
	if ( isVertical )
	{
		for ( int newY = originY; newY >= 0 && newY < mapHeight;
			newY += offsetY == 0 ? offsetX : offsetY )
		{
			canSee[originX + ( newY * mapWidth )] = false;
		}
	}
	if ( isHorizontal )
	{
		for ( int newX = originX; newX >= 0 && newX < mapWidth;
			newX += offsetX == 0 ? offsetY : offsetX )
		{
			canSee[newX + ( originY * mapWidth )] = false;
		}
	}
	
	canSee[originX + ( originY * mapWidth )] = false;
}

Unit::dItem* * Unit::vec = NULL;
void Unit::dijkstraHelper( Point neighbor, int currentIdx, dItem* data, 
	std::vector<int>* toEvaluate, Tilemap * map, OccupiedStatus * occupied, OccupiedStatus self )
{
	//Point neighbor = { current.x + 1, current.y };
	int neighborIdx = map->getInt( neighbor );
	if ( map->isInBounds( neighbor ) && data[neighborIdx].shouldEvaluate )
	{
		int dist = data[currentIdx].distance +
			myClass.getTerrainCost( map->getTile( map->getPoint( neighborIdx ) )->getTerrain() )
			+ getElevationCost( map->getElevation( currentIdx ), map->getElevation( neighborIdx ) );
		if ( !data[neighborIdx].wasAdded )
		{
			// add &neighbor to toEvaluate
			data[neighborIdx].wasAdded = true;
			if ( occupied[neighborIdx] == NOT_OCCUPIED || occupied[neighborIdx] == self )
				data[neighborIdx].distance = dist;
			toEvaluate->push_back( neighborIdx );
		}
		else if ( dist < data[neighborIdx].distance )
		{
			if ( occupied[neighborIdx] == NOT_OCCUPIED || occupied[neighborIdx] == self )
				data[neighborIdx].distance = dist;
		}
	}
}
void Unit::dijkstraHelper2( Point neighbor, int currentIdx, dItem* data,
	std::vector<int>* toEvaluate, Tilemap * map )
{
	int neighborIdx = map->getInt( neighbor );
	if ( map->isInBounds( neighbor ) && data[neighborIdx].shouldEvaluate )
	{
		int dist = data[currentIdx].distance + 
			( map->getTile( map->getPoint( neighborIdx ) )->getTerrain() == TERRAIN_IMPASSABLE ? 100 : 1 );
		if ( !data[neighborIdx].wasAdded )
		{
			// add &neighbor to toEvaluate
			data[neighborIdx].wasAdded = true;
			data[neighborIdx].distance = dist;
			toEvaluate->push_back( neighborIdx );
		}
		else if ( dist < data[neighborIdx].distance )
		{
			data[neighborIdx].distance = dist;
		}
	}
}
int Unit::compare( const void* p1, const void* p2 )
{
	int d1 = ( *vec )[*( (int*)p1 )].distance;
	int d2 = ( *vec )[*( (int*)p2 )].distance;

	if ( d1 > d2 )
	{
		return -1;
	}
	else if ( d1 < d2 )
	{
		return 1;
	}
	return 0;
}
void Unit::getWeaponRange( Point pos, int minRange, int maxRange, bool * canHit, Tilemap * map )
{
	// Dijkstra's
	int mapSize = map->getMapWidth() * map->getMapHeight();
	int mapWidth = map->getMapWidth();
	int mapHeight = map->getMapHeight();

	// init arrays
	std::vector<int> toEvaluate;
	dItem* data = new dItem[mapSize];
	vec = &data;

	int currentIdx = map->getInt( pos );
	data[currentIdx].distance = 0;
	toEvaluate.push_back( currentIdx );
	data[currentIdx].wasAdded = true;


	while ( !toEvaluate.empty() )
	{
		currentIdx = toEvaluate[toEvaluate.size() - 1]; // get current item
		toEvaluate.pop_back(); // remove current from heap
		if ( Tilemap::gridDistance( map->getPoint( currentIdx ), pos ) > maxRange ) continue;

		data[currentIdx].shouldEvaluate = false;
		Point current = map->getPoint( currentIdx );

		// go through all neighbors
		dijkstraHelper2( { current.x + 1, current.y }, currentIdx, data, &toEvaluate, map );
		dijkstraHelper2( { current.x - 1, current.y }, currentIdx, data, &toEvaluate, map );
		dijkstraHelper2( { current.x, current.y + 1 }, currentIdx, data, &toEvaluate, map );
		dijkstraHelper2( { current.x, current.y - 1 }, currentIdx, data, &toEvaluate, map );


		// now sort!
		if ( !toEvaluate.empty() )
		{
			qsort( &toEvaluate[0], toEvaluate.size(), sizeof( int ), Unit::compare );
		}
	}

	for ( int i = 0; i < mapSize; i++ )
	{
		if ( data[i].distance <= maxRange && data[i].distance >= minRange )
		{
			//movement.push_back( map->getPoint( i ) );
			canHit[i] = true;
		}
	}
}
int Unit::getElevationCost( int start, int destination )
{
	int ret = 1; // see below

	if ( start > destination ) // going down
	{
		ret = start - destination;
	}
	else if ( destination > start ) // going up
	{
		ret = ( destination - start ) * 2;
	}
	// else no cost

	// so we're subtracting one here because movement is always at least one
	// if we didn't, it would cost 3 minimum to move up one, when it should be 2 minimum for
	// one-move tiles
	return ret - 1;
}