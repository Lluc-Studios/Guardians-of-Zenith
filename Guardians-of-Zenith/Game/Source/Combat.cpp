#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Combat.h"
#include "Menu.h"
#include "MainMenu.h"
#include "Entitymanager.h"
#include "Scene.h"
#include "Physics.h"
#include "ModuleFonts.h"

#include "Defs.h"
#include "Log.h"

Combat::Combat() : Module()
{
	name.Create("Combat Module");
}

// Destructor
Combat::~Combat()
{}

// Called before render is available
bool Combat::Awake(pugi::xml_node& config)
{
	LOG("Loading Combat Module");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Combat::Start()
{
	char lookupTable[] = { "abcdefghijklmnopqrstuvwxyz0123456789" };
	WF = app->font->Load("Assets/Fonts/FontWhiteDef.png", lookupTable, 1);
	GF = app->font->Load("Assets/Fonts/FontGreyDef.png", lookupTable, 1);

	return true;
}

// Called each loop iteration
bool Combat::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Combat::Update(float dt)
{
	//Render text
	if (InCombat == true) {
		if (option == COMBATMENU::ATTACK) {
			app->font->BlitText(10, 100, WF, "attack");
			app->font->BlitText(10, 120, GF, "defend");
			app->font->BlitText(10, 140, GF, "inventory");
			app->font->BlitText(10, 160, GF, "escape");
		}
		if (option == COMBATMENU::DEFEND) {
			app->font->BlitText(10, 100, GF, "attack");
			app->font->BlitText(10, 120, WF, "defend");
			app->font->BlitText(10, 140, GF, "inventory");
			app->font->BlitText(10, 160, GF, "escape");
		}
		if (option == COMBATMENU::INVENTORY) {
			app->font->BlitText(10, 100, GF, "attack");
			app->font->BlitText(10, 120, GF, "defend");
			app->font->BlitText(10, 140, WF, "inventory");
			app->font->BlitText(10, 160, GF, "escape");
		}
		if (option == COMBATMENU::ESCAPE) {
			app->font->BlitText(10, 100, GF, "attack");
			app->font->BlitText(10, 120, GF, "defend");
			app->font->BlitText(10, 140, GF, "inventory");
			app->font->BlitText(10, 160, WF, "escape");
		}
	}

	//Inputs
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {
		if (option == COMBATMENU::DEFEND) {
			option = COMBATMENU::ATTACK;
		}
		if (option == COMBATMENU::INVENTORY) {
			option = COMBATMENU::DEFEND;
		}
		if (option == COMBATMENU::ESCAPE) {
			option = COMBATMENU::INVENTORY;
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) {
		if (option == COMBATMENU::INVENTORY) {
			option = COMBATMENU::ESCAPE;
		}
		if (option == COMBATMENU::DEFEND) {
			option = COMBATMENU::INVENTORY;
		}
		if (option == COMBATMENU::ATTACK) {
			option = COMBATMENU::DEFEND;
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		if (option == COMBATMENU::ATTACK) {

		}
		if (option == COMBATMENU::DEFEND) {

		}
		if (option == COMBATMENU::INVENTORY) {

		}
		if (option == COMBATMENU::ESCAPE) {
			ExitCombat();
		}

	}

	return true;
}

// Called each loop iteration
bool Combat::PostUpdate()
{
	bool ret = true;
	return ret;
}

// Called before quitting
bool Combat::CleanUp()
{
	LOG("Freeing combat");


	return true;
}

void Combat::StartCombat()  
{
	//Disable map print
	SaveInstance = app->Instance;
	app->Instance = -1;
	app->scene->player->active = false;
	InCombat = true;
}

void Combat::ExitCombat()
{
	//Enable map print
	app->Instance = SaveInstance;
	app->scene->player->active = true;
	InCombat = false;
}

