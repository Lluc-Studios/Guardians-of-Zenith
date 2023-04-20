#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "Path.h"
#include "Physics.h"

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

	bool Update();

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	bool IsAlive(); 

	bool LoadState(pugi::xml_node& data);

	bool SaveState(pugi::xml_node& data);
	
	void Move(); 

	void Attack(int frames);

	void debugKeys(); 

	void Death(); 

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
		float HPG = 3.5, MPG = 4, ATKG = 2.2, DEFG = 2;
		int HPC = 10, MPC = 5, ATKC = 5, DEFC = 5;
		float EXPneeded = 100;
	};
	struct Lucca
	{
		float hp, chp, mp, cmp, atk, def, spe, limit;
		float lvl, exp;
		const char* name = "lucca";

		//Leveling variables
		float HPG = 3, MPG = 3, ATKG = 3, DEFG = 2;
		int HPC = 10, MPC = 5, ATKC = 5, DEFC = 5;
		float EXPneeded = 100;
	};

	Laurea laurea{ 350,350,50,50,100,125,10,2,1,0 };
	Lapis lapis{ 250,250,100,100,125,90,12,4,1,0 };
	Lucca lucca{ 200,200,115,115,150,60,15,3,1,0 };


	int timer = 0;
	bool limitFPS = true;
	int PresetChance = 0;

	//for debugging
	bool boss = false;

	//dialogue
	bool isDialogue = false;

	bool DeathAnimationFinished = false;

	PhysBody* pbody;
	PhysBody* attackHitbox;

	bool alive, invincible = false;

	int life = 3, lifeAux = 0, WF, YF;

	iPoint x;

	bool tp1 = false;
	bool tp2 = false;
	bool tp3 = false;
	bool tp4 = false;
	bool tp5 = false;
	bool tp6 = false;
	bool tp7 = false;
	bool tp8 = false;
	bool tp9 = false;
	bool tp10 = false;
	bool tp11 = false;
	bool tp12 = false;

	bool tpHouse = false;


	//Uint32 TotalTime, Time = 0;

	int auxCounter = 0;
	bool auxBool = false;

private:

	SDL_Texture* texture;
	const char* texturePath;

	bool HardMode = false;

	//Fx
	int Step1, Step2, StepMetalic1, StepMetalic2, DeathSound, Swing, DamageFx, aux, tpFX = -1;

	int width;
	int height;

	//int attackCD = 0;
	//int attackFrames = 10; 

	State playerState; 

	b2Vec2 vel; 


	DIRECTION facing = DIRECTION::RIGHT;

	Animation* currentAnim = nullptr;

	Animation playerIdleR;
	Animation playerIdleL;
	Animation playerRunR;
	Animation playerRunL;
	Animation playerAttackR;
	Animation playerAttackL;
	Animation playerDie;

	SDL_Texture* LFHH;
	SDL_Texture* LFH;
	SDL_Texture* LFL;
	SDL_Texture* LFE;
	SDL_Texture* Dialogue;

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