#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Intro.h"
#include "Menu.h"
#include "MainMenu.h"
#include "Entitymanager.h"
#include "Scene.h"
#include "Physics.h"

#include "Defs.h"
#include "Log.h"

Intro::Intro() : Module()
{
	name.Create("intro");
}

// Destructor
Intro::~Intro()
{}

// Called before render is available
bool Intro::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Intro::Start()
{
	logo = app->tex->Load("Assets/Textures/Logo_resized.png");
	introFx = app->audio->LoadFx("Assets/Soundtrack/Fx/Intro_Emanem.wav");
	menuMusic = app->audio->LoadFx("Assets/Soundtrack/Music/Rocky Tundra OST Version.ogg");

	app->audio->PlayFxWithVolume(introFx);

	return true;
}

// Called each loop iteration
bool Intro::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Intro::Update(float dt)
{
	//Initial fade in
	if (fadeIn == true) {
		if (fading >= 1) { fading--; };
	}
	if (fadeIn == false) {
		if (fading <= 254) { fading++; };
	}

	app->render->DrawTexture(logo,0,0);
	app->render->DrawRectangle({ 0,0,5000,5000 }, 0, 0, 0, fading);
	counter++;

	//Fade out
	if (counter >= 200) {
		fadeIn = false;
	}
	//Scene transition to menu
	if (fadeIn == false && fading == 255 && counter > 400) {
		//app->scene->active = true;
		//app->menu->active = true;
		//app->entityManager->active = true;
		//app->physics->active = true;
		app->mainmenu->active = true;
		app->intro->active = false;

		app->audio->PlayMusic("Assets/Soundtrack/Music/Rocky_Tundra_OST_Version.ogg");

	}
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		fadeIn = false;
		counter = 750;
	}
	if (app->input->controllers.A != 0)
	{
		fadeIn = false;
		counter = 750;
	}
	//Debug
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		app->scene->active = true;
		app->entityManager->active = true;
		app->physics->active = true;
		app->scene->CanPlayerMove = true;
		app->intro->active = false;
	}

	return true;
}

// Called each loop iteration
bool Intro::PostUpdate()
{
	bool ret = true;
	return ret;
}

// Called before quitting
bool Intro::CleanUp()
{
	LOG("Freeing intro");

	app->tex->UnLoad(logo);
	app->audio->PlayMusic("Assets/Soundtrack/silence.ogg");

	return true;
}
