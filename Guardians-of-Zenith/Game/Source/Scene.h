#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "Enemy.h"
#include "Enemy2.h"
#include "Item.h"
#include "Saw.h"
#include "Teleport.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

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

	bool LoadState(pugi::xml_node& data);

	bool SaveState(pugi::xml_node& data);

	bool Pause();

	void PlayMusic();

	void RestartCave();

public:

	//fading
	int fading = 0, selected = 0;
	bool fade = false;
	//L02: DONE 3: Declare a Player attribute 
	Player* player;
	bool CanPlayerMove = false;

	// PATHFINDING
	SDL_Texture* mouseTileTex = nullptr;
	SDL_Texture* originTex = nullptr;

	// L12: Debug pathfing
	iPoint origin;
	iPoint destination;
	bool originSelected = false, checked = false, pathActive = false;;
	iPoint AuxPlayer, AuxEnemy, Checker, Checker2;

	bool isPaused = false;

	bool options = false;

private:
	bool Music = false, PF = false;
	SDL_Texture* img;
	SDL_Texture* MapAdjustment;

	SDL_Texture* invTex;
	SDL_Texture* invArrowTex;

	SDL_Texture* coinTex;
	SDL_Texture* trophyTex;
	SDL_Texture* BGtexture;
	SDL_Texture* NPC1;
	SDL_Texture* NPC2;
	SDL_Texture* NPC3;
	SDL_Texture* LAPIS;
	SDL_Texture* Stone;
	SDL_Texture* Tp;
	SDL_Texture* Unpressed;
	SDL_Texture* Pressed;
	SDL_Texture* Puzzle2Wall;
	SDL_Texture* Slime;
	SDL_Texture* Lily;
	SDL_Texture* Naiadon;

	//Cave dungeon
	
	SDL_Texture* DefaultTile;
	SDL_Texture* BrokenTile;

	bool TPressed1 = false, TPressed2 = false, TPressed3 = false, TPressed4 = false, TPressed5 = false, TPressed6 = false, TPressed7 = false, TPressed8 = false, TPressed9 = false;
	bool TPressed10 = false, TPressed11 = false, TPressed12 = false, TPressed13 = false, TPressed14 = false, TPressed15 = false, TPressed16 = false, TPressed17 = false, TPressed18 = false;
	bool TPressed19 = false, TPressed20 = false, TPressed21 = false, TPressed22 = false, TPressed23 = false, TPressed24 = false, TPressed25 = false, TPressed26 = false, TPressed27 = false;
	bool TBroken1 = false, TBroken2 = false, TBroken3 = false, TBroken4 = false, TBroken5 = false, TBroken6 = false, TBroken7 = false, TBroken8 = false, TBroken9 = false;
	bool TBroken10 = false, TBroken11 = false, TBroken12 = false, TBroken13 = false, TBroken14= false, TBroken15 = false, TBroken16 = false, TBroken17 = false, TBroken18 = false;
	bool TBroken19 = false, TBroken20 = false, TBroken21 = false, TBroken22 = false, TBroken23 = false, TBroken24 = false, TBroken25 = false, TBroken26 = false, TBroken27 = false;

	//Others

	bool Pressed1 = false, Pressed2 = false, Pressed3 = false;

	SDL_Rect ET = { 0,0,64,64 };
	SDL_Rect N1T = { 0,0,32,32 };
	SDL_Rect N2T = { 0,0,32,32 };
	SDL_Rect N3T = { 0,0,32,32 };
	SDL_Rect LAPIS1 = { 0,0,32,32 };

	SDL_Color WF = { 255, 255, 255 };
	SDL_Color GF = { 200, 200, 200 };
	SDL_Color YF = { 255, 255, 80 };

	char  villageMusic;

	//Mando 
	bool Start_pressed = false;

	//Lake enemies

	PhysBody* Slime1;
	PhysBody* Slime2;
	PhysBody* Slime3;
	PhysBody* Slime4;
	PhysBody* Slime5;
	PhysBody* Slime6;
	PhysBody* Slime7;
	PhysBody* Slime8;
	PhysBody* Slime9;
	PhysBody* Slime10;
	PhysBody* Slime11;

	PhysBody* Lily1;
	PhysBody* Lily2;
	PhysBody* Lily3;
	PhysBody* Lily4;
	PhysBody* Lily5;

	PhysBody* Naiadon1;


	//Lake puzzle

	PhysBody* MonoTP;

	PhysBody* colliderPuzzle1;
	PhysBody* colliderPuzzle2;
	PhysBody* colliderPuzzle3;
	PhysBody* colliderPuzzle4;
	PhysBody* colliderPuzzle5;
	PhysBody* colliderPuzzle6;
	PhysBody* colliderPuzzle1Block;
	PhysBody* colliderPuzzle2Block;

	int x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6;

	//Puzzle1
	int stage = 0;


public:
	enum class SELECTED {
		SAVEGAME,
		OPTIONS,
		MAINMENU,
		EXIT,
		NONE
	};

	SELECTED option = SELECTED::NONE;
};

#endif // __SCENE_H__