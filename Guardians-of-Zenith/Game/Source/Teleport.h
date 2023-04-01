#pragma once
#ifndef __TELEPORT_H__
#define __TELEPORT_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

struct SDL_Texture;

class Teleport : public Entity
{
public:

	Teleport();
	virtual ~Teleport();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

public:

	bool isPicked = false;

private:

	SDL_Texture* texture;
	const char* texturePath;


	//DONE 4: Add a physics to an item
	PhysBody* pbody;

	int id; 

	Animation* currentAnimation = nullptr;

	Animation spinAnim;
};

#endif // __TELEPORT_H__