#ifndef __MAINMENU_H__
#define __MAINMENU_H__

#include "Module.h"
#include "Player.h"
#include "Item.h"
#include "GuiButton.h"


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

	//void SaveSelection();

	// Called before quitting
	bool CleanUp();

public:
	enum class SELECTED {
		PLAY = 1,
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

	int WF, GF, YF, change, select, fading = 255, fading2 = 0, MX,FX, RGB = 150, RGB1 = 150;
	bool fadeIn = true, fadeOut = false, options = false, FS = false, VS = false;
	//GuiButton* button1;
	//GuiButton* button2;


private:
	//enum class SELECTED {
	//	PLAY = 1,
	//	OPTIONS,
	//	EXIT,
	//	MUSIC,
	//	BACK
	//};

	//SELECTED option = SELECTED::PLAY;
	//int WF, GF, change, select, fading = 255, fading2 = 0;
	//bool fadeIn = true, fadeOut = false, options = false;
	

};

#endif // __MAINMENU_H__

