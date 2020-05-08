#include "GameStateSelectAttack.h"

class GameStateSelectHeal : public GameStateSelectAttack
{
public:
	GameStateSelectHeal( GameData & gameData, CommonSprites& commonSprites, Point movePosition, Point mousePosition );
	virtual ~GameStateSelectHeal();

	void update() override;
	void drawToScreen() override;

private:

};