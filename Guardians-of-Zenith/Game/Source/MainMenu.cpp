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
#include "GuiManager.h"


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
	app->font->BlitText(134, 10, YF, "obsolete");
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
		app->font->BlitText(142, 40, WF, "music");
		app->font->BlitText(151, 65, WF, "fx");
		app->font->BlitText(127, 95, WF, "fullscreen");
		app->font->BlitText(142, 120, WF, "vsync");
		app->font->BlitText(145, 145, GF, "back");
		app->render->DrawRectangle({ 125,58,64,2 }, 150, 150, 150);
		app->render->DrawRectangle({ MX,56,5,6 }, 150, 150, 150);
		app->render->DrawRectangle({ 125,83,64,2 }, 150, 150, 150);
		app->render->DrawRectangle({ FX,81,5,6 }, 150, 150, 150);
		app->render->DrawRectangle({ 152,108,10,10 }, RGB, RGB, RGB);
		app->render->DrawRectangle({ 152,134,10,10 }, RGB1, RGB1, RGB1);
		app->render->DrawRectangle({ 153,109,8,8 }, 0, 0, 0);
		app->render->DrawRectangle({ 153,135,8,8 }, 0, 0, 0);
		app->render->DrawRectangle({ 154,110,6,6 }, 0, 200, 0);
		app->render->DrawRectangle({ 154,136,6,6 }, 0, 200, 0);
		//Cuadrados rojos
		if (FS == false) {
			app->render->DrawRectangle({ 154,110,6,6 }, 200, 0, 0);
		}
		if (VS == false) {
			app->render->DrawRectangle({ 154,136,6,6 }, 200, 0, 0);
		}
		//Funciones para detectar el ratón
		if (x >= MX && x <= (MX+6) && y >= 56 && y <= 62) {
			app->render->DrawRectangle({ 125,58,64,2 }, 255, 255, 255);
			app->render->DrawRectangle({ MX,56,5,6 }, 255, 255, 255);
			if (option == SELECTED::NONE) {
				option = SELECTED::MUSIC;
				app->audio->PlayFxWithVolume(change, 0, 70);
			}
			//Funcion para arrastrar el boton
			if (option == SELECTED::MUSIC && app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT) {
				MX = x-2;
				if (x < 125) MX = 125;
				if (x > 189) MX = 189;
				app->audio->volume = (MX - 125);
				if (MX <= 130) app->audio->volume = 1;
				if (MX >= 180) app->audio->volume = 100;
			}
		}
		else if (x >= FX && x <= (FX + 6) && y >= 81 && y <= 87) {
			app->render->DrawRectangle({ 125,83,64,2 }, 255, 255, 255);
			app->render->DrawRectangle({ FX,81,5,6 }, 255, 255, 255);
			if (option == SELECTED::NONE) {
				option = SELECTED::FX;
				app->audio->PlayFxWithVolume(change, 0, 70);
			}
			//Funcion para arrastrar el boton
			if (option == SELECTED::FX && app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT) {
				FX = x - 2;
				if (x < 125) FX = 125;
				if (x > 189) FX = 189;
				app->audio->fxvolume = (FX - 95);
				if (FX <= 130) app->audio->fxvolume = 1;
				if (FX >= 180) app->audio->fxvolume = 100;
			}
		}
		else if (x >= 152 && x <= 162 && y >= 108 && y <= 118) {
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
		else if (x >= 152 && x <= 162 && y >= 134 && y <= 144) {
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
		else if (x >= 145 && x <= 169 && y >= 145 && y <= 157) {
			app->font->BlitText(145, 145, WF, "back");
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
		if (x >= 145 && x <= 169 && y >= 40 && y <= 52) {
			app->font->BlitText(145, 40, WF, "play");
			app->font->BlitText(137, 80, GF, "options");
			app->font->BlitText(145, 120, GF, "exit");
			if (option == SELECTED::NONE) {
				option = SELECTED::PLAY;
				app->audio->PlayFxWithVolume(change, 0, 70);
			}
		}
		else if (x >= 137 && x <= 179 && y >= 80 && y <= 92) {
			app->font->BlitText(145, 40, GF, "play");
			app->font->BlitText(137, 80, WF, "options");
			app->font->BlitText(145, 120, GF, "exit");
			if (option == SELECTED::NONE) {
				option = SELECTED::OPTIONS;
				app->audio->PlayFxWithVolume(change, 0, 70);
			}
		}
		else if (x >= 145 && x <= 169 && y >= 120 && y <= 132) {
			app->font->BlitText(145, 40, GF, "play");
			app->font->BlitText(137, 80, GF, "options");
			app->font->BlitText(145, 120, WF, "exit");
			if (option == SELECTED::NONE) {
				option = SELECTED::EXIT;
				app->audio->PlayFxWithVolume(change, 0, 70);
			}
		}
		else {
			app->font->BlitText(145, 40, GF, "play");
			app->font->BlitText(137, 80, GF, "options");
			app->font->BlitText(145, 120, GF, "exit");
			option = SELECTED::NONE;
		}
	}
	//Rectangulos negros para realizar los fades
	app->render->DrawRectangle({ 0,0,1100,800 }, 0, 0, 0, fading);
	app->render->DrawRectangle({ 0,0,1100,800 }, 0, 0, 0, fading2);

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
