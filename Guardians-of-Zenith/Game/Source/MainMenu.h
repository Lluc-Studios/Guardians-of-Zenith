#ifndef __MAINMENU_H__
#define __MAINMENU_H__

#include "Module.h"
#include "Player.h"
#include "Item.h"


class MainMenu : public Module
{
public:

	MainMenu();

	// Destructor
	virtual ~MainMenu();

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

	// Options
	void Options();

public:
	enum class SELECTED {
		START = 1,
		CONTINUE,
		OPTIONS,
		EXIT,
		MUSIC,
		FX,
		FS,
		VS,
		BACK,
		NONE
	};

	SELECTED option = SELECTED::NONE;

	int change, select, fading = 255, fading2 = 0, MX,FX, RGB = 150, RGB1 = 150;
	bool fadeIn = true, fadeOut = false, options = false, FS = false, VS = true;

	SDL_Color WF = { 255, 255, 255 };
	SDL_Color GF = { 200, 200, 200 };
	SDL_Color YF = { 255, 255, 80 };
	SDL_Color RF = { 255, 90, 60 };
	SDL_Color BF = { 0, 0, 0 };

	Uint32 flags;

private:
	SDL_Texture* BG;
	SDL_Texture* Buttons;

	SDL_Rect B1 = { 0,0,106,38 };
	SDL_Rect B2 = { 0,37,106,38 };
	SDL_Rect B3 = { 0,74,106,38 };
	SDL_Rect B4 = { 0,111,106,38 };
	SDL_Rect B5 = { 106,0,106,38 };
	SDL_Rect B6 = { 106,37,106,38 };
	SDL_Rect B7 = { 106,74,106,38 };
	SDL_Rect B8 = { 106,111,106,38 };

};

#endif // __MAINMENU_H__

