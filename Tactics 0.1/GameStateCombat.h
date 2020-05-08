#pragma once
#include "GameState.h"

class GameStateCombat : public GameState
{
public:
	GameStateCombat( GameData & gameData, CommonSprites& commonSprites, 
		int attackingAllyIndex, int attackingEnemyIndex, bool wasInitiatedByPlayer = true );
	virtual ~GameStateCombat();

	void update() override;
	void drawToScreen() override;

private:
	struct AttackReport
	{
		int damage;
		bool wasMiss;
		bool wasCrit;
	};

	int idxAttackingAlly;
	int idxAttackingEnemy;
	int animCounter;
	std::vector<bool> attackIsAlly;
	AttackReport atkReport;
	bool initiatedByPlayer;

	AnimatedSprite * allyAnim;
	AnimatedSprite * enemyAnim;

	// utility functions
	void doCombatAndAnim();
	void handleCombat( Unit * attacker, Unit * defender );
	void checkWeaponUses();
};