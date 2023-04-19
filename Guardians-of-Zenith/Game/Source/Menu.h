#ifndef __MENU_H__
#define __MENU_H__

#include "Module.h"
#include "Player.h"
#include "Item.h"


class Menu : public Module
{
public:

	Menu();

	// Destructor
	virtual ~Menu();

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
	enum class SELECTED {
		FIRST = 1,
		SECOND,
		THIRD
	};

private:
	SELECTED option = SELECTED::FIRST;
	int fading = 255, fontFading = 0, change, select, x, y, tempX;
	bool fadeIn = true, Play = false, PlaySelected = false;
	SDL_Texture* MENUD;

	SDL_Color WF = { 255, 255, 255 };
	SDL_Color GF = { 200, 200, 200 };
	SDL_Color YF = { 255, 255, 80 };
};

#endif // __MENU_H__

