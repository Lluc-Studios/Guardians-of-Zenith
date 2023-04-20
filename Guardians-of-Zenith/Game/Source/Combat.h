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
		WIN,
		LOSE,
		NONE
	};

	//Target: 0 = single, 1 = multiple, 2 = owner, 3 = ally, 4 = party, 5 = all

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

	void RemoveEntityFromList(int id);

	void LoadLaurea(Player::Laurea laurea);
	void LoadLapis(Player::Lapis lapis);
	void LoadLucca(Player::Lucca lucca);
	void LoadEnemy(EntityManager::CombatEnemy enemy);

public:

	COMBATMENU option = COMBATMENU::ATTACK;

	int AttackSelected = 0;
	int cd = 0;

	SDL_Texture* BG;

	//Preset for enemy teams
	int Preset = 0;

	//Class types: 1 = Tank, 2 = Mage, 3 = Archer
	//Decides whose turn is it, 0 = none, 1 = ally, 2 = enemy
	bool TeamTurn = 1;

	bool AttackMenu = false;
	bool EnemySelect = false;

	bool EnemyAdone = false;

	const char* Attack1;
	const char* Attack2;
	const char* Attack3;
	const char* Attack4;
	const char* Attack5;
	const char* Attack6;

	//Print enemy attack
	const char* Cname = "a";
	const char* Ename = "a";
	const char* Aname = "a";

private:

	int WhiteFading = 1;
	bool Fading = false;

	SDL_Texture* Character1;
	SDL_Texture* Character2;
	SDL_Texture* Character3;
	SDL_Texture* Character1Frozen;
	SDL_Texture* Character2Frozen;
	SDL_Texture* Character3Frozen;
	SDL_Texture* Enemy1;
	SDL_Texture* Enemy2;
	SDL_Texture* Enemy3;
	SDL_Texture* ClassTank;
	SDL_Texture* ClassMage;
	SDL_Texture* ClassArcher;
	SDL_Texture* ClassChart;

	//Entities status effects
	SDL_Texture* Frozen;
	SDL_Texture* Burn;
	SDL_Texture* BurnBlue;
	SDL_Texture* Poison;
	SDL_Texture* Bleed;
	SDL_Texture* Electrified;

	//Character1
	float C1MHP = 1000, C1CHP = 900;
	float C1MMP = 250, C1CMP = 200;
	float C1ATK, C1DEF,LIMIT1, limitCount1;
	float C1speed = 0;
	const char* C1NAME;
	float C1lvl;
	int C1class = 1;

	float C1A1lvl = 1, C1A1target = 0, C1A1dmg = 20, C1A1mp = 0;
	const char* C1A1name = "Sword attack";

	float C1A2lvl = 1, C1A2target = 0, C1A2dmg = 40, C1A2mp = 15;
	const char* C1A2name = "Thrust";

	float C1A3lvl = 1, C1A3target = 0, C1A3dmg = 40, C1A3mp = 50;
	const char* C1A3name = "Calibur";

	bool C1Block = false, C1dead = false, C1Check = false, C1FROZEN = false;
	int C1POISON = 0;

	//Character2
	float C2MHP = 1000, C2CHP = 320;
	float C2MMP = 250, C2CMP = 100;
	float C2ATK, C2DEF, LIMIT2, limitCount2;
	float C2speed = 0;
	const char* C2NAME;
	float C2lvl;
	int C2class = 2;

	float C2A1lvl = 1, C2A1target = 0, C2A1dmg = 20, C2A1mp = 0;
	const char* C2A1name = "Staff attack";

	float C2A2lvl = 1, C2A2target = 1, C2A2dmg = 30, C2A2mp = 25;
	const char* C2A2name = "Tidalwave";

	float C2A3lvl = 1, C2A3target = 5, C2A3dmg = 40, C2A3mp = 100;
	const char* C2A3name = "Aqua burst";

	bool C2Block = false, C2dead = false, C2Check = false, C2FROZEN = false;
	int C2POISON = 0;

	//Character3
	float C3MHP = 1000, C3CHP = 500;
	float C3MMP = 250, C3CMP = 250;
	float C3ATK, C3DEF, LIMIT3, limitCount3;
	float C3speed = 0;
	const char* C3NAME;
	float C3lvl;
	int C3class = 3;

	float C3A1lvl = 1, C3A1target = 0, C3A1dmg = 20, C3A1mp = 0;
	const char* C3A1name = "Arrow shot";

	float C3A2lvl = 1, C3A2target = 1, C3A2dmg = 35, C3A2mp = 20;
	const char* C3A2name = "Multishot";

	bool C3Block = false, C3dead = false, C3Check = false, C3FROZEN = false;
	int C3POISON = 0;

	//Enemy1
	float E1MHP = 1000, E1CHP = 800, E1MES = 0, E1CES = 0;
	float E1ATK, E1DEF, E1EXP;
	float E1speed = 0;
	int E1class = 1, E1BOSS = 0;
	const char* E1Weak;
	const char* E1Res;
	const char* E1name;
	const char* E1A1name;
	float E1A1dmg, E1A1target, E1A1effect;

	SDL_Texture* E1asset;

	const char* E1A2name;
	float E1A2dmg, E1A2target, E1A2effect;

	const char* E1A3name;
	float E1A3dmg, E1A3target, E1A3effect;

	const char* E1A4name;
	float E1A4dmg, E1A4target, E1A4effect;

	bool E1dead = false, E1Check = false;

	//Enemy2
	float E2MHP = 1000, E2CHP = 1000, E2MES = 0, E2CES = 0;
	float E2ATK, E2DEF, E2EXP;
	float E2speed = 0;
	int E2class = 1, E2BOSS = 0;
	const char* E2Weak;
	const char* E2Res;
	const char* E2name;
	const char* E2A1name;
	float E2A1dmg, E2A1target, E2A1effect;

	SDL_Texture* E2asset;

	const char* E2A2name;
	float E2A2dmg, E2A2target, E2A2effect;

	const char* E2A3name;
	float E2A3dmg, E2A3target, E2A3effect;

	const char* E2A4name;
	float E2A4dmg, E2A4target, E2A4effect;

	bool E2dead = false, E2Check = false;

	//Enemy3
	float E3MHP = 1000, E3CHP = 150, E3MES = 0, E3CES = 0;
	float E3ATK, E3DEF, E3EXP;
	float E3speed = 0;
	int E3class = 1, E3BOSS = 0;
	const char* E3Weak;
	const char* E3Res;
	const char* E3name;
	const char* E3A1name;
	float E3A1dmg, E3A1target, E3A1effect;

	SDL_Texture* E3asset;

	const char* E3A2name;
	float E3A2dmg, E3A2target, E3A2effect;

	const char* E3A3name;
	float E3A3dmg, E3A3target, E3A3effect;

	const char* E3A4name;
	float E3A4dmg, E3A4target, E3A4effect;

	bool E3dead = false, E3Check = false;

	//Other variables

	int idCount = 3, offset = 0;

	int SaveInstance = 0;
	
	SDL_Color WF = { 255, 255, 255 };
	SDL_Color GF = { 200, 200, 200 };
	SDL_Color YF = { 255, 255, 80 };

	bool InCombat = false;

	int CurrentCharacters = 0, CurrentEnemies = 0;

	int EXPwon = 0;

	int charactersLoaded = 0;
	// 1-3 = ally, 4-6 = enemy
	int Turn[6] = { 1, 4, 5, 2, 6, 3 };

	//To prevent memory leaks (This may cause problems so it must be checked in case something does not work right)

	char Aux[10], Aux1[10], Aux2[10];

	int HpBarLengthC1 = 0;
	int MpBarLengthC1 = 0;
	int HpBarLengthC2 = 0;
	int MpBarLengthC2 = 0;
	int HpBarLengthC3 = 0;
	int MpBarLengthC3 = 0;

	int HpBarLengthE1 = 0;
	int EsBarLengthE1 = 0;
	int HpBarLengthE2 = 0;
	int HpBarLengthE3 = 0;

	SDL_Texture* E1;
	SDL_Texture* E2;
	SDL_Texture* E3;

	int TurnPos = 0, auxiliarPos = 0;

	const char* M1 = "0";
	const char* M2 = "0";
	const char* M3 = "0";

	int block = 1,multiplier = 1, multiplier1 = 1, multiplier2 = 1;

	int EnemyAttackTarget = 0, EnemyAttackNum = 0;

	bool FinishedTurn1 = false;
	bool FinishedTurn2 = false;
	bool FinishedTurn3 = false;
};

#endif // __COMBAT_H__
