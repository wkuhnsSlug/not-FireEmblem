#pragma once
#include "GameState.h"

class GameStateMap : public GameState
{
public:
	GameStateMap( GameData & gameData, CommonSprites& commonSprites );
	virtual ~GameStateMap();

	void update() override;
	void drawToScreen() override;

private:

};