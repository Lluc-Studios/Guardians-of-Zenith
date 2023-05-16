#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "Path.h"
#include "Physics.h"

#include "GuiButton.h"
#include "DialogueSystem.h"

struct SDL_Texture;

enum class State {
	COLLIDING,
	MOVING_RIGHT,
	MOVING_LEFT,
	ATTACKING,
	TELEPORTING,
	IDLE
};

class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	bool IsAlive(); 

	bool LoadState(pugi::xml_node& data);

	bool SaveState(pugi::xml_node& data);

	void InitializePlayers();
	
	void Move(); 

	void Attack(int frames);

	void debugKeys(); 

	void Death(); 

	void LevelToMax();

	void Teleport_Point(int Instance, int TELEPORT_POSITION[2]);

public:
	// stats
	struct Laurea
	{
		float hp, chp, mp, cmp, atk, def, spe, limit;
		float lvl, exp;
		const char* name = "Laurea";

		//Leveling variables
		float HPG = 4, MPG = 3, ATKG = 2, DEFG = 3;
		int HPC = 10, MPC = 5, ATKC = 5, DEFC = 5;
		float EXPneeded = 100;
	};
	struct Lapis
	{
		float hp, chp, mp, cmp, atk, def, spe, limit;
		float lvl, exp;
		const char* name = "Lapis";

		//Leveling variables
		float HPG = 3.5, MPG = 4, ATKG = 2.5, DEFG = 2;
		int HPC = 10, MPC = 5, ATKC = 5, DEFC = 5;
		float EXPneeded = 100;
	};
	struct Lucca
	{
		float hp, chp, mp, cmp, atk, def, spe, limit;
		float lvl, exp;
		const char* name = "Lucca";

		//Leveling variables
		float HPG = 3, MPG = 3, ATKG = 4, DEFG = 2;
		int HPC = 10, MPC = 5, ATKC = 5, DEFC = 5;
		float EXPneeded = 100;
	};

	//Laurea laurea{ 350,350,50,50,100,125,10,2,1,0 };
	//Lapis lapis{ 250,250,100,100,125,90,12,4,1,0 };
	//Lucca lucca{ 200,200,115,115,150,60,15,3,1,0 };

	Laurea laurea;
	Lapis lapis;
	Lucca lucca;

	int timer = 0;
	bool limitFPS = true;
	int PresetChance = 0, PresetVariation = 0;

	//dialogue
	bool isDialogue = false;
	bool isBill = false;
	bool NPC2 = false;
	bool NPC = false;
	bool buttonE = false;

	bool NPC_01 = false;
	bool NPC_02 = false;
	bool NPC_03 = false;
	bool NPC_04 = false;
	iPoint copypos;


	bool DeathAnimationFinished = false;

	PhysBody* pbody;
	PhysBody* attackHitbox;

	bool alive, invincible = false;

	int life = 3, lifeAux = 0, WF, YF;

	iPoint x;

	bool tp = false;
	bool tpHouse = false;

	int future_instance = 0;
	int tp_pos[2] = {0,0};

	int tp_savegame[2] = {0,0};


	int tp1[2] = { 210,-40};
	int tp2[2] = { 690,900 };
	int tp3[2] = { 240,-460};
	int tp4[2] = { 1745,910};
	int tp5[2] = { -330,330};
	int tp6[2] = { 560 ,320 };
	int tp7[2] = { 3450 ,420 };
	int tp8[2] = { 1010, -830 };
	int tp9[2] = { 1473,5648};
	int tp10[2] = { 1365, -1030};
	int tp11[2] = { -1250, 2150 };
	int tp12[2] = { 1715,-830 };
	int tp13[2] = { 1370,-80 };
	int tp14[2] = { 1010,-830 };

	int tpHouse_pos[2] = { -520,150 };

	bool lose = false; 

	//Uint32 TotalTime, Time = 0;

	int auxCounter = 0;
	bool auxBool = false;

private:

	SDL_Texture* texture;
	const char* texturePath;

	//Fx
	int Step1, Step2, StepMetalic1, StepMetalic2, DeathSound, Swing, DamageFx, aux, tpFX = -1;

	int width;
	int height;

	int dialogueID;

	//int attackCD = 0;
	//int attackFrames = 10; 

	State playerState; 

	b2Vec2 vel; 


	DIRECTION facing = DIRECTION::RIGHT;

	Animation* currentAnim = nullptr;

	Animation playerIdleR;
	Animation playerIdleL;
	Animation playerIdleUp;
	Animation playerIdleDown;

	Animation playerRunR;
	Animation playerRunL;
	Animation playerRunUp;
	Animation playerRunDown;

	Animation playerAttackR;
	Animation playerAttackL;
	Animation playerDie;

	SDL_Texture* LFHH;
	SDL_Texture* LFH;
	SDL_Texture* LFL;
	SDL_Texture* LFE;
	SDL_Texture* Dialogue;
	SDL_Texture* PressE;

	SDL_Rect rectDown = { 0,0,32,32 };
	SDL_Rect rectUp = { 32,0,32,32 };
	SDL_Rect rectLeft = { 32*2,0,32,32 };
	SDL_Rect rectRight = { 32*3,0,32,32 };

	SDL_Color WF2 = { 255, 0, 0 };

};
//struct Laurea
//{
//	int hp, chp, mp, cmp, atk, def, spe, limit;
//	int lvl = 1, hpGrowth, mpGrowth, atkGrowth, defGrowth;
//	Laurea(int hp1, int chp1, int mp1, int cmp1, int atk1, int def1, int spe1, int limit1, int hpGrowth1, int mpGrowth1,int atkGrowth1,int defGrowth1) {
//		hp = hp1; chp = chp1; mp = mp1; cmp = cmp1; atk = atk1; def = def1; spe = spe1; limit = limit1; hpGrowth = hpGrowth1; mpGrowth = mpGrowth1; atkGrowth = atkGrowth1; defGrowth = defGrowth1;
//	}
//};

//class Laurea : public Player
//{
//public:
//
//	// stats
//	struct stats
//	{
//		int hp, chp, mp,cmp, atk, def, spe, limit;
//		int lvl, hpGrowth, mpGrowth, atkGrowth, defGrowth;
//	};
//	int hp, chp, mp, cmp, atk, def, spe, limit;
//	int lvl, hpGrowth, mpGrowth, atkGrowth, defGrowth;
//
//};
//
//class Lapis : public Player
//{
//public:
//
//	// stats
//	struct stats
//	{
//		int hp, chp, mp, cmp, atk, def, spe, limit;
//		int lvl, hpGrowth, mpGrowth, atkGrowth, defGrowth;
//	};
//	int hp, chp, mp, cmp, atk, def, spe, limit;
//	int lvl, hpGrowth, mpGrowth, atkGrowth, defGrowth;
//
//};
//
//class Lucca : public Player
//{
//public:
//
//	// stats
//	struct stats
//	{
//		int hp, chp, mp, cmp, atk, def, spe, limit;
//		int lvl, hpGrowth, mpGrowth, atkGrowth, defGrowth;
//	};
//	int hp, chp, mp, cmp, atk, def, spe, limit;
//	int lvl, hpGrowth, mpGrowth, atkGrowth, defGrowth;
//};


#endif // __PLAYER_H__