#ifndef __COMBAT_H__
#define __COMBAT_H__

#include "Module.h"
#include "Player.h"
#include "Item.h"
#include"EntityManager.h"


class Combat : public Module
{
public:

	enum class COMBATMENU {
		ATTACK = 1,
		DEFEND,
		INVENTORY,
		ESCAPE,
		ATTACK1,
		ATTACK2,
		ATTACK3,
		ATTACK4,
		ATTACK5,
		ATTACK6,
		ENEMY1,
		ENEMY2,
		ENEMY3,
		NONE
	};

	//Target: 0 = single, 1 = multiple, 2 = owner, 3 = ally, 4 = party

	Combat();

	// Destructor
	virtual ~Combat();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void StartCombat();

	void ExitCombat();

	void FinishTurn();

	void TurnOrder();

	void PerformAction();

	void LoadLaurea(Player::Laurea laurea);
	void LoadLapis(Player::Lapis lapis);
	void LoadLucca(Player::Lucca lucca);
	void LoadEnemy(EntityManager::CombatEnemy enemy);

public:
	//Temporal variable
	int count = 0;

	COMBATMENU option = COMBATMENU::ATTACK;

	int AttackSelected = 0;
	int cd = 0;

	SDL_Texture* BG;

	//Decides whose turn is it, 0 = none, 1 = ally, 2 = enemy
	bool TeamTurn = 1;

	bool AttackMenu = false;
	bool EnemySelect = false;

	const char* Attack1;
	const char* Attack2;
	const char* Attack3;
	const char* Attack4;
	const char* Attack5;
	const char* Attack6;

private:

	int WhiteFading = 1;
	bool Fading = false;

	SDL_Texture* Character1;
	SDL_Texture* Character2;
	SDL_Texture* Character3;
	SDL_Texture* Enemy1;
	SDL_Texture* Enemy2;
	SDL_Texture* Enemy3;

	//Character1
	int C1MHP = 1000, C1CHP = 900;
	int C1MMP = 250, C1CMP = 200;
	int C1ATK, C1DEF,LIMIT1;
	int C1speed;
	const char* C1NAME;
	int C1lvl;

	int C1A1lvl = 1, C1A1target = 0, C1A1dmg = 20, C1A1mp = 0;
	const char* C1A1name = "swordattack";

	int C1A2lvl = 1, C1A2target = 0, C1A2dmg = 40, C1A2mp = 15;
	const char* C1A2name = "thrust";

	//Character2
	int C2MHP = 1000, C2CHP = 320;
	int C2MMP = 250, C2CMP = 100;
	int C2ATK, C2DEF, LIMIT2;
	int C2speed;
	const char* C2NAME;
	int C2lvl;

	int C2A1lvl = 1, C2A1target = 0, C2A1dmg = 20, C2A1mp = 0;
	const char* C2A1name = "staffattack";

	int C2A2lvl = 1, C2A2target = 1, C2A2dmg = 30, C2A2mp = 25;
	const char* C2A2name = "tidalwave";

	//Character3
	int C3MHP = 1000, C3CHP = 500;
	int C3MMP = 250, C3CMP = 250;
	int C3ATK, C3DEF, LIMIT3;
	int C3speed;
	const char* C3NAME;
	int C3lvl;

	int C3A1lvl = 1, C3A1target = 0, C3A1dmg = 20, C3A1mp = 0;
	const char* C3A1name = "arrowshot";

	int C3A2lvl = 1, C3A2target = 1, C3A2dmg = 35, C3A2mp = 20;
	const char* C3A2name = "multishot";


	//Enemy1
	int E1MHP = 1000, E1CHP = 800;
	int E1ATK, E1DEF, E1EXP;
	int E1speed;
	const char* E1Weak;
	const char* E1Res;
	const char* E1name;
	const char* E1A1name;
	int E1A1dmg, E1A1target;

	const char* E1A2name;
	int E1A2dmg, E1A2target;

	const char* E1A3name;
	int E1A3dmg, E1A3target;

	const char* E1A4name;
	int E1A4dmg, E1A4target;


	//Enemy2
	int E2MHP = 1000, E2CHP = 1000;
	int E2ATK, E2DEF, E2EXP;
	int E2speed;
	const char* E2Weak;
	const char* E2Res;
	const char* E2name;
	const char* E2A1name;
	int E2A1dmg, E2A1target;

	const char* E2A2name;
	int E2A2dmg, E2A2target;

	const char* E2A3name;
	int E2A3dmg, E2A3target;

	const char* E2A4name;
	int E2A4dmg, E2A4target;

	//Enemy3
	int E3MHP = 1000, E3CHP = 150;
	int E3ATK, E3DEF, E3EXP;
	int E3speed;
	const char* E3Weak;
	const char* E3Res;
	const char* E3name;
	const char* E3A1name;
	int E3A1dmg, E3A1target;

	const char* E3A2name;
	int E3A2dmg, E3A2target;

	const char* E3A3name;
	int E3A3dmg, E3A3target;

	const char* E3A4name;
	int E3A4dmg, E3A4target;



	int idCount = 3;

	int SaveInstance = 0, WF, GF;
	bool InCombat = false;

	int CurrentCharacters = 0, CurrentEnemies = 0;


	// 1-3 = ally, 4-6 = enemy
	int Turn[6] = { 1, 4, 5, 2, 6, 3 };
};

#endif // __COMBAT_H__
