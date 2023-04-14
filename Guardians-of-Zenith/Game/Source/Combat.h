#ifndef __COMBAT_H__
#define __COMBAT_H__

#include "Module.h"
#include "Player.h"
#include "Item.h"
#include "Enemy.h"


class Combat : public Module
{
public:

	enum class COMBATMENU {
		ATTACK = 1,
		DEFEND,
		INVENTORY,
		ESCAPE
	};

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

	void LoadLaurea(Laurea laurea);
	void LoadLapis(Lapis lapis);
	void LoadLucca(Lucca lucca);
	void LoadEnemy(CombatEnemy::stats enemy);

public:

	COMBATMENU option = COMBATMENU::ATTACK;

	SDL_Texture* BG;

private:

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

	//Character2
	int C2MHP = 1000, C2CHP = 320;
	int C2MMP = 250, C2CMP = 100;
	int C2ATK, C2DEF, LIMIT2;
	int C2speed;

	//Character3
	int C3MHP = 1000, C3CHP = 500;
	int C3MMP = 250, C3CMP = 250;
	int C3ATK, C3DEF, LIMIT3;
	int C3speed;

	//Enemy1
	int E1MHP = 1000, E1CHP = 800;
	int E1ATK, E1DEF, E1EXP;
	int E1speed;
	const char* E1Weak;
	const char* E1Res;

	//Enemy1
	int E2MHP = 1000, E2CHP = 1000;
	int E2ATK, E2DEF, E2EXP;
	int E2speed;
	const char* E2Weak;
	const char* E2Res;

	//Enemy1
	int E3MHP = 1000, E3CHP = 150;
	int E3ATK, E3DEF, E3EXP;
	int E3speed;
	const char* E3Weak;
	const char* E3Res;

	int idCount = 3;

	int SaveInstance = 0, WF, GF;
	bool InCombat = false;

	int CurrentCharacters = 0, CurrentEnemies = 0;

	// 1-3 = ally, 4-6 = enemy
	int Turn[6] = { 1, 4, 5, 2, 6, 3 };
};

#endif // __COMBAT_H__
