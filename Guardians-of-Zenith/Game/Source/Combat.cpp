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
	BG = app->tex->Load("Assets/Textures/Temporary.png");
	Character1 = app->tex->Load("Assets/Entities/Characters/Character1.png");

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
		app->render->DrawTexture(BG, app->scene->player->position.x-290, app->scene->player->position.y-250);
		app->font->BlitText(150 * app->ScalingMultiplier, 20 * app->ScalingMultiplier, WF, "turn");
		if (option == COMBATMENU::ATTACK) {
			app->font->BlitText(10 * app->ScalingMultiplier, 100 * app->ScalingMultiplier, WF, "attack");
			app->font->BlitText(10 * app->ScalingMultiplier, 120 * app->ScalingMultiplier, GF, "defend");
			app->font->BlitText(10 * app->ScalingMultiplier, 140 * app->ScalingMultiplier, GF, "inventory");
			app->font->BlitText(10 * app->ScalingMultiplier, 160 * app->ScalingMultiplier, GF, "escape");
		}
		if (option == COMBATMENU::DEFEND) {
			app->font->BlitText(10 * app->ScalingMultiplier, 100 * app->ScalingMultiplier, GF, "attack");
			app->font->BlitText(10 * app->ScalingMultiplier, 120 * app->ScalingMultiplier, WF, "defend");
			app->font->BlitText(10 * app->ScalingMultiplier, 140 * app->ScalingMultiplier, GF, "inventory");
			app->font->BlitText(10 * app->ScalingMultiplier, 160 * app->ScalingMultiplier, GF, "escape");
		}
		if (option == COMBATMENU::INVENTORY) {
			app->font->BlitText(10 * app->ScalingMultiplier, 100 * app->ScalingMultiplier, GF, "attack");
			app->font->BlitText(10 * app->ScalingMultiplier, 120 * app->ScalingMultiplier, GF, "defend");
			app->font->BlitText(10 * app->ScalingMultiplier, 140 * app->ScalingMultiplier, WF, "inventory");
			app->font->BlitText(10 * app->ScalingMultiplier, 160 * app->ScalingMultiplier, GF, "escape");
		}
		if (option == COMBATMENU::ESCAPE) {
			app->font->BlitText(10 * app->ScalingMultiplier, 100 * app->ScalingMultiplier, GF, "attack");
			app->font->BlitText(10 * app->ScalingMultiplier, 120 * app->ScalingMultiplier, GF, "defend");
			app->font->BlitText(10 * app->ScalingMultiplier, 140 * app->ScalingMultiplier, GF, "inventory");
			app->font->BlitText(10 * app->ScalingMultiplier, 160 * app->ScalingMultiplier, WF, "escape");
		}

		//Draw turns
		for (int i = 0; i < 6; i++) {
			if (Turn[i] <= 3) {
				app->render->DrawRectangle({ app->scene->player->position.x + (i * 21) - 40,app->scene->player->position.y - 165,20,20 }, 0, 0, 0);
				app->render->DrawRectangle({ app->scene->player->position.x + (i * 21) - 39,app->scene->player->position.y - 164,18,18 }, 0, 200, 0);
			}
			else if (Turn[i] >= 4) {
				app->render->DrawRectangle({ app->scene->player->position.x + (i * 21) - 40,app->scene->player->position.y - 165,20,20 }, 0, 0, 0);
				app->render->DrawRectangle({ app->scene->player->position.x + (i * 21) - 39,app->scene->player->position.y - 164,18,18 }, 200, 0, 0);
			}
		}

		//Draw allies and their stats
		if (CurrentCharacters == 1) {
			//Draw player
			app->render->DrawTexture(Character1, app->scene->player->position.x - 100 , app->scene->player->position.y-60);
			app->font->BlitText(100 * app->ScalingMultiplier, 130 * app->ScalingMultiplier, WF, "character1");
			app->font->BlitText(100 * app->ScalingMultiplier, 140 * app->ScalingMultiplier, WF, "hp");
			//Int to string convert
			char Aux[10];
			sprintf_s(Aux, "%d", C1MHP);
			app->font->BlitText(135 * app->ScalingMultiplier, 140 * app->ScalingMultiplier, WF, Aux);
			sprintf_s(Aux, "%d", C1CHP);
			app->font->BlitText(120 * app->ScalingMultiplier, 140 * app->ScalingMultiplier, WF, Aux);
			app->render->DrawRectangle({ app->scene->player->position.x-90,app->scene->player->position.y+115,100,10 }, 0, 0, 0);
			//Calculate hp bar length
			int HpBarLengthC1 = (C1CHP * 98) / C1MHP;
			app->render->DrawRectangle({ app->scene->player->position.x - 89,app->scene->player->position.y + 116,HpBarLengthC1,8 }, 0, 200, 0);
			app->font->BlitText(100 * app->ScalingMultiplier, 160 * app->ScalingMultiplier, WF, "mp");
			sprintf_s(Aux, "%d", C1MMP);
			app->font->BlitText(135 * app->ScalingMultiplier, 160 * app->ScalingMultiplier, WF, Aux);
			sprintf_s(Aux, "%d", C1CMP);
			app->font->BlitText(120 * app->ScalingMultiplier, 160 * app->ScalingMultiplier, WF, Aux);
			app->render->DrawRectangle({ app->scene->player->position.x-90,app->scene->player->position.y+155,100,10 }, 0, 0, 0);
			//Calculate mp bar length
			int MpBarLengthC1 = (C1CMP * 98) / C1MMP;
			app->render->DrawRectangle({ app->scene->player->position.x - 89,app->scene->player->position.y + 156,MpBarLengthC1,8 }, 0, 0, 200);
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
	if (app->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN) {
		FinishTurn();
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

void Combat::FinishTurn()
{
	int Aux = Turn[0];
	for (int i = 0; i < 6; i++) {
		if (i <= 4) {
			Turn[i] = Turn[i + 1];
		}
		else {
			Turn[i] = Aux;
		}
	};

}

