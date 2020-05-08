#pragma once

#include "Mouse.h"
#include "KeyboardBase.h"
#include "GraphicsInterface.h"
#include "Unit.h"
#include "AnimatedSprite.h"
#include "Tilemap.h"
#include <vector>

class GameState;
struct GameData
{
	GameData( Tilemap& tMap, std::vector<Unit*>& playerUnits,
		std::vector<Unit*>& enemyUnits, MouseInterface& mouse,
		GraphicsInterface& gfx, Viewport& viewport, KeyboardBase& keyboard )
		:
		tMap( tMap ),
		playerUnits( playerUnits ),
		enemyUnits( enemyUnits ),
		mouse( mouse ),
		gfx( gfx ),
		viewport( viewport ),
		kbd( keyboard )
	{
		selectedUnit = -1;
		shouldEndTurn = false;

		drawGrid = false;
		drawEnemyRange = false;
		drawElevation = false;
	}
	Unit* cur() { return playerUnits[selectedUnit]; }
	Tilemap& tMap;
	std::vector<Unit*>& playerUnits;
	std::vector<Unit*>& enemyUnits;
	
	int selectedUnit;
	OccupiedStatus * occupied;
	Point cursorpos;
	GameState * gameState;
	bool shouldEndTurn;

	MouseInterface& mouse;
	KeyboardBase& kbd;
	GraphicsInterface& gfx;
	Viewport& viewport;

	//toggles
	bool drawGrid;
	bool drawEnemyRange;
	bool drawElevation;
};

struct CommonSprites
{
	Font menuFont;
	Font menuFontSmall;
	Sprite mouseCursor;
	Sprite cursorRed;
	Sprite cursorBlue;
	Sprite blueHighlight;
	Sprite redHighlight;
	Sprite greenHighlight;
	Sprite menuBackgroundRed;
	Sprite menuBackgroundBlue;
	Sprite redSquare;
	Sprite battleForecastBackground;

	Sprite background;
	Sprite foreground;

	// for now
	AnimatedSprite attackAnimAlly;
	AnimatedSprite attackAnimEnemy;
	AnimatedSprite attackCritAlly;
	AnimatedSprite attackCritEnemy;
	AnimatedSprite attackStandAlly;
	AnimatedSprite attackStandEnemy;
	AnimatedSprite attackDodgeAlly;
	AnimatedSprite attackDodgeEnemy;
	Sprite attackBackground;
};

class GameState
{
public:
	GameState( GameData & gameData, CommonSprites& commonSprites );
	virtual ~GameState();

	virtual void update() = 0;
	virtual void drawToScreen() = 0;

	static Unit * getUnit( Point p, Tilemap * map,
		OccupiedStatus * occupied, std::vector<Unit*> & playerUnits );

protected:
	GameData& g;
	CommonSprites& s;

	// gfx functions
	virtual void drawHighlights( std::vector<Point>* v, Sprite* highlight, int opacity = 255 );
	virtual void drawEnemyRange();
	virtual void drawElevation();
	virtual void drawCharactersOnTilemap();
	virtual void drawSelectedUnit();
	virtual void drawCursor();
	virtual void drawMouse();
	virtual void drawUnitThumbnail( Unit * unit );
	virtual void drawTerrainInfo();
	virtual void drawGridlines();

	// utility functions
	void updateView();
	void updateCursorPosition();
	void updateStandardToggles();
	int getAllyUnitIndex( Point p );
	int getEnemyUnitIndex( Point p );
	int getAllyUnitActiveIndex( Point p );
	Unit * getUnit( Point p );
	Unit * getUnitActive( Point p );
	void moveUnit( Unit * unit, Point moveTo, bool isAlly );
	void setMoveRanges();
	void getWeaponTriangleBonuses( WeaponType attacker, WeaponType defender, int& atkBonus, int& avoBonus );
	void getAttackStats( Unit * attacker, Unit * defender, const Weapon * attackerWeapon, const Weapon * defenderWeapon,
		Tile * terrain, int& damage, int& hit, int& crit );
	void getAttackStats( Unit * attacker, Unit * defender, int& damage, int& hit, int& crit );
	int getWeaponTriangleEffectiveness( WeaponType attacker, WeaponType defender );
	void centerViewport( Point positionInGrid );
	int getRN();
	int getDoubleRN();
	void removeUnit( int index, bool isAlly );
	void setAllyRanges();
	void setEnemyRanges();
	bool pointIsInRange( Point p, const std::vector<Point> & vec );
};