#include "GameState.h"
#include "Sprite.h"

class GameStateLevelUp : public GameState
{
public:
	GameStateLevelUp( GameData & gameData, CommonSprites& commonSprites,
		Sprite * spr, int expGained, bool isPlayer = true );
	virtual ~GameStateLevelUp();

	void update() override;
	void drawToScreen() override;

private:
	Sprite * background;
	int counter;
	int exp;
	int levelUp;
	bool isPlayerTurn;
};