#ifndef __INTRO_H__
#define __INTRO_H__

#include "Module.h"
#include "Player.h"
#include "Animation.h"
#include "Item.h"


class Intro : public Module
{
public:

	Intro();

	// Destructor
	virtual ~Intro();

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

private:
	int fading = 255, counter = 0;
	bool fadeIn = true;
	SDL_Texture* logo;

	SDL_Texture* logoanim;
	const char* texturePath;

	Animation* currentAnim = nullptr;

	Animation fadeo;
	Animation idle;
	
	char introFx;
	char menuMusic;
	char silence;

};

#endif // __INTRO_H__
