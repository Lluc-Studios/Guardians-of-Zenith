#ifndef __ENEMY2_H__
#define __ENEMY2_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "Path.h"
#include "DynArray.h"
#include "Physics.h"
#include "Enemy.h"

struct SDL_Texture;


class Enemy2 : public Entity
{
public:

	Enemy2();
	
	virtual ~Enemy2();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	bool IsAlive(); 

	bool LoadState(pugi::xml_node& data);

	bool SaveState(pugi::xml_node& data);

	void DetectPlayer(iPoint playerPos, iPoint enemyPos);

	void Patrol();

	void Death();

public:

	PhysBody* pbody;

	bool invincible = false;
	bool alive = true;

	//Fx

	int dedFx = -1;
	
private:

	SDL_Texture* texture;
	const char* texturePath;

	Animation* currentAnim = nullptr;

	Animation enemyIdleR;
	Animation enemyIdleL;
	Animation enemyRunR;
	Animation enemyRunL;
	Animation enemyDropR;
	Animation enemyDropL;
	Animation enemyDie;

	int width;
	int height;

	b2Vec2 vel;
	DynArray<iPoint> enemyPath;

	EnemyState state; 
	SDL_Texture* tileX = nullptr; 

	int detectDistance = 6;
};

#endif // __ENEMY2_H__