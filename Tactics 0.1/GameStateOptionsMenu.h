#pragma once
#include "GameState.h"

class GameStateOptionsMenu : public GameState
{
public:
	GameStateOptionsMenu( GameData & gameData, CommonSprites& commonSprites );
	virtual ~GameStateOptionsMenu();

	void update() override;
	void drawToScreen() override;

private:
	enum MenuOption
	{
		OPTION_END_TURN,
		OPTION_RETURN,
		N_MENU_OPTIONS
	};

	int menuIndex;
	RectD rect;
	int itemHeight;
	int leftClickIndex;

	void drawMenu();
	std::string getMenuOptionName( MenuOption option );
};