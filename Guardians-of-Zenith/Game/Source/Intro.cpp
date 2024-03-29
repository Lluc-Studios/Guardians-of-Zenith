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
	//logo = app->tex->Load("Assets/Textures/Scenes/LlucStudios.png");
	introFx = app->audio->LoadFx("Assets/Soundtrack/Fx/Intro_Emanem.wav");
	menuMusic = app->audio->LoadFx("Assets/Soundtrack/Music/Rocky Tundra OST Version.ogg");
	logoanim = app->tex->Load("Assets/Textures/Scenes/anim_logo.png");

	app->audio->PlayFxWithVolume(introFx);

	fadeo.PushBack({ 0, 0, 640, 360 }); //1
	fadeo.PushBack({ 640, 0, 640, 360 }); //2
	fadeo.PushBack({ 1280, 0, 640, 360 }); //3
	fadeo.PushBack({ 0, 360, 640, 360 }); //4
	fadeo.PushBack({ 640, 360, 640, 360 }); //5
	fadeo.PushBack({ 1280, 360, 640, 360 }); //6
	fadeo.PushBack({ 0, 720, 640, 360 }); //7
	fadeo.PushBack({ 640, 720, 640, 360 }); //8
	fadeo.PushBack({ 1280, 720, 640, 360 }); //9
	fadeo.PushBack({ 0, 1080, 640, 360 }); //10
	fadeo.PushBack({ 640, 1080, 640, 360 }); //11
	fadeo.PushBack({ 1280, 1080, 640, 360 }); //12
	fadeo.loop = false;
	fadeo.speed = 0.08f;

	currentAnim = &fadeo;

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

	currentAnim->Update();
	SDL_Rect rect = currentAnim->GetCurrentFrame();

	app->render->DrawTexture(logoanim, 0, 0, &rect);
	app->render->DrawRectangle({ 0,0,5000,5000 }, 0, 0, 0, fading);
	counter++;

	//Fade out
	if (counter >= 300) {
		fadeIn = false;
	}
	//Scene transition to menu
	if (fadeIn == false && fading == 255 && counter > 300) {
		//app->scene->active = true;
		//app->menu->active = true;
		//app->entityManager->active = true;
		//app->physics->active = true;
		app->mainmenu->active = true;
		app->intro->active = false;
		app->audio->PlayMusic("Assets/Soundtrack/Music/Rocky-Tundra-OST-Version.ogg");

		//app->audio->PlayFxWithVolume(menuMusic);

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
		app->audio->PlayMusic("Assets/Soundtrack/Music/Guidance-Island-OST-Version.ogg");
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
