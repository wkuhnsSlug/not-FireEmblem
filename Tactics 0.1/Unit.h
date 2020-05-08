#pragma once

#include "Extras.h"
#include "Sprite.h"
#include "Tilemap.h"
#include "LevelDefines.h"
#include "Weapon.h"
#include "Medicine.h"
#include "UnitClass.h"
#include <string>
#include <vector>

#define INVENTORY_SIZE 5

class Unit
{
public:
	Unit();
	Unit( std::string filename, int xPos, int yPos );
	~Unit();

	void init( std::string filename, int xPos, int yPos );
	void calculateMovement( Tilemap * map, OccupiedStatus * occupied,
		OccupiedStatus self = OCCUPIED_ALLY, int movementRemaining = -1 );
	void calculateAvailableActions( Point position, std::vector<Unit*>& enemyVector,
		std::vector<Unit*>& allyVector, Tilemap * map, OccupiedStatus * occupied );
	void calculateEnemiesInRange( Point position, std::vector<Unit*>& enemyVector );
	void calculateAttackRangeActive( Point position, Tilemap * map );
	void calculateAdjacentAllies( Point position, std::vector<Unit*>& allyVector );
	static void getGunRange( Point startPos, Weapon wep, Tilemap* map, std::vector<Point> & ret );

	bool isActive() const { return activeStatus; }
	void setActive( bool active );
	Unit * getRescuedUnit() { return rescuedUnit; }
	void setRescuedUnit( Unit * unit ) { rescuedUnit = unit; }
	bool isRescued() const { return isBeingRescued; }
	void setRescued( bool rescue ){ isBeingRescued = rescue; }

	int getAttack() const;
	int getHit() const;
	int getAvo() const;
	int getCrit() const;
	int getOverburden() const;
	int getAttackRangeMin() const;
	int getAttackRangeMax() const;
	bool canStand( TerrainType terrain );
	int getClassType() const { return myClass.getClassType(); }

	Point getPosition() const { return pos; }
	void setPosition( Point p ) { pos = p; }
	Sprite * getSprite() { return sprite; }
	std::vector<Point>* getMovement()			{ return &movement; }
	std::vector<Point>* getEnemiesInRange()		{ return &enemies; }
	std::vector<Point>* getAdjacentAllies()		{ return &adjacentAllies; }
	std::vector<Point>* getRescuableAllies()	{ return &rescuableAllies; }
	std::vector<Point>* getTransferableAllies()	{ return &transferableAllies; }
	std::vector<Point>* getDropRange()			{ return &dropRange; }
	std::vector<Point>* getAttackRange()		{ return &attackRange; }
	std::vector<Point>* getAttackRangeActive()	{ return &attackRangeActive; }
	std::vector<Point>* getHealRange()			{ return &healRange; }
	std::vector<Point>* getTradablePositions()	{ return &tradablePositions; }
	std::vector<Unit*>* getTradePartners()		{ return &potentialTradePartners; }
	std::vector<ActionMenuType>* getAvailableActions() { return &actionTypes; }

	std::string getName()	const { return name; }
	std::string getClassName() const { return myClass.getClassName(); }
	int getExp()			const { return currentEXP; }
	int getLevel()			const { return currentLevel; }
	int getMaxMovement()	const { return myClass.getMove(); }
	int getMaxHP()			const { return maxHP; }
	int getCurrentHP()		const { return currentHP; }
	int getStrength()		const { return strength; }
	int getDefense()		const { return defense; }
	int getSpeed()			const { return speed; }
	int getAccuracy()		const { return accuracy; }
	int getAgility()		const { return agility; }
	int getSkill()			const { return skill; }
	int getLuck()			const { return luck; }
	int getBuild()			const { return build; }
	int getAid() const;

	const Weapon * getWeapon() const;
	Weapon * getWeapon();
	bool canWield( const Weapon * weapon ) const;
	bool canWield( const Medicine * med ) const;
	WeaponRank getWeaponRank( WeaponType wepType ) const;
	Item ** getInventory() { return inventory; }
	int getInventorySize() const { return currentInventorySize; }
	void addItem( Item * item );
	void equipWeapon( int index );
	void removeItem( int index );
	Item * extractItem( int index );
	void consolidateInventory();

	int addExperience( int exp ); // returns number of levels added
	void increaseLevel( int inc ) { currentLevel = currentLevel + inc > 20 ? 20 : currentLevel + inc; }
	void setCurrentHP( int newHP ) { currentHP = newHP; }
	bool isMarked() const { return isCurrentlyMarked; }
	void setMarked( bool isMark ) { isCurrentlyMarked = isMark; }
	void setTraded() { hasTraded = true; }
	void setTransfered() { hasTransfered = true; }
	bool getHasTraded() const { return hasTraded; }

	// guess I didn't write these yet lol
	void mount();
	void dismount();

	static void getLOS( Point startPos, int minRange, int maxRange, Tilemap * map,
		std::vector<Point>& ret, bool includePoint = true );
	static void getLOS( Point startPos, int minRange, int maxRange, Tilemap * map,
		bool * canSee, bool includePoint = true );
	static void getWeaponRange( Point pos, Weapon * wep, Tilemap * map, std::vector<Point>& ret );
	static std::vector<Point> getWeaponRange( Point pos, Weapon * wep, Tilemap * map );

private:
	Point pos;
	Sprite * sprite;
	std::vector<Point> movement;
	std::vector<Point> enemies;
	std::vector<Point> adjacentAllies;
	std::vector<Point> rescuableAllies;
	std::vector<Point> transferableAllies;
	std::vector<Point> dropRange;
	std::vector<Point> attackRange;
	std::vector<Point> attackRangeActive;
	std::vector<Point> healRange;
	std::vector<Point> tradablePositions;
	std::vector<Unit*> potentialTradePartners;
	std::vector<ActionMenuType> actionTypes;
	bool activeStatus;
	bool isCurrentlyMarked;
	Unit * rescuedUnit;
	bool isBeingRescued;
	bool hasTraded;
	bool hasTransfered;

	Item* inventory[INVENTORY_SIZE];
	int currentInventorySize;

	UnitClass myClass; // this is not my preferred name convention
	std::string name;
	int currentEXP;
	int currentLevel;
	int maxHP;
	int currentHP;
	int strength;
	int defense;
	int speed;
	int accuracy;
	int agility;
	int skill;
	int luck;
	int build;
	WeaponRank wepRank[N_WEAPON_TYPES];

	struct dItem
	{
		int distance = 1000; // arbitrarily large value
		bool shouldEvaluate = true;
		bool wasAdded = false;
	};
	static dItem* * vec;
	static int compare( const void* p1, const void* p2 );
	void dijkstraHelper( Point neighbor, int currentIdx, dItem* data,
		std::vector<int>* toEvaluate, Tilemap * map, OccupiedStatus * occupied, OccupiedStatus self );
	static void dijkstraHelper2( Point neighbor, int currentIdx, dItem* data,
		std::vector<int>* toEvaluate, Tilemap * map );

	static void solveForAttackRange( std::vector<Point> positions, int startPos, int endPos, 
		Item** items, int itemLength, Tilemap * map, std::vector<Point>* ret, Unit * u ); // start inclusive, end exclusive
	static void LOShelper( bool * canSee, int originX, int originY, int obstructionX, int obstructionY,
		int mapWidth, int mapHeight, int maxRange );
	static void LOShelperHelper( bool * canSee, int originX, int originY, int offsetX, int offsetY, 
		int mapWidth, int mapHeight, bool isVertical, bool isHorizontal,
		int beginX, int beginY, int maxRange ); // recursion, wow!

	static void getWeaponRange( Point pos, int minRange, int maxRange, bool * canHit, Tilemap * map );
	int getElevationCost( int start, int destination );
};