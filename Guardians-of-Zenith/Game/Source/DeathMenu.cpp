#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Menu.h"
#include "MainMenu.h"
#include "Entitymanager.h"
#include "Scene.h"
#include "Physics.h"
#include "ModuleFonts.h"
#include "Entity.h"
#include "DeathMenu.h"

#include "Defs.h"
#include "Log.h"

DeathMenu::DeathMenu() : Module()
{
	name.Create("menu");
}

// Destructor
DeathMenu::~DeathMenu()
{}

// Called before render is available
bool DeathMenu::Awake(pugi::xml_node& config)
{
	bool ret = true;

	return ret;
}

// Called before the first frame
bool DeathMenu::Start()
{
	//char lookupTable[] = { "abcdefghijklmnopqrstuvwxyz0123456789" };
	//WF = app->font->Load("Assets/Fonts/FontWhiteDef.png", lookupTable, 1);
	//GF = app->font->Load("Assets/Fonts/FontGreyDef.png", lookupTable, 1);
	change = app->audio->LoadFx("Assets/Soundtrack/Fx/ChangeSelection.wav");
	select = app->audio->LoadFx("Assets/Soundtrack/Fx/Select.wav");
	//MENUD = app->tex->Load("Assets/Textures/Menu_Gradient.png");
	x = app->render->camera.x;
	y = app->render->camera.y;

	return true;
}

// Called each loop iteration
bool DeathMenu::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool DeathMenu::Update(float dt)
{
	bool ret = true;
	//Death Menu
	if (app->scene->player->DeathAnimationFinished == true) {
		app->render->DrawRectangle({ -1000,-1000,10000,2000 }, 0, 0, 0, fading);
		if (fading <= 99) {
			fading++;
		}
	}
	if (fading == 100) {
		if (continue1 == true) {
			app->render->DrawText(x + 140, y + 50, WF, "continue", 16);
			app->render->DrawText(x + 150, y + 90, GF, "exit", 16);
		}
		if (continue1 == false) {
			app->render->DrawText(x + 140, y + 50, GF, "continue", 16);
			app->render->DrawText(x + 150, y + 90, WF, "exit", 16);
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		app->audio->PlayFxWithVolume(select,0,70);
		if (continue1 == true) {
			app->scene->player->active = true;
			app->scene->player->alive = true;
			app->scene->player->life = 3;
			app->scene->player->pbody->body->SetActive(true);
			app->scene->player->pbody->body->SetTransform({ PIXEL_TO_METERS(500), PIXEL_TO_METERS(220) }, 0);
			app->deathmenu->active = false;
		}
		if (continue1 == false) {
			app->scene->player->alive = true;
			app->scene->player->life = 3;
			app->scene->player->pbody->body->SetActive(true);
			app->scene->player->pbody->body->SetTransform({ PIXEL_TO_METERS(500), PIXEL_TO_METERS(220) }, 0);
			app->entityManager->active = false;
			app->physics->active = false;
			app->scene->active = false;
			app->scene->CanPlayerMove = false;
			finished = true;
			app->mainmenu->active = true;
			app->deathmenu->active = false;
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN) {
		if (continue1 == false) {
			continue1 = true;
			app->audio->PlayFxWithVolume(change,0,70);
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN) {
		if (continue1 == true) {
			continue1 = false;
			app->audio->PlayFxWithVolume(change,0,70);
		}

	}

	return ret;
}

// Called each loop iteration
bool DeathMenu::PostUpdate()
{
	bool ret = true;
	return ret;
}

// Called before quitting
bool DeathMenu::CleanUp()
{
	LOG("Freeing menu");

	return true;
}
