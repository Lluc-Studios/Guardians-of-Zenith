#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Menu.h"
#include "Entitymanager.h"
#include "Scene.h"
#include "Physics.h"
#include "ModuleFonts.h"
#include "Entity.h"

#include "Defs.h"
#include "Log.h"

Menu::Menu() : Module()
{
	name.Create("menu");
}

// Destructor
Menu::~Menu()
{}

// Called before render is available
bool Menu::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;
	//x = config.child("player").attribute("x").as_int();
	//y = config.child("player").attribute("y").as_int();
	x = app->render->camera.x;
	y = app->render->camera.y;

	return ret;
}

// Called before the first frame
bool Menu::Start()
{
	change = app->audio->LoadFx("Assets/Soundtrack/Fx/ChangeSelection.wav");
	select = app->audio->LoadFx("Assets/Soundtrack/Fx/Select.wav");
	fading = 255;
	fontFading = 0;
	fadeIn = true;
	Play = false;
	PlaySelected = false;
	return true;
}

// Called each loop iteration
bool Menu::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Menu::Update(float dt)
{
	bool ret = true;
	//Fade out and exit
	if (fadeIn == true) {
		if (fading >= 1) { fading--; };
		if (fading == 0) { fadeIn = false; };
	}
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && fadeIn == false) {
		app->audio->PlayFxWithVolume(select,0,70);
		PlaySelected = true;

	}
	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN) {
		if (option == SELECTED::SECOND) {
			option = SELECTED::FIRST;
			app->audio->PlayFxWithVolume(change,0,70);
		}
		if (option == SELECTED::THIRD) {
			option = SELECTED::SECOND;
			app->audio->PlayFxWithVolume(change,0,70);
		}

	}
	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN) {
		if (option == SELECTED::SECOND) {
			option = SELECTED::THIRD;
			app->audio->PlayFxWithVolume(change,0,70);
		}
		if (option == SELECTED::FIRST) {
			option = SELECTED::SECOND;
			app->audio->PlayFxWithVolume(change,0,70);
		}
	}
	tempX = -1*x;
	app->render->DrawTexture(MENUD, tempX - 120, (-1 * y));
	if (option == SELECTED::FIRST && fontFading != 255) {
		app->render->DrawText(x + 65, y + 40, WF, "empty", 16);
		app->render->DrawText(x + 65, y + 80, GF, "empty", 16);
		app->render->DrawText(x + 65, y + 120, GF, "empty", 16);
	}
	if (option == SELECTED::SECOND && fontFading != 255) {
		app->render->DrawText(x + 65, y + 40, GF, "empty", 16);
		app->render->DrawText(x + 65, y + 80, WF, "empty", 16);
		app->render->DrawText(x + 65, y + 120, GF, "empty", 16);
	}
	if (option == SELECTED::THIRD && fontFading != 255) {
		app->render->DrawText(x + 65, y + 40, GF, "empty", 16);
		app->render->DrawText(x + 65, y + 80, GF, "empty", 16);
		app->render->DrawText(x + 65, y + 120, WF, "empty", 16);
	}
	if (PlaySelected == true) {
		if (fontFading <= 254) {
			fontFading++;
		}
		else if (fontFading == 255) {
			Play = true;
		}
	}
	if (Play == true) {
		app->scene->CanPlayerMove = true;
		app->menu->active = false;
	}
	app->render->DrawRectangle({ -1000,-1000,10000,2000 }, 0, 0, 0, fading);
	
	if (PlaySelected == true && Play == false) {
		app->render->DrawRectangle({ x - 40,y + 120,100,200 }, 0, 0, 0, fontFading);
	}
	return ret;
}

// Called each loop iteration
bool Menu::PostUpdate()
{
	bool ret = true;
	return ret;
}

// Called before quitting
bool Menu::CleanUp()
{
	LOG("Freeing menu");

	return true;
}
