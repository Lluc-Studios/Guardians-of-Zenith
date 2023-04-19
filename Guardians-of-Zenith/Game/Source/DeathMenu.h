#ifndef __DEATHMENU_H__
#define __DEATHMENU_H__

#include "Module.h"
#include "Player.h"
#include "Item.h"


class DeathMenu : public Module
{
public:

	DeathMenu();

	// Destructor
	virtual ~DeathMenu();

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

public:
	bool finished = false;

private:
	bool continue1 = true;
	int fading = 0, change, select, x, y;
	SDL_Texture* MENUD;

public:

	SDL_Color WF = { 255, 255, 255 };
	SDL_Color GF = { 200, 200, 200 };
	SDL_Color YF = { 255, 255, 80 };
};

#endif // __DEATHMENU_H__

