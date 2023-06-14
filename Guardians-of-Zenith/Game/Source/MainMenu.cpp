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
	//mainmenu = app->tex->Load("Assets/Textures/main_menu.png");

	change = app->audio->LoadFx("Assets/Soundtrack/Fx/Menu/ChangeSelection.wav");
	select = app->audio->LoadFx("Assets/Soundtrack/Fx/Menu/Select.wav");
	play = app->audio->LoadFx("Assets/Soundtrack/Fx/Menu/Play.wav");

	MX = 125 + app->audio->volume;
	FX = 125 + app->audio->fxvolume/2;
	BG = app->tex->Load("Assets/Textures/Scenes/new_mainmenu.png");
	Buttons = app->tex->Load("Assets/Textures/Scenes/Title_Buttons.png");

	fading = 255;
	fading2 = 0;
	fadeIn = true;
	fadeOut = false;

	titleanim = app->tex->Load("Assets/Textures/Scenes/anim_title.png");

	title.PushBack({ 0, 0, 640, 360 }); //1
	title.PushBack({ 640, 0, 640, 360 }); //2
	title.PushBack({ 1280, 0, 640, 360 }); //3
	title.PushBack({ 0, 360, 640, 360 }); //4
	title.PushBack({ 640, 360, 640, 360 }); //5
	title.PushBack({ 1280, 360, 640, 360 }); //6
	title.PushBack({ 0, 720, 640, 360 }); //7
	title.PushBack({ 640, 720, 640, 360 }); //8
	title.PushBack({ 1280, 720, 640, 360 }); //9
	title.PushBack({ 0, 1080, 640, 360 }); //10
	title.PushBack({ 640, 1080, 640, 360 }); //11
	title.PushBack({ 1280, 1080, 640, 360 }); //12
	title.loop = false;
	title.speed = 0.08f;

	currentAnim = &title;
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

	currentAnim->Update();
	SDL_Rect rect = currentAnim->GetCurrentFrame();

	app->render->DrawTexture(titleanim, 0, 0, &rect);

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
		app->audio->PlayMusic("Assets/Soundtrack/Music/Guidance-Island-OST-Version.ogg");
	}

	//Funcion para detectar el raton en la parte principal del menu
	if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP || app->input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_DOWN || app->input->controllers.A != 0 && !A_pressed) {
		if (option == SELECTED::START) {
			app->scene->player->InitializePlayers();
			app->audio->PlayFxWithVolume(play, 0, app->audio->fxvolume);
			fadeOut = true;
		}
		if (option == SELECTED::CONTINUE) {
			app->audio->PlayFxWithVolume(play, 0, app->audio->fxvolume);
			fadeOut = true;
			app->LoadGameRequest();
		}
		if (option == SELECTED::OPTIONS) {
			app->audio->PlayFxWithVolume(select, 0, app->audio->fxvolume);
			options = true;
		}
		if (option == SELECTED::EXIT) {
			app->audio->PlayFxWithVolume(play, 0, app->audio->fxvolume);
			ret = false;
		}
	}
	//Condicional para dibujar los ajustes cuando se seleccionan las 'opciones'
	if (options == true) {
		Options(0,0);
		app->mainmenu->Options(0,0);
	}
	//Funcion para detectar sobre que boton esta el ratón
	if (options == false) {
		//Button textures
		app->render->DrawTexture(Buttons, 450, 120, &B1);
		app->render->DrawTexture(Buttons, 450, 180, &B2);
		app->render->DrawTexture(Buttons, 450, 240, &B3);
		app->render->DrawTexture(Buttons, 450, 300, &B4);

		int x, y;
		x = app->input->GetMousePositionX();
		y = app->input->GetMousePositionY();
		if (x >= 450 && x <= 556 && y >= 120 && y <= 158 ) {

			option = SELECTED::START;
			PlaySelectFx();
		}
		else if (x >= 450 && x <= 556 && y >= 180 && y <= 216) {

			option = SELECTED::CONTINUE;
			PlaySelectFx();
		}
		else if (x >= 450 && x <= 556  && y >= 240 && y <= 278) {

			option = SELECTED::OPTIONS;
			PlaySelectFx();
		}
		else if (x >= 450 && x <= 556 && y >= 300 && y <= 338) {

			option = SELECTED::EXIT;
			PlaySelectFx();
		}
		else {
			if (alreadyChangeFX)
				alreadyChangeFX = false;
		}

		// Buttons and gamepad
		if (option == SELECTED::NONE) {
			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) {
				option = SELECTED::START;
				PlaySelectFx();
			}
		}
		else if (option == SELECTED::START) {
			app->render->DrawTexture(Buttons, 450, 120, &B5);
			if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) {
				option = SELECTED::CONTINUE;
				PlaySelectFx();
			}
		}
		else if (option == SELECTED::CONTINUE) {
			app->render->DrawTexture(Buttons, 450, 180, &B6);
			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {
				option = SELECTED::START;
				PlaySelectFx();
			}
			if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) {
				option = SELECTED::OPTIONS;
				PlaySelectFx();
			}
		}
		else if (option == SELECTED::OPTIONS) {
			app->render->DrawTexture(Buttons, 450, 240, &B7);
			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {
				option = SELECTED::CONTINUE;
				PlaySelectFx();
			}
			if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) {
				option = SELECTED::EXIT;
				PlaySelectFx();
			}
		}
		else if (option == SELECTED::EXIT) {
			app->render->DrawTexture(Buttons, 450, 300, &B8);
			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {
				option = SELECTED::OPTIONS;
				PlaySelectFx();
			}
		}
		else {
			if (alreadyChangeFX)
				alreadyChangeFX = false;
		}
		if (option == SELECTED::NONE) {
			if (app->input->controllers.DPADU > 0 || app->input->controllers.DPADD > 0) {
				moveCounter++;
				if (moveCounter >= MOVE_DELAY) {
				option = SELECTED::START;
				PlaySelectFx();
				moveCounter = 0;
				}
			}
		}
		else if (option == SELECTED::START) {
			app->render->DrawTexture(Buttons, 450, 120, &B5);
			if (app->input->controllers.DPADD > 0) {
				moveCounter++;
				if (moveCounter >= MOVE_DELAY) {
				option = SELECTED::CONTINUE;
				PlaySelectFx();
				moveCounter = 0;
				}
			}
		}
		else if (option == SELECTED::CONTINUE) {
			app->render->DrawTexture(Buttons, 450, 180, &B6);
			if (app->input->controllers.DPADU > 0) {
				moveCounter++;
				if (moveCounter >= MOVE_DELAY) {
				option = SELECTED::START;
				PlaySelectFx();
				moveCounter = 0;
				}
			}
			if (app->input->controllers.DPADD > 0) {
				moveCounter++;
				if (moveCounter >= MOVE_DELAY) {
				option = SELECTED::OPTIONS;
				PlaySelectFx();
				moveCounter = 0;
				}
			}
		}
		else if (option == SELECTED::OPTIONS) {
			app->render->DrawTexture(Buttons, 450, 240, &B7);
			if (app->input->controllers.DPADU > 0) {
				moveCounter++;
				if (moveCounter >= MOVE_DELAY) {
				option = SELECTED::CONTINUE;
				PlaySelectFx();
				moveCounter = 0;
				}
			}
			if (app->input->controllers.DPADD > 0) {
				moveCounter++;
				if (moveCounter >= MOVE_DELAY) {
				option = SELECTED::EXIT;
				PlaySelectFx();
				moveCounter = 0;
				}
			}
		}
		else if (option == SELECTED::EXIT) {
			app->render->DrawTexture(Buttons, 450, 300, &B8);
			if (app->input->controllers.DPADU > 0) {
				moveCounter++;
				if (moveCounter >= MOVE_DELAY) {
				option = SELECTED::OPTIONS;
				PlaySelectFx();
				moveCounter = 0;
				}
			}
		}
		else {
			if (alreadyChangeFX)
				alreadyChangeFX = false;
		}
	}
	//Rectangulos negros para realizar los fades
	app->render->DrawRectangle({ 0,0,5000,5000 }, 0, 0, 0, fading);
	app->render->DrawRectangle({ 0,0,5000,5000 }, 0, 0, 0, fading2);

	return ret;
}

void MainMenu::PlaySelectFx()
{
	if (alreadyChangeFX == false) {
		app->audio->PlayFxWithVolume(change, 0, app->audio->fxvolume);
		alreadyChangeFX = true;
	}
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

	return true;
}

void MainMenu::Options(int posx, int posy)
{
	// Oscurecer todo el fondo
	app->render->DrawRectangle({ 0,0,1280,720 }, 0, 0, 0, 80);

	// UI opciones fondo
	app->render->DrawRectangle({ posx + 640 / 2 - 96, posy + 360 / 2 - 112, 96 * 2, 128 * 2 }, 0, 0, 0, 180);

	// posición ratón
	int x, y;
	x = app->input->GetMousePositionX();
	y = app->input->GetMousePositionY();
	app->render->DrawText(640 / 2 - 20, 44 * 2, WF, "Music", 16);
	app->render->DrawText(640 / 2 - 8, 71 * 2, WF, "Fx", 16);
	app->render->DrawText(640 / 2 - 32, 95 * 2, WF, "Fullscreen", 16);
	app->render->DrawText(640 / 2 - 24, 120 * 2, WF, "Vsync", 16);
	app->render->DrawText(640 / 2 - 16, 148 * 2, GF, "Back", 16);
	app->render->DrawRectangle({ posx + 6 + 125 * app->ScalingMultiplier, posy + 58 * app->ScalingMultiplier,64 * app->ScalingMultiplier,2 * app->ScalingMultiplier }, 150, 150, 150);
	app->render->DrawRectangle({ posx + 6 + MX * app->ScalingMultiplier, posy + 56 * app->ScalingMultiplier,5 * app->ScalingMultiplier,6 * app->ScalingMultiplier }, 150, 150, 150);
	app->render->DrawRectangle({ posx + 6 + 125 * app->ScalingMultiplier, posy + 83 * app->ScalingMultiplier,64 * app->ScalingMultiplier,2 * app->ScalingMultiplier }, 150, 150, 150);
	app->render->DrawRectangle({ posx + 6 + FX * app->ScalingMultiplier, posy + 81 * app->ScalingMultiplier,5 * app->ScalingMultiplier,6 * app->ScalingMultiplier }, 150, 150, 150);
	app->render->DrawRectangle({ posx + 6 + 152 * app->ScalingMultiplier, posy + 104 * app->ScalingMultiplier,10 * app->ScalingMultiplier,10 * app->ScalingMultiplier }, RGB, RGB, RGB);
	app->render->DrawRectangle({ posx + 6 + 152 * app->ScalingMultiplier, posy + 130 * app->ScalingMultiplier,10 * app->ScalingMultiplier,10 * app->ScalingMultiplier }, RGB1, RGB1, RGB1);
	app->render->DrawRectangle({ posx + 6 + 153 * app->ScalingMultiplier, posy + 105 * app->ScalingMultiplier,8 * app->ScalingMultiplier,8 * app->ScalingMultiplier }, 0, 0, 0);
	app->render->DrawRectangle({ posx + 6 + 153 * app->ScalingMultiplier, posy + 131 * app->ScalingMultiplier,8 * app->ScalingMultiplier,8 * app->ScalingMultiplier }, 0, 0, 0);
	app->render->DrawRectangle({ posx + 6 + 154 * app->ScalingMultiplier, posy + 106 * app->ScalingMultiplier,6 * app->ScalingMultiplier,6 * app->ScalingMultiplier }, 0, 200, 0);
	app->render->DrawRectangle({ posx + 6 + 154 * app->ScalingMultiplier, posy + 132 * app->ScalingMultiplier,6 * app->ScalingMultiplier,6 * app->ScalingMultiplier }, 0, 200, 0);
	//Cuadrados rojos
	if (FS == false) {
		app->render->DrawRectangle({ posx + 6 + 154 * app->ScalingMultiplier, posy + 106 * app->ScalingMultiplier,6 * app->ScalingMultiplier,6 * app->ScalingMultiplier }, 200, 0, 0);
	}
	if (VS == false) {
		app->scene->player->limitFPS = false;
		app->render->DrawRectangle({ posx + 6 + 154 * app->ScalingMultiplier, posy + 132 * app->ScalingMultiplier,6 * app->ScalingMultiplier,6 * app->ScalingMultiplier }, 200, 0, 0);
	}
	else
		app->scene->player->limitFPS = true;

	//Funciones para detectar el ratón
	if (x >= MX * app->ScalingMultiplier && x <= (MX * app->ScalingMultiplier + 6) && y >= 56 * app->ScalingMultiplier && y <= 62 * app->ScalingMultiplier) {
		app->render->DrawRectangle({ posx + 6 + 125 * app->ScalingMultiplier, posy + 58 * app->ScalingMultiplier,64 * app->ScalingMultiplier,2 * app->ScalingMultiplier }, 255, 255, 255);
		app->render->DrawRectangle({ posx + 6 + MX * app->ScalingMultiplier, posy + 56 * app->ScalingMultiplier,5 * app->ScalingMultiplier,6 * app->ScalingMultiplier }, 255, 255, 255);
		if (option == SELECTED::NONE) {
			option = SELECTED::MUSIC;
			app->audio->PlayFxWithVolume(change, 0, app->audio->fxvolume);
		}
		//Funcion para arrastrar el boton
		if (option == SELECTED::MUSIC && app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT) {
			MX = (x - 2) / app->ScalingMultiplier;
			if (x < 125 * app->ScalingMultiplier) MX = 125;
			if (x > 189 * app->ScalingMultiplier) MX = 189;
			if (MX > 189 * app->ScalingMultiplier) MX = 189;
			app->audio->volume = (MX - 125);
			if (MX <= 130) app->audio->volume = 1;
			if (MX >= 180) app->audio->volume = 100;
		}
	}
	else if (x >= FX * app->ScalingMultiplier && x <= (FX * app->ScalingMultiplier + 6) && y >= 81 * app->ScalingMultiplier && y <= 87 * app->ScalingMultiplier) {
		app->render->DrawRectangle({ posx + 6 + 125 * app->ScalingMultiplier, posy + 83 * app->ScalingMultiplier,64 * app->ScalingMultiplier,2 * app->ScalingMultiplier }, 255, 255, 255);
		app->render->DrawRectangle({ posx + 6 + FX * app->ScalingMultiplier, posy + 81 * app->ScalingMultiplier,5 * app->ScalingMultiplier,6 * app->ScalingMultiplier }, 255, 255, 255);
		if (option == SELECTED::NONE) {
			option = SELECTED::FX;
			app->audio->PlayFxWithVolume(change, 0, app->audio->fxvolume);
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
			app->audio->PlayFxWithVolume(change, 0, app->audio->fxvolume);
		}
		if (option == SELECTED::FS && app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN) {
			FS = !FS;
			app->win->fullscreen = !app->win->fullscreen;
			app->win->Fullscreen();
			app->audio->PlayFxWithVolume(select, 0, app->audio->fxvolume);
		}

	}
	else if (x >= 152 * app->ScalingMultiplier && x <= 162 * app->ScalingMultiplier && y >= 134 * app->ScalingMultiplier && y <= 144 * app->ScalingMultiplier) {
		RGB1 = 255;
		if (option == SELECTED::NONE) {
			option = SELECTED::VS;
			app->audio->PlayFxWithVolume(change, 0, app->audio->fxvolume);
		}
		if (option == SELECTED::VS && app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN) {
			VS = !VS;
			app->audio->PlayFxWithVolume(select, 0, app->audio->fxvolume);
		}

	}
	else if (x >= 145 * app->ScalingMultiplier && x <= 169 * app->ScalingMultiplier && y >= 145 * app->ScalingMultiplier && y <= 157 * app->ScalingMultiplier) {
		app->render->DrawText(640 / 2 - 16, 148 * 2, WF, "Back", 16);
		if (option == SELECTED::NONE) {
			option = SELECTED::BACK;
			app->audio->PlayFxWithVolume(change, 0, app->audio->fxvolume);
		}
		if (option == SELECTED::BACK && app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN) {
			options = false;
			app->scene->options = false;
			app->scene->option = Scene::SELECTED::NONE;
		}
	}
	else {
		RGB = 150;
		RGB1 = 150;
		option = SELECTED::NONE;
	}
}
