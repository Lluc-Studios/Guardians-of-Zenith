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

private:
	bool Music = false, PF = false;
	SDL_Texture* img;
	SDL_Texture* MapAdjustment;

	SDL_Texture* coinTex;
	SDL_Texture* trophyTex;
	SDL_Texture* BGtexture;
	SDL_Texture* EnemyT;

	SDL_Rect ET = { 16,17,32,32 };
};

#endif // __SCENE_H__