#ifndef __SAW_H__
#define __SAW_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "Path.h"

struct SDL_Texture;

class Saw : public Entity
{
public:

	Saw();
	virtual ~Saw();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	bool Path(iPoint p);

	bool LoadState(pugi::xml_node& data);

	bool SaveState(pugi::xml_node& data);
public:


private:

	SDL_Texture* texture;
	const char* texturePath;

	PhysBody* pbody;

	iPoint p1;
	iPoint p2;

	iPoint currentPoint = p2;

	bool noMove;

	int speed;

	Animation* currentAnimation = nullptr;

	Animation spinAnim;
};

#endif // __SAW_H__