#include "Screen.h"
#include <string>
#include "Viewport.h"
#include "Unit.h"
#include "LevelDefines.h"
#include "GameState.h"
#include <vector>

class ScreenLevel : public Screen
{
public:
	ScreenLevel( std::string filename, KeyboardBase& Kbds, MouseInterface& m, GraphicsInterface& Gfx, Screen ** ptr );
	~ScreenLevel();

	bool logic() override;
	void render() override;

private:
	Point viewportPos;
	bool isPlayerTurn;

	GameData gameData;
	CommonSprites commonSprites;

	Tilemap tMap;
	std::vector<Unit*> playerUnits;
	std::vector<Unit*> enemyUnits;
	OccupiedStatus * occupied;

	void loadUnits( std::vector<Unit*>& units, std::string filename, OccupiedStatus occ );
};