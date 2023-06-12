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

	void Puzzle_Lake();

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

	//Lake Puzzle
	int rock_1[2], rock_2[2], rock_3[2], rock_4[2], rock_5[2], rock_6[2];

	//Puzzle stage
	int stage = 0;

	PhysBody* colliderPuzzle1;
	PhysBody* colliderPuzzle2;
	PhysBody* colliderPuzzle3;
	PhysBody* colliderPuzzle4;
	PhysBody* colliderPuzzle5;
	PhysBody* colliderPuzzle6;
	PhysBody* colliderPuzzle1Block;
	PhysBody* colliderPuzzle2Block;

	bool block1Fx = false, block2Fx = false, block3Fx = false;


	
	//Forest Puzzle
	bool P1Active = false;
	bool P2Active = false;
	bool P3Active = false;
	//Cave Puzzle
	PhysBody* Block1;
	PhysBody* Block2;
	PhysBody* Block3;

	bool B1Block = false, B2Block = false, B3Block = false;

	bool TPressed1 = false, TPressed2 = false, TPressed3 = true, TPressed4 = false, TPressed5 = false, TPressed6 = false, TPressed7 = true, TPressed8 = false, TPressed9 = false;
	bool TPressed10 = false, TPressed11 = false, TPressed12 = false, TPressed13 = false, TPressed14 = false, TPressed15 = false, TPressed16 = false, TPressed17 = true, TPressed18 = false;
	bool TPressed19 = false, TPressed20 = false, TPressed21 = false, TPressed22 = false, TPressed23 = false, TPressed24 = false, TPressed25 = true, TPressed26 = false, TPressed27 = false;
	bool TPressed28 = false, TPressed29 = false, TPressed30 = false, TPressed31 = false, TPressed32 = false, TPressed33 = false, TPressed34 = false, TPressed35 = false, TPressed36 = false;
	bool TPressed37 = false, TPressed38 = false, TPressed39 = false, TPressed40 = false, TPressed41 = false, TPressed42 = false, TPressed43 = false, TPressed44 = true, TPressed45 = false;
	bool TPressed46 = false, TPressed47 = false, TPressed48 = true, TPressed49 = false, TPressed50 = false, TPressed51 = false, TPressed52 = false, TPressed53 = false, TPressed54 = false;
	bool TPressed55 = false, TPressed56 = false, TPressed57 = false, TPressed58 = false, TPressed59 = false, TPressed60 = false, TPressed61 = true, TPressed62 = false, TPressed63 = false;
	bool TPressed64 = false, TPressed65 = false, TPressed66 = false, TPressed67 = false, TPressed68 = false, TPressed69 = true, TPressed70 = false, TPressed71 = false;
	bool TBroken1 = false, TBroken2 = false, TBroken3 = true, TBroken4 = false, TBroken5 = false, TBroken6 = false, TBroken7 = true, TBroken8 = false, TBroken9 = false;
	bool TBroken10 = false, TBroken11 = false, TBroken12 = false, TBroken13 = false, TBroken14 = false, TBroken15 = false, TBroken16 = false, TBroken17 = true, TBroken18 = false;
	bool TBroken19 = false, TBroken20 = false, TBroken21 = false, TBroken22 = false, TBroken23 = false, TBroken24 = false, TBroken25 = true, TBroken26 = false, TBroken27 = false;
	bool TBroken28 = false, TBroken29 = false, TBroken30 = false, TBroken31 = false, TBroken32 = false, TBroken33 = false, TBroken34 = false, TBroken35 = false, TBroken36 = false;
	bool TBroken37 = false, TBroken38 = false, TBroken39 = false, TBroken40 = false, TBroken41 = false, TBroken42 = false, TBroken43 = false, TBroken44 = true, TBroken45 = false;
	bool TBroken46 = false, TBroken47 = false, TBroken48 = true, TBroken49 = false, TBroken50 = false, TBroken51 = false, TBroken52 = false, TBroken53 = false, TBroken54 = false;
	bool TBroken55 = false, TBroken56 = false, TBroken57 = false, TBroken58 = false, TBroken59 = false, TBroken60 = false, TBroken61 = true, TBroken62 = false, TBroken63 = false;
	bool TBroken64 = false, TBroken65 = false, TBroken66 = false, TBroken67 = false, TBroken68 = false, TBroken69 = true, TBroken70 = false, TBroken71 = false;

private:
	bool Music = false, PF = false;
	SDL_Texture* img;
	SDL_Texture* MapAdjustment;

	SDL_Texture* invTex;
	SDL_Texture* invArrowTex;

	SDL_Texture* TownPNG;
	SDL_Texture* WaterPNG;
	SDL_Texture* ForestPNG;
	SDL_Texture* ForestPNG_Sadow;

	SDL_Texture* coinTex;
	SDL_Texture* trophyTex;
	SDL_Texture* BGtexture;
	//Tavernero
	SDL_Texture* NPC1;
	//Herrara
	SDL_Texture* NPC2;
	//Chica Wasap
	SDL_Texture* NPC3;

	SDL_Texture* LAPIS;
	SDL_Texture* Stone;
	SDL_Texture* StoneDeep;
	SDL_Texture* Tp;
	SDL_Texture* Unpressed;
	SDL_Texture* Pressed;
	SDL_Texture* Puzzle2Wall;
	SDL_Texture* Slime;
	SDL_Texture* Lily;
	SDL_Texture* Naiadon;

	//Cave dungeon

	bool Reload = false;
	
	SDL_Texture* Blocking;
	SDL_Texture* DefaultTile;
	SDL_Texture* BrokenTile;

	//Forest dungeon
	
	SDL_Texture* Pilar1A;
	SDL_Texture* Pilar1N;
	SDL_Texture* Pilar2A;
	SDL_Texture* Pilar2N;
	SDL_Texture* Pilar3A;
	SDL_Texture* Pilar3N;
	SDL_Texture* ForestDoor;

	bool FdoorActive = false;

	//Others

	bool Pressed1 = false, Pressed2 = false, Pressed3 = false;
	bool pressed1Fx = false, pressed2Fx = false, pressed3Fx = false;

	//FX
	int change, select, play;
	int puzzleFX;

	bool alreadyChangeFX = false;

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

	//Forest puzzle

	PhysBody* P1;
	PhysBody* P2;
	PhysBody* P3;
	PhysBody* Fdoor;

public:
	enum class SELECTED {
		SAVEGAME,
		OPTIONS,
		MAINMENU,
		EXIT,
		NONE
	};

	SELECTED option = SELECTED::NONE;

	SDL_Texture* texturas[15];
	iPoint itemPos[15];
	bool itemPicked[15];
	PhysBody* itemBody[15];
	int itemInstance[15];

};

#endif // __SCENE_H__