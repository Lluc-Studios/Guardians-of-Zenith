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
#include "DeathMenu.h"


#include "Defs.h"
#include "Log.h"

MainMenu::MainMenu() : Module()
{
	name.Create("mainmenu");
}

// Destructor
MainMenu::~MainMenu()
{}

// Called before render is available
bool MainMenu::Awake(pugi::xml_node& config)
{
	LOG("Loading main menu");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool MainMenu::Start()
{
	char lookupTable[] = { "abcdefghijklmnopqrstuvwxyz0123456789" };
	WF = app->font->Load("Assets/Fonts/FontWhiteDef.png", lookupTable, 1);
	GF = app->font->Load("Assets/Fonts/FontGreyDef.png", lookupTable, 1);
	YF = app->font->Load("Assets/Fonts/FontYellowDef.png", lookupTable, 1);
	change = app->audio->LoadFx("Assets/Sounds/ChangeSelection.wav");
	select = app->audio->LoadFx("Assets/Sounds/Select.wav");
	MX = 125 + app->audio->volume;
	FX = 125 + app->audio->fxvolume/2;

	return true;
}

// Called each loop iteration
bool MainMenu::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool MainMenu::Update(float dt)
{
	bool ret = true;

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	//Funcion llamada desde el menu de muerte para volver al menu principal (Se desactiva a si misma)
	if (app->deathmenu->finished == true) {
		fading = 255;
		fading2 = 0;
		fadeIn = true;
		fadeOut = false;
		options = false;
		app->deathmenu->finished = false;
	}
	//FadeIn para empezar a ver el menu
	if (fadeIn == true) {
		if (fading >= 1) { fading--; };
		if (fading == 0) { fadeIn = false; };
	}
	//Fadeout para empezar el gameplay
	if (fadeOut == true) {
		if (fading2 <= 254) { fading2++; };
	}
	//Se activan todos los modulos necesarios para empezar el gameplay y se desactiva este mismo modulo
	if (fading2 == 255) {
		app->scene->active = true;
		app->scene->player->active = true;
		app->entityManager->active = true;
		app->physics->active = true;
		app->scene->CanPlayerMove = true;
		app->mainmenu->active = false;

	}
	//Nombre del juego (el original)
	//app->font->BlitText(134, 10, YF, "obsolete");
	//Funcion para detectar el raton en la parte principal del menu
	if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP) {
		if (option == SELECTED::PLAY) {
			app->audio->PlayFxWithVolume(select, 0, 70);
			fadeOut = true;
		}
		if (option == SELECTED::OPTIONS) {
			app->audio->PlayFxWithVolume(select, 0, 70);
			options = true;
		}
		if (option == SELECTED::EXIT) {
			ret = false;
		}
	}
	//Condicional para dibujar los ajustes cuando se seleccionan las 'opciones'
	if (options == true) {
		int x, y;
		x = app->input->GetMousePositionX();
		y = app->input->GetMousePositionY();
		app->font->BlitText(142 * app->ScalingMultiplier, 40 * app->ScalingMultiplier, WF, "music");
		app->font->BlitText(151 * app->ScalingMultiplier, 65 * app->ScalingMultiplier, WF, "fx");
		app->font->BlitText(127 * app->ScalingMultiplier, 95 * app->ScalingMultiplier, WF, "fullscreen");
		app->font->BlitText(142 * app->ScalingMultiplier, 120 * app->ScalingMultiplier, WF, "vsync");
		app->font->BlitText(145 * app->ScalingMultiplier, 145 * app->ScalingMultiplier, GF, "back");
		app->render->DrawRectangle({ 125 * app->ScalingMultiplier,58 * app->ScalingMultiplier,64 * app->ScalingMultiplier,2 * app->ScalingMultiplier }, 150, 150, 150);
		app->render->DrawRectangle({ MX * app->ScalingMultiplier,56 * app->ScalingMultiplier,5 * app->ScalingMultiplier,6 * app->ScalingMultiplier }, 150, 150, 150);
		app->render->DrawRectangle({ 125 * app->ScalingMultiplier,83 * app->ScalingMultiplier,64 * app->ScalingMultiplier,2 * app->ScalingMultiplier }, 150, 150, 150);
		app->render->DrawRectangle({ FX * app->ScalingMultiplier,81 * app->ScalingMultiplier,5 * app->ScalingMultiplier,6 * app->ScalingMultiplier }, 150, 150, 150);
		app->render->DrawRectangle({ 152 * app->ScalingMultiplier,108 * app->ScalingMultiplier,10 * app->ScalingMultiplier,10 * app->ScalingMultiplier }, RGB, RGB, RGB);
		app->render->DrawRectangle({ 152 * app->ScalingMultiplier,134 * app->ScalingMultiplier,10 * app->ScalingMultiplier,10 * app->ScalingMultiplier }, RGB1, RGB1, RGB1);
		app->render->DrawRectangle({ 153 * app->ScalingMultiplier,109 * app->ScalingMultiplier,8 * app->ScalingMultiplier,8 * app->ScalingMultiplier }, 0, 0, 0);
		app->render->DrawRectangle({ 153 * app->ScalingMultiplier,135 * app->ScalingMultiplier,8 * app->ScalingMultiplier,8 * app->ScalingMultiplier }, 0, 0, 0);
		app->render->DrawRectangle({ 154 * app->ScalingMultiplier,110 * app->ScalingMultiplier,6 * app->ScalingMultiplier,6 * app->ScalingMultiplier }, 0, 200, 0);
		app->render->DrawRectangle({ 154 * app->ScalingMultiplier,136 * app->ScalingMultiplier,6 * app->ScalingMultiplier,6 * app->ScalingMultiplier }, 0, 200, 0);
		//Cuadrados rojos
		if (FS == false) {
			app->render->DrawRectangle({ 154 * app->ScalingMultiplier,110 * app->ScalingMultiplier,6 * app->ScalingMultiplier,6 * app->ScalingMultiplier }, 200, 0, 0);
		}
		if (VS == false) {
			app->render->DrawRectangle({ 154 * app->ScalingMultiplier,136 * app->ScalingMultiplier,6 * app->ScalingMultiplier,6 * app->ScalingMultiplier }, 200, 0, 0);
		}
		//Funciones para detectar el ratón
		if (x >= MX * app->ScalingMultiplier && x <= (MX * app->ScalingMultiplier +6) && y >= 56 * app->ScalingMultiplier && y <= 62 * app->ScalingMultiplier) {
			app->render->DrawRectangle({ 125 * app->ScalingMultiplier,58 * app->ScalingMultiplier,64 * app->ScalingMultiplier,2 * app->ScalingMultiplier }, 255, 255, 255);
			app->render->DrawRectangle({ MX * app->ScalingMultiplier,56 * app->ScalingMultiplier,5 * app->ScalingMultiplier,6 * app->ScalingMultiplier }, 255, 255, 255);
			if (option == SELECTED::NONE) {
				option = SELECTED::MUSIC;
				app->audio->PlayFxWithVolume(change, 0, 70);
			}
			//Funcion para arrastrar el boton
			if (option == SELECTED::MUSIC && app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT) {
				MX = (x - 2) / app->ScalingMultiplier;
				if (x < 125 * app->ScalingMultiplier) MX = 125;
				if (x > 189 * app->ScalingMultiplier) MX = 189;
				if (MX > 189 * app->ScalingMultiplier) MX = 189;
				app->audio->volume = (MX - 125);
				if (MX <= 130 ) app->audio->volume = 1;
				if (MX >= 180 ) app->audio->volume = 100;
			}
		}
		else if (x >= FX * app->ScalingMultiplier && x <= (FX * app->ScalingMultiplier + 6) && y >= 81 * app->ScalingMultiplier && y <= 87 * app->ScalingMultiplier) {
			app->render->DrawRectangle({ 125 * app->ScalingMultiplier,83 * app->ScalingMultiplier,64 * app->ScalingMultiplier,2 * app->ScalingMultiplier }, 255, 255, 255);
			app->render->DrawRectangle({ FX * app->ScalingMultiplier,81 * app->ScalingMultiplier,5 * app->ScalingMultiplier,6 * app->ScalingMultiplier }, 255, 255, 255);
			if (option == SELECTED::NONE) {
				option = SELECTED::FX;
				app->audio->PlayFxWithVolume(change, 0, 70);
			}
			//Funcion para arrastrar el boton
			if (option == SELECTED::FX && app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT) {
				FX = (x - 2) / app->ScalingMultiplier;
				if (x < 125 * app->ScalingMultiplier) FX = 125;
				if (x > 189 * app->ScalingMultiplier) FX = 189;
				app->audio->fxvolume = (FX - 95);
				if (FX <= 130) app->audio->fxvolume = 1;
				if (FX >= 180) app->audio->fxvolume = 100;
			}
		}
		else if (x >= 152 * app->ScalingMultiplier && x <= 162 * app->ScalingMultiplier && y >= 108 * app->ScalingMultiplier && y <= 118 * app->ScalingMultiplier) {
			RGB = 255;
			if (option == SELECTED::NONE) {
				option = SELECTED::FS;
				app->audio->PlayFxWithVolume(change, 0, 70);
			}
			if (option == SELECTED::FS && app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN) {
				FS = !FS;
				app->audio->PlayFxWithVolume(select, 0, 70);
			}

		}
		else if (x >= 152 * app->ScalingMultiplier && x <= 162 * app->ScalingMultiplier && y >= 134 * app->ScalingMultiplier && y <= 144 * app->ScalingMultiplier) {
			RGB1 = 255;
			if (option == SELECTED::NONE) {
				option = SELECTED::VS;
				app->audio->PlayFxWithVolume(change, 0, 70);
			}
			if (option == SELECTED::VS && app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN) {
				VS = !VS;
				app->audio->PlayFxWithVolume(select, 0, 70);
			}

		}
		else if (x >= 145 * app->ScalingMultiplier && x <= 169 * app->ScalingMultiplier && y >= 145 * app->ScalingMultiplier && y <= 157 * app->ScalingMultiplier) {
			app->font->BlitText(145 * app->ScalingMultiplier, 145 * app->ScalingMultiplier, WF, "back");
			if (option == SELECTED::NONE) {
				option = SELECTED::BACK;
				app->audio->PlayFxWithVolume(change, 0, 70);
			}
			if (option == SELECTED::BACK && app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN) {
				options = false;
			}
		}
		else {
			RGB = 150;
			RGB1 = 150;
			option = SELECTED::NONE;
		}
	}
	//Funcion para detectar sobre que boton esta el ratón
	if (options == false) {
		int x, y;
		x = app->input->GetMousePositionX();
		y = app->input->GetMousePositionY();
		if (x >= 145 * app->ScalingMultiplier && x <= 169 * app->ScalingMultiplier && y >= 40 * app->ScalingMultiplier && y <= 52 * app->ScalingMultiplier) {
			app->font->BlitText(145 * app->ScalingMultiplier, 40 * app->ScalingMultiplier, WF, "play");
			app->font->BlitText(137 * app->ScalingMultiplier, 80 * app->ScalingMultiplier, GF, "options");
			app->font->BlitText(145 * app->ScalingMultiplier, 120 * app->ScalingMultiplier, GF, "exit");
			if (option == SELECTED::NONE) {
				option = SELECTED::PLAY;
				app->audio->PlayFxWithVolume(change, 0, 70);
			}
		}
		else if (x >= 137 * app->ScalingMultiplier && x <= 179 * app->ScalingMultiplier && y >= 80 * app->ScalingMultiplier && y <= 92 * app->ScalingMultiplier) {
			app->font->BlitText(145 * app->ScalingMultiplier, 40 * app->ScalingMultiplier, GF, "play");
			app->font->BlitText(137 * app->ScalingMultiplier, 80 * app->ScalingMultiplier, WF, "options");
			app->font->BlitText(145 * app->ScalingMultiplier, 120 * app->ScalingMultiplier, GF, "exit");
			if (option == SELECTED::NONE) {
				option = SELECTED::OPTIONS;
				app->audio->PlayFxWithVolume(change, 0, 70);
			}
		}
		else if (x >= 145 * app->ScalingMultiplier && x <= 169 * app->ScalingMultiplier && y >= 120 * app->ScalingMultiplier && y <= 132 * app->ScalingMultiplier) {
			app->font->BlitText(145 * app->ScalingMultiplier, 40 * app->ScalingMultiplier, GF, "play");
			app->font->BlitText(137 * app->ScalingMultiplier, 80 * app->ScalingMultiplier, GF, "options");
			app->font->BlitText(145 * app->ScalingMultiplier, 120 * app->ScalingMultiplier, WF, "exit");
			if (option == SELECTED::NONE) {
				option = SELECTED::EXIT;
				app->audio->PlayFxWithVolume(change, 0, 70);
			}
		}
		else {
			app->font->BlitText(145 * app->ScalingMultiplier, 40 * app->ScalingMultiplier, GF, "play");
			app->font->BlitText(137 * app->ScalingMultiplier, 80 * app->ScalingMultiplier, GF, "options");
			app->font->BlitText(145 * app->ScalingMultiplier, 120 * app->ScalingMultiplier, GF, "exit");
			option = SELECTED::NONE;
		}
	}
	//Rectangulos negros para realizar los fades
	app->render->DrawRectangle({ 0,0,5000,5000 }, 0, 0, 0, fading);
	app->render->DrawRectangle({ 0,0,5000,5000 }, 0, 0, 0, fading2);

	return ret;
}

// Called each loop iteration
bool MainMenu::PostUpdate()
{
	bool ret = true;
	return ret;
}

// Called before quitting
bool MainMenu::CleanUp()
{
	LOG("Freeing main menu");
	app->font->UnLoad(WF);
	app->font->UnLoad(GF);

	return true;
}
