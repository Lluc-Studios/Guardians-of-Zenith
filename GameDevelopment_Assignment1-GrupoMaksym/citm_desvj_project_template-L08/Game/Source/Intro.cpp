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
	logo = app->tex->Load("Assets/Textures/LongCat_Logo.png");
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

	app->render->DrawTexture(logo,app->render->camera.w / 2 / app->win->GetScale() - 64, app->render->camera.h / 2 / app->win->GetScale() - 56);
	app->render->DrawRectangle({ 0,0,1100,800 }, 0, 0, 0, fading);
	counter++;
	//Fade out
	if (counter >= 400) {
		fadeIn = false;
	}
	//Scene transition to menu
	if (fadeIn == false && fading == 255 && counter > 750) {
		//app->scene->active = true;
		//app->menu->active = true;
		//app->entityManager->active = true;
		//app->physics->active = true;
		app->mainmenu->active = true;
		app->intro->active = false;
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

	return true;
}
