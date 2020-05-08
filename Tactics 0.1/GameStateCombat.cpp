#include "GameStateCombat.h"
#include "GameStateLevelUp.h"
#include "GameStateMap.h"
#include "GameStateEnemy.h"
#include "Utility.h"

GameStateCombat::GameStateCombat( GameData & gameData, CommonSprites& commonSprites, 
	int attackingAllyIndex, int attackingEnemyIndex, bool wasInitiatedByPlayer )
	:
	GameState( gameData, commonSprites )
{
	idxAttackingAlly = attackingAllyIndex;
	idxAttackingEnemy = attackingEnemyIndex;
	initiatedByPlayer = wasInitiatedByPlayer;

	g.selectedUnit = attackingAllyIndex;

	//g.selectedUnit = -1;
	animCounter = 0;
	//attackAnim.setFrame( 0 );

	int griddist = Tilemap::gridDistance( g.playerUnits[idxAttackingAlly]->getPosition(), 
		g.enemyUnits[idxAttackingEnemy]->getPosition() );
	bool enemyCanAttack = false;
	if ( griddist >= g.enemyUnits[idxAttackingEnemy]->getAttackRangeMin()
		&& griddist <= g.enemyUnits[idxAttackingEnemy]->getAttackRangeMax() )
	{
		enemyCanAttack = true;
	}
	// double attack
	int doubleNumber = 4; // have to have speed difference >= to double
	int attackDiff = g.playerUnits[idxAttackingAlly]->getSpeed() - g.enemyUnits[idxAttackingEnemy]->getSpeed();
	if ( attackDiff >= doubleNumber ) // ally doubles
	{
		attackIsAlly.push_back( true );
	}
	else if ( attackDiff <= -doubleNumber && enemyCanAttack ) // enemy doubles
	{
		attackIsAlly.push_back( false );
	}

	// attacks second
	if ( enemyCanAttack )
	{
		attackIsAlly.push_back( false );
	}
	// attacks first
	attackIsAlly.push_back( true );

	doCombatAndAnim();
}

GameStateCombat::~GameStateCombat()
{
	// ~~~
}


void GameStateCombat::update()
{
	if ( attackIsAlly.empty() ) // end attack
	{
		// weapon breaking
		if ( g.playerUnits[idxAttackingAlly]->getWeapon()->currentUses <= 0 )
		{
			g.playerUnits[idxAttackingAlly]->removeItem( 0 );
		}
		if ( g.enemyUnits[idxAttackingEnemy]->getWeapon()->currentUses <= 0 )
		{
			g.enemyUnits[idxAttackingEnemy]->removeItem( 0 );
		}

		// death
		if ( g.enemyUnits[idxAttackingEnemy]->getCurrentHP() <= 0 )
		{
			removeUnit( idxAttackingEnemy, false );
		}

		if ( g.playerUnits[idxAttackingAlly]->getCurrentHP() <= 0 )
		{
			removeUnit( idxAttackingAlly, true );

			if ( initiatedByPlayer )
			{
				g.selectedUnit = -1;
				g.gameState = new GameStateMap( g, s );
				delete this;
				return;
			}
			else // initiated by enemy
			{
				g.selectedUnit = -1;
				g.gameState = new GameStateEnemy( g, s );
				delete this;
				return;
			}
		}
		else
		{
			// not dead; recalc range
			g.playerUnits[idxAttackingAlly]->calculateMovement( &g.tMap, g.occupied );

			if ( initiatedByPlayer )
			{
				g.gameState = new GameStateLevelUp( g, s, g.gfx.getLastTexture(), 30, true );
				delete this;
				return;
			}
			else // initiated by enemy
			{
				g.gameState = new GameStateLevelUp( g, s, g.gfx.getLastTexture(), 30, false );
				delete this;
				return;
			}
		}
	}
	else
	{
		allyAnim->update();
		enemyAnim->update();
		animCounter++;
		if ( attackIsAlly[attackIsAlly.size() - 1] ) // ally attacking
		{
			int animFr = allyAnim->getNumFrames() * allyAnim->getStep();
			if ( animCounter == animFr - 8 )
			{
				g.enemyUnits[idxAttackingEnemy]->setCurrentHP
					( (int)fmax( 0, g.enemyUnits[idxAttackingEnemy]->getCurrentHP() - atkReport.damage ) );
			}
			if ( animCounter == animFr )
			{
				checkWeaponUses();
				//handleCombat( playerUnits[idxAttackingAlly], enemyUnits[idxAttackingEnemy] );
				animCounter = 0;
				attackIsAlly.pop_back();
				if ( g.enemyUnits[idxAttackingEnemy]->getCurrentHP() <= 0 )
				{
					attackIsAlly.clear();
				}
				else
				{
					doCombatAndAnim();
				}
			}
		}
		else // enemy attacking
		{
			int animFr = enemyAnim->getNumFrames() * enemyAnim->getStep();
			if ( animCounter == animFr - 8 )
			{
				g.playerUnits[idxAttackingAlly]->setCurrentHP
					( (int)fmax( 0, g.playerUnits[idxAttackingAlly]->getCurrentHP() - atkReport.damage ) );
			}
			if ( animCounter == animFr )
			{
				checkWeaponUses();
				//handleCombat( enemyUnits[idxAttackingEnemy], playerUnits[idxAttackingAlly] );
				animCounter = 0;
				attackIsAlly.pop_back();
				if ( g.playerUnits[idxAttackingAlly]->getCurrentHP() <= 0 )
				{
					attackIsAlly.clear();
				}
				else
				{
					doCombatAndAnim();
				}
			}
		}
	}
}
void GameStateCombat::drawToScreen()
{
	g.gfx.drawFixed( &s.attackBackground, 0, 0 );

	g.gfx.drawFixed( allyAnim, 200, 250, 255, 255, 255, 255, true );
	g.gfx.drawFixed( enemyAnim, 700, 250 );
	
	std::string str;
	g.gfx.drawFixed( g.playerUnits[idxAttackingAlly]->getName(), &s.menuFont, 10, 10, 10, 10, 10 );
	str = "HP: " + std::to_string( g.playerUnits[idxAttackingAlly]->getCurrentHP() )
		+ "/" + std::to_string( g.playerUnits[idxAttackingAlly]->getMaxHP() );
	g.gfx.drawFixed( str, &s.menuFont, 70, 610, 10, 10, 10 );
	str = "ATK: " + std::to_string( g.playerUnits[idxAttackingAlly]->getAttack() );
	g.gfx.drawFixed( str, &s.menuFont, 250, 610, 10, 10, 10 );

	g.gfx.drawFixed( g.enemyUnits[idxAttackingEnemy]->getName(), &s.menuFont, 1100, 10, 10, 10, 10 );
	str = "HP: " + std::to_string( g.enemyUnits[idxAttackingEnemy]->getCurrentHP() )
		+ "/" + std::to_string( g.enemyUnits[idxAttackingEnemy]->getMaxHP() );
	g.gfx.drawFixed( str, &s.menuFont, 750, 610, 10, 10, 10 );
	str = "ATK: " + std::to_string( g.enemyUnits[idxAttackingEnemy]->getAttack() );
	g.gfx.drawFixed( str, &s.menuFont, 1000, 610, 10, 10, 10 );
}


void GameStateCombat::doCombatAndAnim()
{
	if ( attackIsAlly.empty() ) return;
	if ( attackIsAlly[attackIsAlly.size() - 1] )
	{
		handleCombat( g.playerUnits[idxAttackingAlly], g.enemyUnits[idxAttackingEnemy] );
		if ( atkReport.wasCrit )
		{
			allyAnim = &s.attackCritAlly;
		}
		else
		{
			allyAnim = &s.attackAnimAlly;
		}

		if ( atkReport.wasMiss )
		{
			enemyAnim = &s.attackDodgeEnemy;
		}
		else
		{
			enemyAnim = &s.attackStandEnemy;
		}
	}
	else
	{
		handleCombat( g.enemyUnits[idxAttackingEnemy], g.playerUnits[idxAttackingAlly] );
		if ( atkReport.wasCrit )
		{
			enemyAnim = &s.attackCritEnemy;
		}
		else
		{
			enemyAnim = &s.attackAnimEnemy;
		}

		if ( atkReport.wasMiss )
		{
			allyAnim = &s.attackDodgeAlly;
		}
		else
		{
			allyAnim = &s.attackStandAlly;
		}
	}
}
void GameStateCombat::handleCombat( Unit * attacker, Unit * defender )
{
	int damage, hit, crit;
	getAttackStats( attacker, defender, damage, hit, crit );

	atkReport.wasCrit = false;
	atkReport.wasMiss = getDoubleRN() >= hit;
	if ( !atkReport.wasMiss )
	{
		atkReport.damage = damage;
		atkReport.wasCrit = getRN() < crit;
		if ( atkReport.wasCrit )
		{
			atkReport.damage *= 3;
		}
		attacker->getWeapon()->currentUses--;
	}
	else
	{
		atkReport.damage = 0;
		if ( Utility::isGun( attacker->getWeapon()->weaponType ) )
		{
			attacker->getWeapon()->currentUses--;
		}
	}
}

void GameStateCombat::checkWeaponUses()
{
	//handleCombat( g.playerUnits[idxAttackingAlly], g.enemyUnits[idxAttackingEnemy] );
	if ( g.playerUnits[idxAttackingAlly]->getWeapon()->currentUses <= 0 )
	{
		for ( int i = attackIsAlly.size() - 1; i >= 0; i-- )
		{
			if ( attackIsAlly[i] )
			{
				attackIsAlly.erase( attackIsAlly.begin() + i, attackIsAlly.begin() + i + 1 ); // remove ally attack
				i--;
			}
		}
	}

	if ( g.enemyUnits[idxAttackingEnemy]->getWeapon()->currentUses <= 0 )
	{
		for ( int i = attackIsAlly.size() - 1; i >= 0; i-- )
		{
			if ( !attackIsAlly[i] )
			{
				attackIsAlly.erase( attackIsAlly.begin() + i, attackIsAlly.begin() + i + 1 ); // remove enemy attack
				i--;
			}
		}
	}
}