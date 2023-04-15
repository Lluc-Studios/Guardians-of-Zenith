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
#include "Player.h"

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
	Character2 = app->tex->Load("Assets/Entities/Characters/Character1.png");
	Character3 = app->tex->Load("Assets/Entities/Characters/Character1.png");
	Enemy1 = app->tex->Load("Assets/Entities/Enemies/Frog.png");
	Enemy2 = app->tex->Load("Assets/Entities/Enemies/Frog.png");
	Enemy3 = app->tex->Load("Assets/Entities/Enemies/Frog.png");

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
		if (CurrentCharacters >= 1) {
			//Draw player
			app->render->DrawTexture(Character1, app->scene->player->position.x - 100 , app->scene->player->position.y-60);
			app->font->BlitText(100 * app->ScalingMultiplier, 130 * app->ScalingMultiplier, WF, C1NAME);
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
			if (CurrentCharacters >= 2) {
				//Draw player
				app->render->DrawTexture(Character2, app->scene->player->position.x - 160, app->scene->player->position.y + 10);
				app->font->BlitText(180 * app->ScalingMultiplier, 130 * app->ScalingMultiplier, WF, C2NAME);
				app->font->BlitText(180 * app->ScalingMultiplier, 140 * app->ScalingMultiplier, WF, "hp");
				//Int to string convert
				char Aux[10];
				sprintf_s(Aux, "%d", C2MHP);
				app->font->BlitText(215 * app->ScalingMultiplier, 140 * app->ScalingMultiplier, WF, Aux);
				sprintf_s(Aux, "%d", C2CHP);
				app->font->BlitText(200 * app->ScalingMultiplier, 140 * app->ScalingMultiplier, WF, Aux);
				app->render->DrawRectangle({ app->scene->player->position.x + 70,app->scene->player->position.y + 115,100,10 }, 0, 0, 0);
				//Calculate hp bar length 
				int HpBarLengthC2 = (C2CHP * 98) / C2MHP;
				app->render->DrawRectangle({ app->scene->player->position.x + 71,app->scene->player->position.y + 116,HpBarLengthC2,8 }, 0, 200, 0);
				app->font->BlitText(180 * app->ScalingMultiplier, 160 * app->ScalingMultiplier, WF, "mp");
				sprintf_s(Aux, "%d", C2MMP);
				app->font->BlitText(215 * app->ScalingMultiplier, 160 * app->ScalingMultiplier, WF, Aux);
				sprintf_s(Aux, "%d", C2CMP);
				app->font->BlitText(200 * app->ScalingMultiplier, 160 * app->ScalingMultiplier, WF, Aux);
				app->render->DrawRectangle({ app->scene->player->position.x + 70,app->scene->player->position.y + 155,100,10 }, 0, 0, 0);
				//Calculate mp bar length
				int MpBarLengthC2 = (C2CMP * 98) / C2MMP;
				app->render->DrawRectangle({ app->scene->player->position.x + 71,app->scene->player->position.y + 156,MpBarLengthC2,8 }, 0, 0, 200);
				if (CurrentCharacters == 3) {
					//Draw player
					app->render->DrawTexture(Character3, app->scene->player->position.x - 160, app->scene->player->position.y - 120);
					app->font->BlitText(260 * app->ScalingMultiplier, 130 * app->ScalingMultiplier, WF, C3NAME);
					app->font->BlitText(260 * app->ScalingMultiplier, 140 * app->ScalingMultiplier, WF, "hp");
					//Int to string convert
					char Aux[10];
					sprintf_s(Aux, "%d", C3MHP);
					app->font->BlitText(295 * app->ScalingMultiplier, 140 * app->ScalingMultiplier, WF, Aux);
					sprintf_s(Aux, "%d", C3CHP);
					app->font->BlitText(280 * app->ScalingMultiplier, 140 * app->ScalingMultiplier, WF, Aux);
					app->render->DrawRectangle({ app->scene->player->position.x + 230,app->scene->player->position.y + 115,100,10 }, 0, 0, 0);
					//Calculate hp bar length 
					int HpBarLengthC3 = (C3CHP * 98) / C3MHP;
					app->render->DrawRectangle({ app->scene->player->position.x + 231,app->scene->player->position.y + 116,HpBarLengthC3,8 }, 0, 200, 0);
					app->font->BlitText(260 * app->ScalingMultiplier, 160 * app->ScalingMultiplier, WF, "mp");
					sprintf_s(Aux, "%d", C3MMP);
					app->font->BlitText(295 * app->ScalingMultiplier, 160 * app->ScalingMultiplier, WF, Aux);
					sprintf_s(Aux, "%d", C3CMP);
					app->font->BlitText(280 * app->ScalingMultiplier, 160 * app->ScalingMultiplier, WF, Aux);
					app->render->DrawRectangle({ app->scene->player->position.x + 230,app->scene->player->position.y + 155,100,10 }, 0, 0, 0);
					//Calculate mp bar length
					int MpBarLengthC3 = (C3CMP * 98) / C3MMP;
					app->render->DrawRectangle({ app->scene->player->position.x + 231,app->scene->player->position.y + 156,MpBarLengthC3,8 }, 0, 0, 200);
				}
			}
		}

		//Draw enemies and their hp
		if (CurrentEnemies >= 1) {
			//Draw enemy
			app->render->DrawTexture(Enemy1, app->scene->player->position.x + 120, app->scene->player->position.y - 75);
			//Int to string convert
			char Aux[10];
			sprintf_s(Aux, "%d", E1MHP);
			app->render->DrawRectangle({ app->scene->player->position.x + 120,app->scene->player->position.y - 5,64,10 }, 0, 0, 0);
			int HpBarLengthE1 = (E1CHP * 62) / E1MHP;
			app->render->DrawRectangle({ app->scene->player->position.x + 121,app->scene->player->position.y - 4,HpBarLengthE1,8 }, 0, 200, 0);
			if (CurrentEnemies >= 2) {
				//Draw enemy
				app->render->DrawTexture(Enemy2, app->scene->player->position.x + 200, app->scene->player->position.y - 130);
				//Int to string convert
				char Aux[10];
				sprintf_s(Aux, "%d", E2MHP);
				app->render->DrawRectangle({ app->scene->player->position.x + 200,app->scene->player->position.y - 60,64,10 }, 0, 0, 0);
				int HpBarLengthE2 = (E2CHP * 62) / E2MHP;
				app->render->DrawRectangle({ app->scene->player->position.x + 201,app->scene->player->position.y - 59,HpBarLengthE2,8 }, 0, 200, 0);
				if (CurrentEnemies == 3) {
					//Draw enemy
					app->render->DrawTexture(Enemy3, app->scene->player->position.x + 200, app->scene->player->position.y - 10);
					//Int to string convert
					char Aux[10];
					sprintf_s(Aux, "%d", E3MHP);
					app->render->DrawRectangle({ app->scene->player->position.x + 200,app->scene->player->position.y + 60,64,10 }, 0, 0, 0);
					int HpBarLengthE3 = (E3CHP * 62) / E3MHP;
					app->render->DrawRectangle({ app->scene->player->position.x + 201,app->scene->player->position.y + 61,HpBarLengthE3,8 }, 0, 200, 0);
				}
			}
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
	LoadLaurea(app->scene->player->laurea);
	LoadLapis(app->scene->player->lapis);
	LoadLucca(app->scene->player->lucca);
	LoadEnemy(app->entityManager->slimeFrog1);
	LoadEnemy(app->entityManager->slimeFrog2);
	LoadEnemy(app->entityManager->slimeFrog3);
	TurnOrder();
}

void Combat::ExitCombat()
{
	//Enable map print
	app->Instance = SaveInstance;
	app->scene->player->active = true;
	InCombat = false;
	CurrentCharacters = 0;
	CurrentEnemies = 0;
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

void Combat::TurnOrder()
{
	bool ordered = false;
	Turn[0] = C1speed;
	Turn[1] = C2speed;
	Turn[2] = C3speed;
	Turn[3] = E1speed;
	Turn[4] = E2speed;
	Turn[5] = E3speed;
	
	while (ordered == false) {
		int a = 0;
		for (int i = 0; i < 4; i++) {

			if (Turn[i] < Turn[i + 1]) {
				a++;
			}

			if (a == 0) ordered = true;
		}
		for (int i = 0; i < 4; i++) {
			int b;
			if (Turn[i] <= Turn[i + 1]) {
				b = Turn[i];
				Turn[i] = Turn[i + 1];
				Turn[i + 1] = b;
			}
		}
	}

	for (int i = 0; i < 6; i++) {
		if (Turn[i] == C1speed) {
			Turn[i] = 1;
		}
		else if (Turn[i] == C2speed) {
			Turn[i] = 2;
		}
		else if (Turn[i] == C3speed) {
			Turn[i] = 3;
		}
		else if (Turn[i] == E1speed) {
			Turn[i] = 4;
		}
		else if (Turn[i] == E2speed) {
			Turn[i] = 5;
		}
		else if (Turn[i] == E3speed) {
			Turn[i] = 6;
		}
	}
}

void Combat::LoadLaurea(Player::Laurea laurea)
{
	C1speed = laurea.spe;
	CurrentCharacters++;
	
	C1MHP = laurea.hp;
	C1CHP = laurea.chp;
	C1MMP = laurea.mp;
	C1CMP = laurea.cmp;
	C1ATK = laurea.atk;
	C1DEF = laurea.def;
	LIMIT1 = laurea.limit;
	C1NAME = laurea.name;
}

void Combat::LoadLapis(Player::Lapis lapis)
{
	C2speed = lapis.spe;
	CurrentCharacters++;

	C2MHP = lapis.hp;
	C2CHP = lapis.chp;
	C2MMP = lapis.mp;
	C2CMP = lapis.cmp;
	C2ATK = lapis.atk;
	C2DEF = lapis.def;
	LIMIT2 = lapis.limit;
	C2NAME = lapis.name;
}

void Combat::LoadLucca(Player::Lucca lucca)
{
	C3speed = lucca.spe;
	CurrentCharacters++;

	C3MHP = lucca.hp;
	C3CHP = lucca.chp;
	C3MMP = lucca.mp;
	C3CMP = lucca.cmp;
	C3ATK = lucca.atk;
	C3DEF = lucca.def;
	LIMIT3 = lucca.limit;
	C3NAME = lucca.name;
}

void Combat::LoadEnemy(EntityManager::CombatEnemy enemy)
{
	//Loading speed
	if (CurrentEnemies == 2) {
		E3speed = enemy.spe;
		CurrentEnemies++;

		E3MHP = enemy.hp;
		E3CHP = enemy.chp;
		E3ATK = enemy.atk;
		E3DEF = enemy.def;
		E3Weak = enemy.weakness;
		E3Res = enemy.resistance;
	}
	if (CurrentEnemies == 1) {
		E2speed = enemy.spe;
		CurrentEnemies++;

		E2MHP = enemy.hp;
		E2CHP = enemy.chp;
		E2ATK = enemy.atk;
		E2DEF = enemy.def;
		E2Weak = enemy.weakness;
		E2Res = enemy.resistance;
	}
	if (CurrentEnemies == 0) {
		E1speed = enemy.spe;
		CurrentEnemies++;

		E1MHP = enemy.hp;
		E1CHP = enemy.chp;
		E1ATK = enemy.atk;
		E1DEF = enemy.def;
		E1Weak = enemy.weakness;
		E1Res = enemy.resistance;
	}
}


