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
#include "time.h"

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
	//WF = app->font->Load("Assets/Fonts/FontWhiteDef.png", lookupTable, 1);
	//GF = app->font->Load("Assets/Fonts/FontGreyDef.png", lookupTable, 1);
	//YF = app->font->Load("Assets/Fonts/FontYellowDef.png", lookupTable, 1);
	BG = app->tex->Load("Assets/Textures/Temporary.png");
	Character1 = app->tex->Load("Assets/Entities/Characters/Character1.png");
	Character2 = app->tex->Load("Assets/Entities/Characters/Character2.png");
	Character3 = app->tex->Load("Assets/Entities/Characters/Character1.png");
	Character1Frozen = app->tex->Load("Assets/Entities/Characters/Character1Frozen.png");
	Character2Frozen = app->tex->Load("Assets/Entities/Characters/Character2Frozen.png");
	Character3Frozen = app->tex->Load("Assets/Entities/Characters/Character1Frozen.png");
	Enemy1 = app->tex->Load("Assets/Entities/Enemies/Frog.png");
	Enemy2 = app->tex->Load("Assets/Entities/Enemies/LilyFish.png");
	Enemy3 = app->tex->Load("Assets/Entities/Enemies/NaiadonGoddess.png");
	Enemy4 = app->tex->Load("Assets/Entities/Enemies/PoisonFrog.png");
	Enemy5 = app->tex->Load("Assets/Entities/Enemies/FireFrog.png");
	E1asset = app->tex->Load("Assets/Entities/Enemies/Frog.png");
	E2asset = app->tex->Load("Assets/Entities/Enemies/Frog.png");
	E3asset = app->tex->Load("Assets/Entities/Enemies/Frog.png");
	ClassTank = app->tex->Load("Assets/Textures/Type_Orange.png");
	ClassMage = app->tex->Load("Assets/Textures/Type_Purple.png");
	ClassArcher = app->tex->Load("Assets/Textures/Type_Green.png");
	ClassChart = app->tex->Load("Assets/Textures/Types.png");

	//Entities status effects loading
	Frozen = app->tex->Load("Assets/Textures/Effects/SnowFlakeBlue.png");
	Burn = app->tex->Load("Assets/Textures/Effects/Fire.png");
	BurnBlue = app->tex->Load("Assets/Textures/Effects/FireBlue.png");
	Poison = app->tex->Load("Assets/Textures/Effects/Poison.png");
	Bleed = app->tex->Load("Assets/Textures/Effects/BloodDrop.png");
	Electrified = app->tex->Load("Assets/Textures/Effects/Thunder.png");

	//Prevent memory leaks
	E1 = app->tex->Load("Assets/Textures/Type_Orange.png");
	E2 = app->tex->Load("Assets/Textures/Type_Orange.png");
	E3 = app->tex->Load("Assets/Textures/Type_Orange.png");


	//Random number generation
	srand((unsigned int)time(NULL));

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
	//button cd
	if (cd != 0) {
		cd++;
		if (cd >= 50) {
			cd = 0;
		}
	}

	if (InCombat == true) {

		//White fading
		if (WhiteFading < 200 && Fading == false) {
			WhiteFading += 5;
			if (WhiteFading >= 200) {
				WhiteFading = 200;
				Fading = true;
			}
		}
		if (Fading == true) {
			WhiteFading -= 5;
			if (WhiteFading <= 40) {
				WhiteFading = 40;
				Fading = false;
			}
		}
		//Freezing
		if (Turn[0] == 1 && C1FROZEN == true) {
			C1FROZEN = false;
			FinishTurn();
		}
		if (Turn[0] == 2 && C2FROZEN == true) {
			C2FROZEN = false;
			FinishTurn();
		}
		if (Turn[0] == 3 && C3FROZEN == true) {
			C3FROZEN = false;
			FinishTurn();
		}
		// Posion
		if (C1POISON != 0 && FinishedTurn1 == false && Turn[0] != 0) {
			C1POISON--;
			C1CHP -= C1MHP * 0.03;
			FinishedTurn1 = true;
		}
		if (C2POISON != 0 && FinishedTurn2 == false && Turn[0] != 0) {
			C2POISON--;
			C2CHP -= C2MHP * 0.03;
			FinishedTurn2 = true;
		}
		if (C3POISON != 0 && FinishedTurn3 == false && Turn[0] != 0) {
			C3POISON--;
			C3CHP -= C3MHP * 0.03;
			FinishedTurn3 = true;
		}
		//Burning
		if (C1BURN != 0 && FinishedTurnB1 == false && Turn[0] != 0) {
			C1BURN--;
			C1CHP -= 20;
			C1ATK = app->scene->player->laurea.atk * 0.7;
			FinishedTurnB1 = true;
		}
		if (C2BURN != 0 && FinishedTurnB2 == false && Turn[0] != 0) {
			C2BURN--;
			C2CHP -= 20;
			C2ATK = app->scene->player->lapis.atk * 0.7;
			FinishedTurnB2 = true;
		}
		if (C3BURN != 0 && FinishedTurnB3 == false && Turn[0] != 0) {
			C3BURN--;
			C3CHP -= 20;
			C3ATK = app->scene->player->lucca.atk * 0.7;
			FinishedTurnB3 = true;
		}
		if (C1BURN == 0) {
			C1ATK = app->scene->player->laurea.atk;
		}
		if (C2BURN == 0) {
			C2ATK = app->scene->player->lapis.atk;
		}
		if (C3BURN == 0) {
			C3ATK = app->scene->player->lucca.atk;
		}

		//Render text
		app->render->DrawTexture(BG, app->scene->player->position.x - 290, app->scene->player->position.y - 250);
		app->render->DrawTexture(ClassChart, app->scene->player->position.x-280, app->scene->player->position.y -170);
		app->render->DrawRectangle({ app->scene->player->position.x - 280,app->scene->player->position.y,115,160 }, 0, 0, 255, 150);
		if (option != COMBATMENU::NONE) {
			if (option == COMBATMENU::ATTACK && AttackMenu == false) {
				app->render->DrawText(10 * app->ScalingMultiplier, 100 * app->ScalingMultiplier, WF, "Attack", 16);
				app->render->DrawText(10 * app->ScalingMultiplier, 120 * app->ScalingMultiplier, GF, "Defend", 16);
				app->render->DrawText(10 * app->ScalingMultiplier, 140 * app->ScalingMultiplier, GF, "Inventory", 16);
				app->render->DrawText(10 * app->ScalingMultiplier, 160 * app->ScalingMultiplier, GF, "Escape", 16);
				app->render->DrawRectangle({ app->scene->player->position.x - 280,app->scene->player->position.y + 15,115,20 }, 255, 255, 255, WhiteFading);
			}
			if (option == COMBATMENU::DEFEND && AttackMenu == false) {
				app->render->DrawText(10 * app->ScalingMultiplier, 100 * app->ScalingMultiplier, GF, "Attack", 16);
				app->render->DrawText(10 * app->ScalingMultiplier, 120 * app->ScalingMultiplier, WF, "Defend", 16);
				app->render->DrawText(10 * app->ScalingMultiplier, 140 * app->ScalingMultiplier, GF, "Inventory", 16);
				app->render->DrawText(10 * app->ScalingMultiplier, 160 * app->ScalingMultiplier, GF, "Escape", 16);
				app->render->DrawRectangle({ app->scene->player->position.x - 280,app->scene->player->position.y + 55,115,20 }, 255, 255, 255, WhiteFading);
			}
			if (option == COMBATMENU::INVENTORY && AttackMenu == false) {
				app->render->DrawText(10 * app->ScalingMultiplier, 100 * app->ScalingMultiplier, GF, "Attack", 16);
				app->render->DrawText(10 * app->ScalingMultiplier, 120 * app->ScalingMultiplier, GF, "Defend", 16);
				app->render->DrawText(10 * app->ScalingMultiplier, 140 * app->ScalingMultiplier, WF, "Inventory", 16);
				app->render->DrawText(10 * app->ScalingMultiplier, 160 * app->ScalingMultiplier, GF, "Escape", 16);
				app->render->DrawRectangle({ app->scene->player->position.x - 280,app->scene->player->position.y + 95,115,20 }, 255, 255, 255, WhiteFading);
			}
			if (option == COMBATMENU::ESCAPE && AttackMenu == false) {
				app->render->DrawText(10 * app->ScalingMultiplier, 100 * app->ScalingMultiplier, GF, "Attack", 16);
				app->render->DrawText(10 * app->ScalingMultiplier, 120 * app->ScalingMultiplier, GF, "Defend", 16);
				app->render->DrawText(10 * app->ScalingMultiplier, 140 * app->ScalingMultiplier, GF, "Inventory", 16);
				app->render->DrawText(10 * app->ScalingMultiplier, 160 * app->ScalingMultiplier, WF, "Escape", 16);
				app->render->DrawRectangle({ app->scene->player->position.x - 280,app->scene->player->position.y + 135,115,20 }, 255, 255, 255, WhiteFading);
			}
			if (option == COMBATMENU::ATTACK1 && AttackMenu == true && EnemySelect == false) {
				app->render->DrawText(10 * app->ScalingMultiplier - 10, 100 * app->ScalingMultiplier, WF, Attack1, 16);
				app->render->DrawText(10 * app->ScalingMultiplier - 10, 120 * app->ScalingMultiplier, GF, Attack2, 16);
				app->render->DrawText(10 * app->ScalingMultiplier - 10, 140 * app->ScalingMultiplier, GF, Attack3, 16);
				app->render->DrawText(10 * app->ScalingMultiplier+95, 100 * app->ScalingMultiplier, WF, M1, 16);
				app->render->DrawText(10 * app->ScalingMultiplier+87, 120 * app->ScalingMultiplier, GF, M2, 16);
				if (Turn[0] == 1) {
					app->render->DrawText(10 * app->ScalingMultiplier + 87, 140 * app->ScalingMultiplier, WF, M3, 16);
				}
				if (Turn[0] == 2) {
					app->render->DrawText(10 * app->ScalingMultiplier + 76, 140 * app->ScalingMultiplier, WF, M3, 16);
				}
				if (Turn[0] == 3) {
					app->render->DrawText(10 * app->ScalingMultiplier + 76, 140 * app->ScalingMultiplier, WF, M3, 16);
				}
				app->render->DrawRectangle({ app->scene->player->position.x - 280,app->scene->player->position.y + 15,115,20 }, 255, 255, 255, WhiteFading);
			}
			if (option == COMBATMENU::ATTACK2 && AttackMenu == true && EnemySelect == false) {
				app->render->DrawText(10 * app->ScalingMultiplier-10, 100 * app->ScalingMultiplier, GF, Attack1, 16);
				app->render->DrawText(10 * app->ScalingMultiplier - 10, 120 * app->ScalingMultiplier, WF, Attack2, 16);
				app->render->DrawText(10 * app->ScalingMultiplier - 10, 140 * app->ScalingMultiplier, GF, Attack3, 16);
				app->render->DrawText(10 * app->ScalingMultiplier+95, 100 * app->ScalingMultiplier, GF, M1, 16);
				app->render->DrawText(10 * app->ScalingMultiplier+87, 120 * app->ScalingMultiplier, WF, M2, 16);
				if (Turn[0] == 1) {
					app->render->DrawText(10 * app->ScalingMultiplier + 87, 140 * app->ScalingMultiplier, WF, M3, 16);
				}
				if (Turn[0] == 2) {
					app->render->DrawText(10 * app->ScalingMultiplier + 76, 140 * app->ScalingMultiplier, WF, M3, 16);
				}
				if (Turn[0] == 3) {
					app->render->DrawText(10 * app->ScalingMultiplier + 76, 140 * app->ScalingMultiplier, WF, M3, 16);
				}
				app->render->DrawRectangle({ app->scene->player->position.x - 280,app->scene->player->position.y + 55,115,20 }, 255, 255, 255, WhiteFading);
			}
			if (option == COMBATMENU::ATTACK3 && AttackMenu == true && EnemySelect == false) {
				app->render->DrawText(10 * app->ScalingMultiplier - 10, 100 * app->ScalingMultiplier, GF, Attack1, 16);
				app->render->DrawText(10 * app->ScalingMultiplier - 10, 120 * app->ScalingMultiplier, GF, Attack2, 16);
				app->render->DrawText(10 * app->ScalingMultiplier - 10, 140 * app->ScalingMultiplier, WF, Attack3, 16);
				app->render->DrawText(10 * app->ScalingMultiplier + 95, 100 * app->ScalingMultiplier, GF, M1, 16);
				app->render->DrawText(10 * app->ScalingMultiplier + 87, 120 * app->ScalingMultiplier, GF, M2, 16);
				if (Turn[0] == 1) {
					app->render->DrawText(10 * app->ScalingMultiplier + 87, 140 * app->ScalingMultiplier, WF, M3, 16);
				}
				if (Turn[0] == 2) {
					app->render->DrawText(10 * app->ScalingMultiplier + 76, 140 * app->ScalingMultiplier, WF, M3, 16);
				}
				if (Turn[0] == 3) {
					app->render->DrawText(10 * app->ScalingMultiplier + 76, 140 * app->ScalingMultiplier, WF, M3, 16);
				}
				app->render->DrawRectangle({ app->scene->player->position.x - 280,app->scene->player->position.y + 95,115,20 }, 255, 255, 255, WhiteFading);
			}
			if (option == COMBATMENU::ENEMY1 && EnemySelect == true) {
				if (E1dead == true) option = COMBATMENU::ENEMY2;
				if (E1dead == false) {
					app->render->DrawText(10 * app->ScalingMultiplier, 100 * app->ScalingMultiplier, WF, E1name, 16);
				}
				if (E2dead == false) {
					app->render->DrawText(10 * app->ScalingMultiplier, 120 * app->ScalingMultiplier, GF, E2name, 16);
				}
				if (E3dead == false) {
					app->render->DrawText(10 * app->ScalingMultiplier, 140 * app->ScalingMultiplier, GF, E3name, 16);
				}
				app->render->DrawRectangle({ app->scene->player->position.x - 280,app->scene->player->position.y + 15,115,20 }, 255, 255, 255, WhiteFading);
				if (E1dead == false) {
					if (E1BOSS == 0) {
						app->render->DrawRectangle({ app->scene->player->position.x + 114 , app->scene->player->position.y - 71,66,66 }, 255, 255, 255, 120);
					}
					if (E1BOSS == 64) {
						app->render->DrawRectangle({ app->scene->player->position.x + 114 - E1BOSS, app->scene->player->position.y - 71 - E1BOSS,66 + E1BOSS,66 + E1BOSS }, 255, 255, 255, 120);
					}
				}
			}
			if (option == COMBATMENU::ENEMY2 && EnemySelect == true) {
				if (E2dead == true) option = COMBATMENU::ENEMY3;
				if (E1dead == false) {
					app->render->DrawText(10 * app->ScalingMultiplier, 100 * app->ScalingMultiplier, WF, E1name, 16);
				}
				if (E2dead == false) {
					app->render->DrawText(10 * app->ScalingMultiplier, 120 * app->ScalingMultiplier, GF, E2name, 16);
				}
				if (E3dead == false) {
					app->render->DrawText(10 * app->ScalingMultiplier, 140 * app->ScalingMultiplier, GF, E3name, 16);
				}
				app->render->DrawRectangle({ app->scene->player->position.x - 280,app->scene->player->position.y + 55,115,20 }, 255, 255, 255, WhiteFading);
				if (E2dead == false) {
					app->render->DrawRectangle({ app->scene->player->position.x + 199 , app->scene->player->position.y - 131,66,66 }, 255, 255, 255, 120);
				}
			}
			if (option == COMBATMENU::ENEMY3 && EnemySelect == true) {
				if (E3dead == true) option = COMBATMENU::ENEMY1;
				if (E1dead == false) {
					app->render->DrawText(10 * app->ScalingMultiplier, 100 * app->ScalingMultiplier, WF, E1name, 16);
				}
				if (E2dead == false) {
					app->render->DrawText(10 * app->ScalingMultiplier, 120 * app->ScalingMultiplier, GF, E2name, 16);
				}
				if (E3dead == false) {
					app->render->DrawText(10 * app->ScalingMultiplier, 140 * app->ScalingMultiplier, GF, E3name, 16);
				}
				app->render->DrawRectangle({ app->scene->player->position.x - 280,app->scene->player->position.y + 95,115,20 }, 255, 255, 255, WhiteFading);
				if (E3dead == false) {
					app->render->DrawRectangle({ app->scene->player->position.x + 199 , app->scene->player->position.y - 11,66,66 }, 255, 255, 255, 120);
				}
			}
		}


		//Draw turns
		TurnPos = 0;
		auxiliarPos = 0;
		for (int i = 0; i < 6; i++) {
			if (Turn[i] == 0) {
				if (C1Check == false && C1dead == true) {
				offset++;
				C1Check = true;
				}
				if (C2Check == false && C2dead == true) {
				offset++;
				C2Check = true;
				}
				if (C3Check == false && C3dead == true) {
				offset++;
				C3Check = true;
				}
				if (E1Check == false && E1dead == true) {
				offset++;
				E1Check = true;
				}
				if (E2Check == false && E2dead == true) {
				offset++;
				E2Check = true;
				}
				if (E3Check == false && E3dead == true) {
				offset++;
				E3Check = true;
				}
			}
			else if (Turn[i] >= 1 && Turn[i] <= 3) {
				TurnPos++;
				app->render->DrawRectangle({ ((app->scene->player->position.x + (21 * TurnPos) - 40) + offset * 10),app->scene->player->position.y - 165,20,20 }, 0, 0, 0);
				app->render->DrawRectangle({ ((app->scene->player->position.x + (21 * TurnPos) - 39) + offset * 10),app->scene->player->position.y - 164,18,18 }, 0, 200, 0);
			}
			else if (Turn[i] >= 4) {
				TurnPos++;
				app->render->DrawRectangle({ ((app->scene->player->position.x + (21 * TurnPos) - 40) + offset * 10),app->scene->player->position.y - 165,20,20 }, 0, 0, 0);
				app->render->DrawRectangle({ ((app->scene->player->position.x + (21 * TurnPos) - 39) + offset * 10),app->scene->player->position.y - 164,18,18 }, 200, 0, 0);
			}
		}

		//Draw allies and their stats
		if (CurrentCharacters >= 1) {
			//Draw player
			if (Turn[0] == 1) {
				app->render->DrawRectangle({ app->scene->player->position.x - 101 , app->scene->player->position.y - 61,64,66 }, 255, 255, 255, 120);
			}
			if (C1dead == false) {
				app->render->DrawTexture(Character1, app->scene->player->position.x - 100, app->scene->player->position.y - 60);
				if (C1FROZEN == true) {
					app->render->DrawTexture(Character1Frozen, app->scene->player->position.x - 100, app->scene->player->position.y - 60);
					app->render->DrawTexture(Frozen, app->scene->player->position.x - 40, app->scene->player->position.y + 77);
				}
				if (C1POISON != 0) {
					app->render->DrawTexture(Poison, app->scene->player->position.x - 20, app->scene->player->position.y + 77);
				}
				if (C1BURN != 0) {
					app->render->DrawTexture(Burn, app->scene->player->position.x, app->scene->player->position.y + 77);
				}
			}
			app->render->DrawText(100 * app->ScalingMultiplier, 130 * app->ScalingMultiplier, WF, C1NAME, 16);
			app->render->DrawTexture(ClassTank,app->scene->player->position.x-110, app->scene->player->position.y+77);
			sprintf_s(Aux, "%.0f", limitCount1);
			if (limitCount1 != LIMIT1) {
				app->render->DrawText(182, 280, WF, Aux, 16);
			}
			if (limitCount1 == LIMIT1) {
				app->render->DrawText(182, 280, YF, Aux, 16);
			}
			app->render->DrawText(100 * app->ScalingMultiplier, 140 * app->ScalingMultiplier, WF, "HP", 12);
			//Int to string convert
			sprintf_s(Aux, "/ %.0f", C1MHP);
			app->render->DrawText(130 * app->ScalingMultiplier, 140 * app->ScalingMultiplier, WF, Aux, 16);
			sprintf_s(Aux, "%.0f", C1CHP);
			app->render->DrawText(115 * app->ScalingMultiplier, 140 * app->ScalingMultiplier, WF, Aux, 16);
			app->render->DrawRectangle({ app->scene->player->position.x - 90,app->scene->player->position.y + 115,100,10 }, 0, 0, 0);
			//Calculate hp bar length
			HpBarLengthC1 = (C1CHP * 98) / C1MHP;
			app->render->DrawRectangle({ app->scene->player->position.x - 89,app->scene->player->position.y + 116,HpBarLengthC1,8 }, 0, 200, 0);
			app->render->DrawText(100 * app->ScalingMultiplier, 160 * app->ScalingMultiplier, WF, "MP", 12);
			sprintf_s(Aux, "/ %.0f", C1MMP);
			app->render->DrawText(130 * app->ScalingMultiplier, 160 * app->ScalingMultiplier, WF, Aux, 16);
			sprintf_s(Aux, "%.0f", C1CMP);
			app->render->DrawText(115 * app->ScalingMultiplier, 160 * app->ScalingMultiplier, WF, Aux, 16);
			app->render->DrawRectangle({ app->scene->player->position.x - 90,app->scene->player->position.y + 155,100,10 }, 0, 0, 0);
			//Calculate mp bar length
			MpBarLengthC1 = (C1CMP * 98) / C1MMP;
			app->render->DrawRectangle({ app->scene->player->position.x - 89,app->scene->player->position.y + 156,MpBarLengthC1,8 }, 0, 0, 200);
			if (CurrentCharacters >= 2) {
				//Draw player
				if (Turn[0] == 2) {
					app->render->DrawRectangle({ app->scene->player->position.x - 161 , app->scene->player->position.y + 9,64,66 }, 255, 255, 255, 120);
				}
				if (C2dead == false) {
					app->render->DrawTexture(Character2, app->scene->player->position.x - 160, app->scene->player->position.y + 10);
					if (C2FROZEN == true) {
						app->render->DrawTexture(Character2Frozen, app->scene->player->position.x - 160, app->scene->player->position.y + 10);
						app->render->DrawTexture(Frozen, app->scene->player->position.x + 110, app->scene->player->position.y + 77);
					}
					if (C2POISON != 0) {
						app->render->DrawTexture(Poison, app->scene->player->position.x + 130, app->scene->player->position.y + 77);
					}
					if (C2BURN != 0) {
						app->render->DrawTexture(Burn, app->scene->player->position.x, app->scene->player->position.y + 77);
					}
				}
				app->render->DrawText(180 * app->ScalingMultiplier, 130 * app->ScalingMultiplier, WF, C2NAME, 16);
				app->render->DrawTexture(ClassMage, app->scene->player->position.x + 50, app->scene->player->position.y + 77);
				sprintf_s(Aux, "%.0f", limitCount2);
				if (limitCount2 == LIMIT2) {
					app->render->DrawText(342, 280, YF, Aux, 16);
				}
				if (limitCount2 != LIMIT2) {
					app->render->DrawText(342, 280, WF, Aux, 16);
				}
				app->render->DrawText(180 * app->ScalingMultiplier, 140 * app->ScalingMultiplier, WF, "HP", 12);
				//Int to string convert
				sprintf_s(Aux, "/ %.0f", C2MHP);
				app->render->DrawText(210 * app->ScalingMultiplier, 140 * app->ScalingMultiplier, WF, Aux, 16);
				sprintf_s(Aux, "%.0f", C2CHP);
				app->render->DrawText(195 * app->ScalingMultiplier, 140 * app->ScalingMultiplier, WF, Aux, 16);
				app->render->DrawRectangle({ app->scene->player->position.x + 70,app->scene->player->position.y + 115,100,10 }, 0, 0, 0);
				//Calculate hp bar length 
				HpBarLengthC2 = (C2CHP * 98) / C2MHP;
				app->render->DrawRectangle({ app->scene->player->position.x + 71,app->scene->player->position.y + 116,HpBarLengthC2,8 }, 0, 200, 0);
				app->render->DrawText(175 * app->ScalingMultiplier+10, 160 * app->ScalingMultiplier, WF, "MP", 12);
				sprintf_s(Aux, "/ %.0f", C2MMP);
				app->render->DrawText(210 * app->ScalingMultiplier, 160 * app->ScalingMultiplier, WF, Aux, 16);
				sprintf_s(Aux, "%.0f", C2CMP);
				app->render->DrawText(200 * app->ScalingMultiplier-10, 160 * app->ScalingMultiplier, WF, Aux, 16);
				app->render->DrawRectangle({ app->scene->player->position.x + 70,app->scene->player->position.y + 155,100,10 }, 0, 0, 0);
				//Calculate mp bar length
				MpBarLengthC2 = (C2CMP * 98) / C2MMP;
				app->render->DrawRectangle({ app->scene->player->position.x + 71,app->scene->player->position.y + 156,MpBarLengthC2,8 }, 0, 0, 200);
				if (CurrentCharacters == 3) {
					//Draw player
					if (Turn[0] == 3) {
						app->render->DrawRectangle({ app->scene->player->position.x - 161 , app->scene->player->position.y - 121,64,66 }, 255, 255, 255, 120);
					}
					if (C3dead == false) {
						app->render->DrawTexture(Character3, app->scene->player->position.x - 160, app->scene->player->position.y - 120);
						if (C3FROZEN == true) {
							app->render->DrawTexture(Character3Frozen, app->scene->player->position.x - 160, app->scene->player->position.y - 120);
							app->render->DrawTexture(Frozen, app->scene->player->position.x + 270, app->scene->player->position.y + 77);
						}
						if (C3POISON != 0) {
							app->render->DrawTexture(Poison, app->scene->player->position.x + 290, app->scene->player->position.y + 77);
						}
						if (C3BURN != 0) {
							app->render->DrawTexture(Burn, app->scene->player->position.x, app->scene->player->position.y + 77);
						}
					}
					app->render->DrawText(260 * app->ScalingMultiplier, 130 * app->ScalingMultiplier, WF, C3NAME, 16);
					app->render->DrawTexture(ClassArcher, app->scene->player->position.x + 210, app->scene->player->position.y + 77);
					app->render->DrawText(260 * app->ScalingMultiplier, 140 * app->ScalingMultiplier, WF, "HP", 12);
					//Int to string convert
					sprintf_s(Aux, "/ %.0f", C3MHP);
					app->render->DrawText(295 * app->ScalingMultiplier, 140 * app->ScalingMultiplier, WF, Aux, 16);
					sprintf_s(Aux, "%.0f", C3CHP);
					app->render->DrawText(280 * app->ScalingMultiplier, 140 * app->ScalingMultiplier, WF, Aux, 16);
					app->render->DrawRectangle({ app->scene->player->position.x + 230,app->scene->player->position.y + 115,100,10 }, 0, 0, 0);
					//Calculate hp bar length 
					HpBarLengthC3 = (C3CHP * 98) / C3MHP;
					app->render->DrawRectangle({ app->scene->player->position.x + 231,app->scene->player->position.y + 116,HpBarLengthC3,8 }, 0, 200, 0);
					app->render->DrawText(260 * app->ScalingMultiplier, 160 * app->ScalingMultiplier, WF, "MP", 12);
					sprintf_s(Aux, "/ %.0f", C3MMP);
					app->render->DrawText(295 * app->ScalingMultiplier, 160 * app->ScalingMultiplier, WF, Aux, 16);
					sprintf_s(Aux, "%.0f", C3CMP);
					app->render->DrawText(280 * app->ScalingMultiplier, 160 * app->ScalingMultiplier, WF, Aux, 16);
					app->render->DrawRectangle({ app->scene->player->position.x + 230,app->scene->player->position.y + 155,100,10 }, 0, 0, 0);
					//Calculate mp bar length
					MpBarLengthC3 = (C3CMP * 98) / C3MMP;
					app->render->DrawRectangle({ app->scene->player->position.x + 231,app->scene->player->position.y + 156,MpBarLengthC3,8 }, 0, 0, 200);
				}
			}
		}

		//Draw enemies and their hp
		if (CurrentEnemies >= 1) {
			//Draw enemy
			if (E1dead == false) {
				//Draw enemy class
				if (E1class == 1) {
					E1 = ClassTank;
				}
				if (E1class == 2) {
					E1 = ClassMage;
				}
				if (E1class == 3) {
					E1 = ClassArcher;
				}
				app->render->DrawTexture(E1, app->scene->player->position.x + 95, app->scene->player->position.y - 3);
				if (Turn[0] == 4) {
					if (E1BOSS == 0) {
						app->render->DrawRectangle({ app->scene->player->position.x + 114 , app->scene->player->position.y - 71,66,66 }, 255, 255, 255, 120);
					}
					if (E1BOSS == 64) {
						app->render->DrawRectangle({ app->scene->player->position.x + 114 - E1BOSS, app->scene->player->position.y - 71 - E1BOSS,66 + E1BOSS,66 + E1BOSS }, 255, 255, 255, 120);
					}
				}
				app->render->DrawTexture(E1asset, app->scene->player->position.x + 115-E1BOSS, app->scene->player->position.y - 70-E1BOSS);
				//Int to string convert
				sprintf_s(Aux, "%.0f", E1MHP);
				app->render->DrawRectangle({ app->scene->player->position.x + 115,app->scene->player->position.y,64,10 }, 0, 0, 0);
				HpBarLengthE1 = (E1CHP * 62) / E1MHP;
				app->render->DrawRectangle({ app->scene->player->position.x + 116,app->scene->player->position.y + 1,HpBarLengthE1,8 }, 0, 200, 0);
				EsBarLengthE1 = (E1CES * 60) / E1MES;
				app->render->DrawRectangle({ app->scene->player->position.x + 117,app->scene->player->position.y + 2,EsBarLengthE1,6 }, 0, 255, 255);
			}
			if (CurrentEnemies >= 2) {
				//Draw enemy
				if (E2dead == false) {
					//Draw enemy class
					if (E2class == 1) {
						E2 = ClassTank;
					}
					if (E2class == 2) {
						E2 = ClassMage;
					}
					if (E2class == 3) {
						E2 = ClassArcher;
					}
					app->render->DrawTexture(E2, app->scene->player->position.x + 180, app->scene->player->position.y - 63);
					if (Turn[0] == 5) {
						app->render->DrawRectangle({ app->scene->player->position.x + 199 , app->scene->player->position.y - 131,66,66 }, 255, 255, 255, 120);
					}
					app->render->DrawTexture(E2asset, app->scene->player->position.x + 200 - E2BOSS, app->scene->player->position.y - 130 - E2BOSS);
					//Int to string convert
					sprintf_s(Aux, "%.0f", E2MHP);
					app->render->DrawRectangle({ app->scene->player->position.x + 200,app->scene->player->position.y - 60,64,10 }, 0, 0, 0);
					HpBarLengthE2 = (E2CHP * 62) / E2MHP;
					app->render->DrawRectangle({ app->scene->player->position.x + 201,app->scene->player->position.y - 59,HpBarLengthE2,8 }, 0, 200, 0);
				}
				if (CurrentEnemies == 3) {
					//Draw enemy
					if (E3dead == false) {
						//Draw enemy class
						if (E3class == 1) {
							E3 = ClassTank;
						}
						if (E3class == 2) {
							E3 = ClassMage;
						}
						if (E3class == 3) {
							E3 = ClassArcher;
						}
						app->render->DrawTexture(E3, app->scene->player->position.x + 180, app->scene->player->position.y + 57);
						if (Turn[0] == 6) {
							app->render->DrawRectangle({ app->scene->player->position.x + 199 , app->scene->player->position.y - 11,66,66 }, 255, 255, 255, 120);
						}
						app->render->DrawTexture(E3asset, app->scene->player->position.x + 200 - E3BOSS, app->scene->player->position.y - 10 - E3BOSS);
						//Int to string convert
						sprintf_s(Aux, "%.0f", E3MHP);
						app->render->DrawRectangle({ app->scene->player->position.x + 200,app->scene->player->position.y + 60,64,10 }, 0, 0, 0);
						HpBarLengthE3 = (E3CHP * 62) / E3MHP;
						app->render->DrawRectangle({ app->scene->player->position.x + 201,app->scene->player->position.y + 61,HpBarLengthE3,8 }, 0, 200, 0);
					}
				}
			}
		}
		//Make sure player hp does not go below 0
		if (C1CHP <= 0) {
			C1CHP = 0;
		}
		if (C2CHP <= 0) {
			C2CHP = 0;
		}
		if (C3CHP <= 0) {
			C3CHP = 0;
		}
		if (E1CHP <= 0) {
			E1CHP = 0;
		}
		if (E2CHP <= 0) {
			E2CHP = 0;
		}
		if (E3CHP <= 0) {
			E3CHP = 0;
		}

		if (C1CHP <= 0 && C1dead == false) {
			C1dead = true;
			RemoveEntityFromList(1);
		}
		if (C2CHP <= 0 && C2dead == false) {
			C2dead = true;
			RemoveEntityFromList(2);
		}
		if (C3CHP <= 0 && C3dead == false) {
			C3dead = true;
			RemoveEntityFromList(3);
		}
		if (E1CHP <= 0 && E1dead == false) {
			E1dead = true;
			RemoveEntityFromList(4);
		}
		if (E2CHP <= 0 && E2dead == false) {
			E2dead = true;
			RemoveEntityFromList(5);
		}
		if (E3CHP <= 0 && E3dead == false) {
			E3dead = true;
			RemoveEntityFromList(6);
		}

		if (Turn[0] == 0) {
			FinishTurn();
		}
		app->render->DrawText(160 * app->ScalingMultiplier-5, 20 * app->ScalingMultiplier, WF, "Turn", 16);


		//Load the characters attacks
		if (AttackMenu == true && TeamTurn == 1) {
			if (Turn[0] == 1) {
				Attack1 = C1A1name;
				Attack2 = C1A2name;
				sprintf_s(Aux1, "%.0f", C1A2mp);
				M2 = Aux1;
				Attack3 = C1A3name;
				sprintf_s(Aux2, "%.0f", C1A3mp);
				M3 = Aux2;
				//Attack4 = C1A4name;
				//Attack5 = C1A5name;
				//Attack6 = C1A6name;
			}
			if (Turn[0] == 2) {
				Attack1 = C2A1name;
				Attack2 = C2A2name;
				sprintf_s(Aux1, "%.0f", C2A2mp);
				M2 = Aux1;
				Attack3 = C2A3name;
				sprintf_s(Aux2, "%.0f", C2A3mp);
				M3 = Aux2;
				//Attack4 = C2A4name;
				//Attack5 = C2A5name;
				//Attack6 = C2A6name;
			}
			if (Turn[0] == 3) {
				Attack1 = C3A1name;
				Attack2 = C3A2name;
				sprintf_s(Aux1, "%.0f", C3A2mp);
				M2 = Aux1;
				//Attack3 = C3A3name;
				//Attack4 = C3A4name;
				//Attack5 = C3A5name;
				//Attack6 = C3A6name;
			}
		}


		//Inputs
		if (TeamTurn == 1) {
			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && TeamTurn == 1) {
				if (AttackMenu == false) {
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
				if (AttackMenu == true && EnemySelect == false) {
					if (option == COMBATMENU::ATTACK2) {
						option = COMBATMENU::ATTACK1;
					}
					if (option == COMBATMENU::ATTACK3) {
						option = COMBATMENU::ATTACK2;
					}
				}
				if (EnemySelect == true) {
					if (option == COMBATMENU::ENEMY2 && E1dead == false) {
						option = COMBATMENU::ENEMY1;
					}
					if (option == COMBATMENU::ENEMY3) {
						if (E2dead == false) {
							option = COMBATMENU::ENEMY2;
						}
						if (E2dead == true && E1dead == false) {
							option = COMBATMENU::ENEMY1;
						}
					}
				}
			}
			if (app->input->controllers.j1_y <= -32767 && TeamTurn == 1)
			{
				if (AttackMenu == false) {
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
				if (AttackMenu == true && EnemySelect == false) {
					if (option == COMBATMENU::ATTACK2) {
						option = COMBATMENU::ATTACK1;
					}
					if (option == COMBATMENU::ATTACK3) {
						option = COMBATMENU::ATTACK2;
					}
				}
				if (EnemySelect == true) {
					if (option == COMBATMENU::ENEMY2 && E1dead == false) {
						option = COMBATMENU::ENEMY1;
					}
					if (option == COMBATMENU::ENEMY3) {
						if (E2dead == false) {
							option = COMBATMENU::ENEMY2;
						}
						if (E2dead == true && E1dead == false) {
							option = COMBATMENU::ENEMY1;
						}
					}
				}
			}
			if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN && TeamTurn == 1) {
				if (AttackMenu == false) {
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
				if (AttackMenu == true && EnemySelect == false) {
					if (option == COMBATMENU::ATTACK2) {
						option = COMBATMENU::ATTACK3;
					}
					if (option == COMBATMENU::ATTACK1) {
						option = COMBATMENU::ATTACK2;
					}
				}
				if (EnemySelect == true) {
					if (option == COMBATMENU::ENEMY2 && E3dead == false) {
						option = COMBATMENU::ENEMY3;
					}
					if (option == COMBATMENU::ENEMY1) {
						if (E2dead == false) {
							option = COMBATMENU::ENEMY2;
						}
						if (E2dead == true && E3dead == false) {
							option = COMBATMENU::ENEMY3;
						}
					}
				}
			}
			if (app->input->controllers.j1_y >= 32767 && TeamTurn == 1)
			{
				if (AttackMenu == false) {
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
				if (AttackMenu == true && EnemySelect == false) {
					if (option == COMBATMENU::ATTACK2) {
						option = COMBATMENU::ATTACK3;
					}
					if (option == COMBATMENU::ATTACK1) {
						option = COMBATMENU::ATTACK2;
					}
				}
				if (EnemySelect == true) {
					if (option == COMBATMENU::ENEMY2 && E3dead == false) {
						option = COMBATMENU::ENEMY3;
					}
					if (option == COMBATMENU::ENEMY1) {
						if (E2dead == false) {
							option = COMBATMENU::ENEMY2;
						}
						if (E2dead == true && E3dead == false) {
							option = COMBATMENU::ENEMY3;
						}
					}
				}
			}
			if (app->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN && TeamTurn == 1) {
				FinishTurn();
			}
			if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN && TeamTurn == 1) {
				if (AttackMenu == true && EnemySelect == false) {
					AttackMenu = false;
					option = COMBATMENU::ATTACK;
				}
				else if (AttackMenu == true && EnemySelect == true) {
					EnemySelect = false;
					option = COMBATMENU::ATTACK1;
				}
			}
			if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && TeamTurn == 1) {
				if (AttackMenu == false) {
					if (option == COMBATMENU::ATTACK) {
						AttackMenu = true;
						option = COMBATMENU::ATTACK1;
						cd = 1;
					}
					if (option == COMBATMENU::DEFEND) {
						if (Turn[0] == 1) {
							C1Block = true;
							FinishTurn();
						}
						else if (Turn[0] == 2) {
							C2Block = true;
							FinishTurn();
						}
						else if (Turn[0] == 3) {
							C3Block = true;
							FinishTurn();
						}
					}
					if (option == COMBATMENU::INVENTORY) {

					}
					if (option == COMBATMENU::ESCAPE) {
						EXPwon = 0;
						ExitCombat();
					}
				}
				if (AttackMenu == true && cd == 0 && EnemySelect == false) {
					if (option == COMBATMENU::ATTACK1) {
						EnemySelect = true;
						AttackSelected = 1;
						option = COMBATMENU::ENEMY1;
						cd = 1;
					}
					if (option == COMBATMENU::ATTACK2) {
						EnemySelect = true;
						AttackSelected = 2;
						option = COMBATMENU::ENEMY1;
						cd = 1;
					}
					if (option == COMBATMENU::ATTACK3) {
						EnemySelect = true;
						AttackSelected = 3;
						option = COMBATMENU::ENEMY1;
						cd = 1;
					}
				}
				//Perform the attack
				if (EnemySelect == true && cd == 0) {
					if (option == COMBATMENU::ENEMY1 && E1dead == false) {
						if (Turn[0] == 1) {
							if (AttackSelected == 1) {
								if (E1CES == 0) {
									multiplier = 1;
									multiplier2 = 1;
									if (E1Weak == "Laurea") multiplier = 2;
									if (E1Res == "Laurea") multiplier2 = 2;
									E1CHP = E1CHP - (((C1A1dmg * (C1ATK / E1DEF)) * multiplier) / multiplier2);
									FinishTurn();
								}
								if (E1CES > 0) {
									multiplier = 1;
									multiplier2 = 1;
									if (E1Weak == "Laurea") multiplier = 2;
									if (E1Res == "Laurea") multiplier2 = 2;
									E1CES = E1CES - ((((C1A1dmg * (C1ATK / E1DEF)) * multiplier) / multiplier2) / 3);
									if (E1CES < 0) {
										E1CHP += E1CES;
										E1CES = 0;
									}
									FinishTurn();
								}
							}
							if (AttackSelected == 2 && C1CMP >= C1A2mp) {
								C1CMP -= C1A2mp;
								if (E1CES == 0) {
									multiplier = 1;
									multiplier2 = 1;
									if (E1Weak == "Laurea") multiplier = 2;
									if (E1Res == "Laurea") multiplier2 = 2;
									E1CHP = E1CHP - (((C1A2dmg * (C1ATK / E1DEF)) * multiplier) / multiplier2);
									FinishTurn();
								}
								if (E1CES > 0) {
									multiplier = 1;
									multiplier2 = 1;
									if (E1Weak == "Laurea") multiplier = 2;
									if (E1Res == "Laurea") multiplier2 = 2;
									E1CES = E1CES - ((((C1A2dmg * (C1ATK / E1DEF)) * multiplier) / multiplier2) / 3);
									if (E1CES < 0) {
										E1CHP += E1CES;
										E1CES = 0;
									}
									FinishTurn();
								}
							}
							if (AttackSelected == 3 && C1CMP >= C1A3mp && limitCount1 == LIMIT1) {
								limitCount1 = 0;
								C1CMP -= C1A3mp;
								if (E1CES == 0) {
									multiplier = 1;
									multiplier2 = 1;
									if (E1Weak == "Laurea") multiplier = 2;
									if (E1Res == "Laurea") multiplier2 = 2;
									E1CHP = E1CHP - (((C1A3dmg * (C1ATK / E1DEF)) * multiplier) / multiplier2);
									FinishTurn();
								}
								if (E1CES > 0) {
									multiplier = 1;
									multiplier2 = 1;
									if (E1Weak == "Laurea") multiplier = 2;
									if (E1Res == "Laurea") multiplier2 = 2;
									E1CES = E1CES - ((((C1A3dmg * (C1ATK / E1DEF)) * multiplier) / multiplier2) / 3);
									if (E1CES < 0) {
										E1CHP += E1CES;
										E1CES = 0;
									}
									FinishTurn();
								}
							}
						}
						if (Turn[0] == 2) {
							if (AttackSelected == 1) {
								if (E1CES == 0) {
									multiplier = 1;
									multiplier2 = 1;
									if (E1Weak == "Lapis") multiplier = 2;
									if (E1Res == "Lapis") multiplier2 = 2;
									E1CHP = E1CHP - (((C2A1dmg * (C2ATK / E1DEF)) * multiplier) / multiplier2);
									FinishTurn();
								}
								if (E1CES > 0) {
									multiplier = 1;
									multiplier2 = 1;
									if (E1Weak == "Lapis") multiplier = 2;
									if (E1Res == "Lapis") multiplier2 = 2;
									E1CES = E1CES - ((((C2A1dmg * (C2ATK / E1DEF)) * multiplier) / multiplier2) / 3);
									if (E1CES < 0) {
										E1CHP += E1CES;
										E1CES = 0;
									}
									FinishTurn();
								}
							}
							if (AttackSelected == 2 && C2CMP >= C2A2mp) {
								C2CMP -= C2A2mp;
								if (E1CES == 0) {
									multiplier = 1;
									multiplier2 = 1;
									if (E1Weak == "Lapis") multiplier = 2;
									if (E1Res == "Lapis") multiplier2 = 2;
									E1CHP = E1CHP - (((C2A2dmg * (C2ATK / E1DEF)) * multiplier) / multiplier2);
								}
								if (E1CES > 0) {
									multiplier = 1;
									multiplier2 = 1;
									if (E1Weak == "Lapis") multiplier = 2;
									if (E1Res == "Lapis") multiplier2 = 2;
									E1CES = E1CES - ((((C2A2dmg * (C2ATK / E1DEF)) * multiplier) / multiplier2) / 3);
									if (E1CES < 0) {
										E1CHP += E1CES;
										E1CES = 0;
									}
								}
								multiplier = 1;
								multiplier2 = 1;
								if (E2Weak == "Lapis") multiplier = 2;
								if (E2Res == "Lapis") multiplier2 = 2;
								E2CHP = E2CHP - (((C2A2dmg * (C2ATK / E2DEF)) * multiplier) / multiplier2);
								multiplier = 1;
								multiplier2 = 1;
								if (E3Weak == "Lapis") multiplier = 2;
								if (E3Res == "Lapis") multiplier2 = 2;
								E3CHP = E3CHP - (((C2A2dmg * (C2ATK / E3DEF)) * multiplier) / multiplier2);
								FinishTurn();
							}
							if (AttackSelected == 3 && C2CMP >= C2A3mp && limitCount2 == LIMIT2) {
								limitCount2 = 0;
								C2CMP -= C2A3mp;
								if (E1CES == 0) {
									multiplier = 1;
									multiplier2 = 1;
									if (E1Weak == "Lapis") multiplier = 2;
									if (E1Res == "Lapis") multiplier2 = 2;
									E1CHP = E1CHP - (((C2A3dmg * (C2ATK / E1DEF)) * multiplier) / multiplier2);
								}
								if (E1CES > 0) {
									multiplier = 1;
									multiplier2 = 1;
									if (E1Weak == "Lapis") multiplier = 2;
									if (E1Res == "Lapis") multiplier2 = 2;
									E1CES = E1CES - ((((C2A3dmg * (C2ATK / E1DEF)) * multiplier) / multiplier2) / 3);
									if (E1CES < 0) {
										E1CHP += E1CES;
										E1CES = 0;
									}
								}
								multiplier = 1;
								multiplier2 = 1;
								if (E2Weak == "Lapis") multiplier = 2;
								if (E2Res == "Lapis") multiplier2 = 2;
								E2CHP = E2CHP - (((C2A3dmg * (C2ATK / E2DEF)) * multiplier) / multiplier2);
								multiplier = 1;
								multiplier2 = 1;
								if (E3Weak == "Lapis") multiplier = 2;
								if (E3Res == "Lapis") multiplier2 = 2;
								E3CHP = E3CHP - (((C2A3dmg * (C2ATK / E3DEF)) * multiplier) / multiplier2);
								if (C1dead == false) {
									C1CHP += C1MHP * 0.5;
								}
								C2CHP += C2MHP * 0.5;
								if (C3dead == false) {
									C3CHP += C3MHP * 0.5;
								}
								if (C1CHP > C1MHP) {
									C1CHP = C1MHP;
								}
								if (C2CHP > C2MHP) {
									C2CHP = C2MHP;
								}
								if (C3CHP > C3MHP) {
									C3CHP = C3MHP;
								}
								FinishTurn();
							}
						}
						if (Turn[0] == 3) {
							if (AttackSelected == 1) {
								if (E1CES == 0) {
									multiplier = 1;
									multiplier2 = 1;
									if (E1Weak == "Lucca") multiplier = 2;
									if (E1Res == "Lucca") multiplier2 = 2;
									E1CHP = E1CHP - (((C3A1dmg * (C3ATK / E1DEF)) * multiplier) / multiplier2);
									FinishTurn();
								}
								if (E1CES > 0) {
									multiplier = 1;
									multiplier2 = 1;
									if (E1Weak == "Lucca") multiplier = 2;
									if (E1Res == "Lucca") multiplier2 = 2;
									E1CES = E1CES - ((((C3A1dmg * (C3ATK / E1DEF)) * multiplier) / multiplier2) / 3);
									if (E1CES < 0) {
										E1CHP += E1CES;
										E1CES = 0;
									}
									FinishTurn();
								}
							}
							if (AttackSelected == 2 && C3CMP >= C3A2mp) {
								C3CMP -= C3A2mp;
								if (E1CES == 0) {
									multiplier = 1;
									multiplier2 = 1;
									if (E1Weak == "Lucca") multiplier = 2;
									if (E1Res == "Lucca") multiplier2 = 2;
									E1CHP = E1CHP - (((C3A2dmg * (C3ATK / E1DEF)) * multiplier) / multiplier2);
								}
								if (E1CES > 0) {
									multiplier = 1;
									multiplier2 = 1;
									if (E1Weak == "Lucca") multiplier = 2;
									if (E1Res == "Lucca") multiplier2 = 2;
									E1CES = E1CES - ((((C3A2dmg * (C3ATK / E1DEF)) * multiplier) / multiplier2) / 3);
									if (E1CES < 0) {
										E1CHP += E1CES;
										E1CES = 0;
									}
								}
								multiplier = 1;
								multiplier2 = 1;
								if (E2Weak == "Lucca") multiplier = 2;
								if (E2Res == "Lucca") multiplier2 = 2;
								E2CHP = E2CHP - (((C3A2dmg * (C3ATK / E2DEF)) * multiplier) / multiplier2);
								multiplier = 1;
								multiplier2 = 1;
								if (E3Weak == "Lucca") multiplier = 2;
								if (E3Res == "Lucca") multiplier2 = 2;
								E3CHP = E3CHP - (((C3A2dmg * (C3ATK / E3DEF)) * multiplier) / multiplier2);
								FinishTurn();
							}
						}
					}
					if (option == COMBATMENU::ENEMY2 && E2dead == false) {
						if (Turn[0] == 1) {
							if (AttackSelected == 1) {
								multiplier = 1;
								multiplier2 = 1;
								if (E2Weak == "Laurea") multiplier = 2;
								if (E2Res == "Laurea") multiplier2 = 2;
								E2CHP = E2CHP - (((C1A1dmg * (C1ATK / E2DEF)) * multiplier) / multiplier2);
								FinishTurn();
							}
							if (AttackSelected == 2 && C1CMP >= C1A2mp) {
								C1CMP -= C1A2mp;
								multiplier = 1;
								multiplier2 = 1;
								if (E2Weak == "Laurea") multiplier = 2;
								if (E2Res == "Laurea") multiplier2 = 2;
								E2CHP = E2CHP - (((C1A2dmg * (C1ATK / E2DEF)) * multiplier) / multiplier2);
								FinishTurn();
							}
							if (AttackSelected == 3 && C1CMP >= C1A3mp && limitCount1 == LIMIT1) {
								limitCount1 = 0;
								C1CMP -= C1A3mp;
								multiplier = 1;
								multiplier2 = 1;
								if (E2Weak == "Laurea") multiplier = 2;
								if (E2Res == "Laurea") multiplier2 = 2;
								E2CHP = E2CHP - (((C1A3dmg * (C1ATK / E2DEF)) * multiplier) / multiplier2);
								FinishTurn();
							}
						}
						if (Turn[0] == 2) {
							if (AttackSelected == 1) {
								multiplier = 1;
								multiplier2 = 1;
								if (E2Weak == "Lapis") multiplier = 2;
								if (E2Res == "Lapis") multiplier2 = 2;
								E2CHP = E2CHP - (((C2A1dmg * (C2ATK / E2DEF)) * multiplier) / multiplier2);
								FinishTurn();
							}
							if (AttackSelected == 2 && C2CMP >= C2A2mp) {
								C2CMP -= C2A2mp;
								multiplier = 1;
								multiplier2 = 1;
								if (E1Weak == "Lapis") multiplier = 2;
								if (E1Res == "Lapis") multiplier2 = 2;
								E1CHP = E1CHP - (((C2A2dmg * (C2ATK / E1DEF)) * multiplier) / multiplier2);
								multiplier = 1;
								multiplier2 = 1;
								if (E2Weak == "Lapis") multiplier = 2;
								if (E2Res == "Lapis") multiplier2 = 2;
								E2CHP = E2CHP - (((C2A2dmg * (C2ATK / E2DEF)) * multiplier) / multiplier2);
								multiplier = 1;
								multiplier2 = 1;
								if (E3Weak == "Lapis") multiplier = 2;
								if (E3Res == "Lapis") multiplier2 = 2;
								E3CHP = E3CHP - (((C2A2dmg * (C2ATK / E3DEF)) * multiplier) / multiplier2);
								FinishTurn();
							}
							if (AttackSelected == 3 && C2CMP >= C2A3mp && limitCount2 == LIMIT2) {
								limitCount2 = 0;
								C2CMP -= C2A3mp;
								multiplier = 1;
								multiplier2 = 1;
								if (E1Weak == "Lapis") multiplier = 2;
								if (E1Res == "Lapis") multiplier2 = 2;
								E1CHP = E1CHP - (((C2A3dmg * (C2ATK / E1DEF)) * multiplier) / multiplier2);
								multiplier = 1;
								multiplier2 = 1;
								if (E2Weak == "Lapis") multiplier = 2;
								if (E2Res == "Lapis") multiplier2 = 2;
								E2CHP = E2CHP - (((C2A3dmg * (C2ATK / E2DEF)) * multiplier) / multiplier2);
								multiplier = 1;
								multiplier2 = 1;
								if (E3Weak == "Lapis") multiplier = 2;
								if (E3Res == "Lapis") multiplier2 = 2;
								E3CHP = E3CHP - (((C2A3dmg * (C2ATK / E3DEF)) * multiplier) / multiplier2);
								if (C1dead == false) {
									C1CHP += C1MHP * 0.5;
								}
								C2CHP += C2MHP * 0.5;
								if (C3dead == false) {
									C3CHP += C3MHP * 0.5;
								}
								if (C1CHP > C1MHP) {
									C1CHP = C1MHP;
								}
								if (C2CHP > C2MHP) {
									C2CHP = C2MHP;
								}
								if (C3CHP > C3MHP) {
									C3CHP = C3MHP;
								}
								FinishTurn();
							}
						}
						if (Turn[0] == 3) {
							if (AttackSelected == 1) {
								multiplier = 1;
								multiplier2 = 1;
								if (E2Weak == "Lucca") multiplier = 2;
								if (E2Res == "Lucca") multiplier2 = 2;
								E2CHP = E2CHP - (((C3A1dmg * (C3ATK / E2DEF)) * multiplier) / multiplier2);
								FinishTurn();
							}
							if (AttackSelected == 2 && C3CMP >= C3A2mp) {
								C3CMP -= C3A2mp;
								multiplier = 1;
								multiplier2 = 1;
								if (E1Weak == "Lucca") multiplier = 2;
								if (E1Res == "Lucca") multiplier2 = 2;
								E1CHP = E1CHP - (((C3A2dmg * (C3ATK / E1DEF)) * multiplier) / multiplier2);
								multiplier = 1;
								multiplier2 = 1;
								if (E2Weak == "Lucca") multiplier = 2;
								if (E2Res == "Lucca") multiplier2 = 2;
								E2CHP = E2CHP - (((C3A2dmg * (C3ATK / E2DEF)) * multiplier) / multiplier2);
								multiplier = 1;
								multiplier2 = 1;
								if (E3Weak == "Lucca") multiplier = 2;
								if (E3Res == "Lucca") multiplier2 = 2;
								E3CHP = E3CHP - (((C3A2dmg * (C3ATK / E3DEF)) * multiplier) / multiplier2);
								FinishTurn();
							}
						}
					}
					if (option == COMBATMENU::ENEMY3 && E3dead == false) {
						if (Turn[0] == 1) {
							if (AttackSelected == 1) {
								multiplier = 1;
								multiplier2 = 1;
								if (E3Weak == "Laurea") multiplier = 2;
								if (E3Res == "Laurea") multiplier2 = 2;
								E3CHP = E3CHP - (((C1A1dmg * (C1ATK / E3DEF)) * multiplier) / multiplier2);
								FinishTurn();
							}
							if (AttackSelected == 2 && C1CMP >= C1A2mp) {
								C1CMP -= C1A2mp;
								multiplier = 1;
								multiplier2 = 1;
								if (E3Weak == "Laurea") multiplier = 2;
								if (E3Res == "Laurea") multiplier2 = 2;
								E3CHP = E3CHP - (((C1A2dmg * (C1ATK / E3DEF)) * multiplier) / multiplier2);
								FinishTurn();
							}
							if (AttackSelected == 3 && C1CMP >= C1A3mp && limitCount1 == LIMIT1) {
								limitCount1 = 0;
								C1CMP -= C1A3mp;
								multiplier = 1;
								multiplier2 = 1;
								if (E3Weak == "Laurea") multiplier = 2;
								if (E3Res == "Laurea") multiplier2 = 2;
								E3CHP = E3CHP - (((C1A3dmg * (C1ATK / E3DEF)) * multiplier) / multiplier2);
								FinishTurn();
							}
						}
						if (Turn[0] == 2) {
							if (AttackSelected == 1) {
								multiplier = 1;
								multiplier2 = 1;
								if (E3Weak == "Lapis") multiplier = 2;
								if (E3Res == "Lapis") multiplier2 = 2;
								E3CHP = E3CHP - (((C2A1dmg * (C2ATK / E3DEF)) * multiplier) / multiplier2);
								FinishTurn();
							}
							if (AttackSelected == 2 && C2CMP >= C2A2mp) {
								C2CMP -= C2A2mp;
								multiplier = 1;
								multiplier2 = 1;
								if (E1Weak == "Lapis") multiplier = 2;
								if (E1Res == "Lapis") multiplier2 = 2;
								E1CHP = E1CHP - (((C2A2dmg * (C2ATK / E1DEF)) * multiplier) / multiplier2);
								multiplier = 1;
								multiplier2 = 1;
								if (E2Weak == "Lapis") multiplier = 2;
								if (E2Res == "Lapis") multiplier2 = 2;
								E2CHP = E2CHP - (((C2A2dmg * (C2ATK / E2DEF)) * multiplier) / multiplier2);
								multiplier = 1;
								multiplier2 = 1;
								if (E3Weak == "Lapis") multiplier = 2;
								if (E3Res == "Lapis") multiplier2 = 2;
								E3CHP = E3CHP - (((C2A2dmg * (C2ATK / E3DEF)) * multiplier) / multiplier2);
								FinishTurn();
							}
							if (AttackSelected == 3 && C2CMP >= C2A3mp && limitCount2 == LIMIT2) {
								limitCount2 = 0;
								C2CMP -= C2A3mp;
								multiplier = 1;
								multiplier2 = 1;
								if (E1Weak == "Lapis") multiplier = 2;
								if (E1Res == "Lapis") multiplier2 = 2;
								E1CHP = E1CHP - (((C2A3dmg * (C2ATK / E1DEF)) * multiplier) / multiplier2);
								multiplier = 1;
								multiplier2 = 1;
								if (E2Weak == "Lapis") multiplier = 2;
								if (E2Res == "Lapis") multiplier2 = 2;
								E2CHP = E2CHP - (((C2A3dmg * (C2ATK / E2DEF)) * multiplier) / multiplier2);
								multiplier = 1;
								multiplier2 = 1;
								if (E3Weak == "Lapis") multiplier = 2;
								if (E3Res == "Lapis") multiplier2 = 2;
								E3CHP = E3CHP - (((C2A3dmg * (C2ATK / E3DEF)) * multiplier) / multiplier2);
								if (C1dead == false) {
									C1CHP += C1MHP * 0.5;
								}
								C2CHP += C2MHP * 0.5;
								if (C3dead == false) {
									C3CHP += C3MHP * 0.5;
								}
								if (C1CHP > C1MHP) {
									C1CHP = C1MHP;
								}
								if (C2CHP > C2MHP) {
									C2CHP = C2MHP;
								}
								if (C3CHP > C3MHP) {
									C3CHP = C3MHP;
								}
								FinishTurn();
							}
						}
						if (Turn[0] == 3) {
							if (AttackSelected == 1) {
								multiplier = 1;
								multiplier2 = 1;
								if (E3Weak == "Lucca") multiplier = 2;
								if (E3Res == "Lucca") multiplier2 = 2;
								E3CHP = E3CHP - (((C3A1dmg * (C3ATK / E3DEF)) * multiplier) / multiplier2);
								FinishTurn();
							}
							if (AttackSelected == 2 && C3CMP >= C3A2mp) {
								C3CMP -= C3A2mp;
								multiplier = 1;
								multiplier2 = 1;
								if (E1Weak == "Lucca") multiplier = 2;
								if (E1Res == "Lucca") multiplier2 = 2;
								E1CHP = E1CHP - (((C3A2dmg * (C3ATK / E1DEF)) * multiplier) / multiplier2);
								multiplier = 1;
								multiplier2 = 1;
								if (E2Weak == "Lucca") multiplier = 2;
								if (E2Res == "Lucca") multiplier2 = 2;
								E2CHP = E2CHP - (((C3A2dmg * (C3ATK / E2DEF)) * multiplier) / multiplier2);
								multiplier = 1;
								multiplier2 = 1;
								if (E3Weak == "Lucca") multiplier = 2;
								if (E3Res == "Lucca") multiplier2 = 2;
								E3CHP = E3CHP - (((C3A2dmg * (C3ATK / E3DEF)) * multiplier) / multiplier2);
								FinishTurn();
							}
						}
					}
				}

			}
			if (app->input->controllers.A != 0 && !A_pressed)
			{
				if (AttackMenu == false) {
					if (option == COMBATMENU::ATTACK) {
						AttackMenu = true;
						option = COMBATMENU::ATTACK1;
						cd = 1;
					}
					if (option == COMBATMENU::DEFEND) {
						if (Turn[0] == 1) {
							C1Block = true;
							FinishTurn();
						}
						else if (Turn[0] == 2) {
							C2Block = true;
							FinishTurn();
						}
						else if (Turn[0] == 3) {
							C3Block = true;
							FinishTurn();
						}
					}
					if (option == COMBATMENU::INVENTORY) {

					}
					if (option == COMBATMENU::ESCAPE) {
						EXPwon = 0;
						ExitCombat();
					}
				}
				if (AttackMenu == true && cd == 0 && EnemySelect == false) {
					if (option == COMBATMENU::ATTACK1) {
						EnemySelect = true;
						AttackSelected = 1;
						option = COMBATMENU::ENEMY1;
						cd = 1;
					}
					if (option == COMBATMENU::ATTACK2) {
						EnemySelect = true;
						AttackSelected = 2;
						option = COMBATMENU::ENEMY1;
						cd = 1;
					}
					if (option == COMBATMENU::ATTACK3) {
						EnemySelect = true;
						AttackSelected = 3;
						option = COMBATMENU::ENEMY1;
						cd = 1;
					}
				}
				//Perform the attack
				if (EnemySelect == true && cd == 0) {
					if (option == COMBATMENU::ENEMY1 && E1dead == false) {
						if (Turn[0] == 1) {
							if (AttackSelected == 1) {
								if (E1CES == 0) {
									multiplier = 1;
									multiplier2 = 1;
									if (E1Weak == "Laurea") multiplier = 2;
									if (E1Res == "Laurea") multiplier2 = 2;
									E1CHP = E1CHP - (((C1A1dmg * (C1ATK / E1DEF)) * multiplier) / multiplier2);
									FinishTurn();
								}
								if (E1CES > 0) {
									multiplier = 1;
									multiplier2 = 1;
									if (E1Weak == "Laurea") multiplier = 2;
									if (E1Res == "Laurea") multiplier2 = 2;
									E1CES = E1CES - ((((C1A1dmg * (C1ATK / E1DEF)) * multiplier) / multiplier2) / 3);
									if (E1CES < 0) {
										E1CHP += E1CES;
										E1CES = 0;
									}
									FinishTurn();
								}
							}
							if (AttackSelected == 2 && C1CMP >= C1A2mp) {
								C1CMP -= C1A2mp;
								if (E1CES == 0) {
									multiplier = 1;
									multiplier2 = 1;
									if (E1Weak == "Laurea") multiplier = 2;
									if (E1Res == "Laurea") multiplier2 = 2;
									E1CHP = E1CHP - (((C1A2dmg * (C1ATK / E1DEF)) * multiplier) / multiplier2);
									FinishTurn();
								}
								if (E1CES > 0) {
									multiplier = 1;
									multiplier2 = 1;
									if (E1Weak == "Laurea") multiplier = 2;
									if (E1Res == "Laurea") multiplier2 = 2;
									E1CES = E1CES - ((((C1A2dmg * (C1ATK / E1DEF)) * multiplier) / multiplier2) / 3);
									if (E1CES < 0) {
										E1CHP += E1CES;
										E1CES = 0;
									}
									FinishTurn();
								}
							}
							if (AttackSelected == 3 && C1CMP >= C1A3mp && limitCount1 == LIMIT1) {
								limitCount1 = 0;
								C1CMP -= C1A3mp;
								if (E1CES == 0) {
									multiplier = 1;
									multiplier2 = 1;
									if (E1Weak == "Laurea") multiplier = 2;
									if (E1Res == "Laurea") multiplier2 = 2;
									E1CHP = E1CHP - (((C1A3dmg * (C1ATK / E1DEF)) * multiplier) / multiplier2);
									FinishTurn();
								}
								if (E1CES > 0) {
									multiplier = 1;
									multiplier2 = 1;
									if (E1Weak == "Laurea") multiplier = 2;
									if (E1Res == "Laurea") multiplier2 = 2;
									E1CES = E1CES - ((((C1A3dmg * (C1ATK / E1DEF)) * multiplier) / multiplier2) / 3);
									if (E1CES < 0) {
										E1CHP += E1CES;
										E1CES = 0;
									}
									FinishTurn();
								}
							}
						}
						if (Turn[0] == 2) {
							if (AttackSelected == 1) {
								if (E1CES == 0) {
									multiplier = 1;
									multiplier2 = 1;
									if (E1Weak == "Lapis") multiplier = 2;
									if (E1Res == "Lapis") multiplier2 = 2;
									E1CHP = E1CHP - (((C2A1dmg * (C2ATK / E1DEF)) * multiplier) / multiplier2);
									FinishTurn();
								}
								if (E1CES > 0) {
									multiplier = 1;
									multiplier2 = 1;
									if (E1Weak == "Lapis") multiplier = 2;
									if (E1Res == "Lapis") multiplier2 = 2;
									E1CES = E1CES - ((((C2A1dmg * (C2ATK / E1DEF)) * multiplier) / multiplier2) / 3);
									if (E1CES < 0) {
										E1CHP += E1CES;
										E1CES = 0;
									}
									FinishTurn();
								}
							}
							if (AttackSelected == 2 && C2CMP >= C2A2mp) {
								C2CMP -= C2A2mp;
								if (E1CES == 0) {
									multiplier = 1;
									multiplier2 = 1;
									if (E1Weak == "Lapis") multiplier = 2;
									if (E1Res == "Lapis") multiplier2 = 2;
									E1CHP = E1CHP - (((C2A2dmg * (C2ATK / E1DEF)) * multiplier) / multiplier2);
								}
								if (E1CES > 0) {
									multiplier = 1;
									multiplier2 = 1;
									if (E1Weak == "Lapis") multiplier = 2;
									if (E1Res == "Lapis") multiplier2 = 2;
									E1CES = E1CES - ((((C2A2dmg * (C2ATK / E1DEF)) * multiplier) / multiplier2) / 3);
									if (E1CES < 0) {
										E1CHP += E1CES;
										E1CES = 0;
									}
								}
								multiplier = 1;
								multiplier2 = 1;
								if (E2Weak == "Lapis") multiplier = 2;
								if (E2Res == "Lapis") multiplier2 = 2;
								E2CHP = E2CHP - (((C2A2dmg * (C2ATK / E2DEF)) * multiplier) / multiplier2);
								multiplier = 1;
								multiplier2 = 1;
								if (E3Weak == "Lapis") multiplier = 2;
								if (E3Res == "Lapis") multiplier2 = 2;
								E3CHP = E3CHP - (((C2A2dmg * (C2ATK / E3DEF)) * multiplier) / multiplier2);
								FinishTurn();
							}
							if (AttackSelected == 3 && C2CMP >= C2A3mp && limitCount2 == LIMIT2) {
								limitCount2 = 0;
								C2CMP -= C2A3mp;
								if (E1CES == 0) {
									multiplier = 1;
									multiplier2 = 1;
									if (E1Weak == "Lapis") multiplier = 2;
									if (E1Res == "Lapis") multiplier2 = 2;
									E1CHP = E1CHP - (((C2A3dmg * (C2ATK / E1DEF)) * multiplier) / multiplier2);
								}
								if (E1CES > 0) {
									multiplier = 1;
									multiplier2 = 1;
									if (E1Weak == "Lapis") multiplier = 2;
									if (E1Res == "Lapis") multiplier2 = 2;
									E1CES = E1CES - ((((C2A3dmg * (C2ATK / E1DEF)) * multiplier) / multiplier2) / 3);
									if (E1CES < 0) {
										E1CHP += E1CES;
										E1CES = 0;
									}
								}
								multiplier = 1;
								multiplier2 = 1;
								if (E2Weak == "Lapis") multiplier = 2;
								if (E2Res == "Lapis") multiplier2 = 2;
								E2CHP = E2CHP - (((C2A3dmg * (C2ATK / E2DEF)) * multiplier) / multiplier2);
								multiplier = 1;
								multiplier2 = 1;
								if (E3Weak == "Lapis") multiplier = 2;
								if (E3Res == "Lapis") multiplier2 = 2;
								E3CHP = E3CHP - (((C2A3dmg * (C2ATK / E3DEF)) * multiplier) / multiplier2);
								if (C1dead == false) {
									C1CHP += C1MHP * 0.5;
								}
								C2CHP += C2MHP * 0.5;
								if (C3dead == false) {
									C3CHP += C3MHP * 0.5;
								}
								if (C1CHP > C1MHP) {
									C1CHP = C1MHP;
								}
								if (C2CHP > C2MHP) {
									C2CHP = C2MHP;
								}
								if (C3CHP > C3MHP) {
									C3CHP = C3MHP;
								}
								FinishTurn();
							}
						}
						if (Turn[0] == 3) {
							if (AttackSelected == 1) {
								if (E1CES == 0) {
									multiplier = 1;
									multiplier2 = 1;
									if (E1Weak == "Lucca") multiplier = 2;
									if (E1Res == "Lucca") multiplier2 = 2;
									E1CHP = E1CHP - (((C3A1dmg * (C3ATK / E1DEF)) * multiplier) / multiplier2);
									FinishTurn();
								}
								if (E1CES > 0) {
									multiplier = 1;
									multiplier2 = 1;
									if (E1Weak == "Lucca") multiplier = 2;
									if (E1Res == "Lucca") multiplier2 = 2;
									E1CES = E1CES - ((((C3A1dmg * (C3ATK / E1DEF)) * multiplier) / multiplier2) / 3);
									if (E1CES < 0) {
										E1CHP += E1CES;
										E1CES = 0;
									}
									FinishTurn();
								}
							}
							if (AttackSelected == 2 && C3CMP >= C3A2mp) {
								C3CMP -= C3A2mp;
								if (E1CES == 0) {
									multiplier = 1;
									multiplier2 = 1;
									if (E1Weak == "Lucca") multiplier = 2;
									if (E1Res == "Lucca") multiplier2 = 2;
									E1CHP = E1CHP - (((C3A2dmg * (C3ATK / E1DEF)) * multiplier) / multiplier2);
								}
								if (E1CES > 0) {
									multiplier = 1;
									multiplier2 = 1;
									if (E1Weak == "Lucca") multiplier = 2;
									if (E1Res == "Lucca") multiplier2 = 2;
									E1CES = E1CES - ((((C3A2dmg * (C3ATK / E1DEF)) * multiplier) / multiplier2) / 3);
									if (E1CES < 0) {
										E1CHP += E1CES;
										E1CES = 0;
									}
								}
								multiplier = 1;
								multiplier2 = 1;
								if (E2Weak == "Lucca") multiplier = 2;
								if (E2Res == "Lucca") multiplier2 = 2;
								E2CHP = E2CHP - (((C3A2dmg * (C3ATK / E2DEF)) * multiplier) / multiplier2);
								multiplier = 1;
								multiplier2 = 1;
								if (E3Weak == "Lucca") multiplier = 2;
								if (E3Res == "Lucca") multiplier2 = 2;
								E3CHP = E3CHP - (((C3A2dmg * (C3ATK / E3DEF)) * multiplier) / multiplier2);
								FinishTurn();
							}
						}
					}
					if (option == COMBATMENU::ENEMY2 && E2dead == false) {
						if (Turn[0] == 1) {
							if (AttackSelected == 1) {
								multiplier = 1;
								multiplier2 = 1;
								if (E2Weak == "Laurea") multiplier = 2;
								if (E2Res == "Laurea") multiplier2 = 2;
								E2CHP = E2CHP - (((C1A1dmg * (C1ATK / E2DEF)) * multiplier) / multiplier2);
								FinishTurn();
							}
							if (AttackSelected == 2 && C1CMP >= C1A2mp) {
								C1CMP -= C1A2mp;
								multiplier = 1;
								multiplier2 = 1;
								if (E2Weak == "Laurea") multiplier = 2;
								if (E2Res == "Laurea") multiplier2 = 2;
								E2CHP = E2CHP - (((C1A2dmg * (C1ATK / E2DEF)) * multiplier) / multiplier2);
								FinishTurn();
							}
							if (AttackSelected == 3 && C1CMP >= C1A3mp && limitCount1 == LIMIT1) {
								limitCount1 = 0;
								C1CMP -= C1A3mp;
								multiplier = 1;
								multiplier2 = 1;
								if (E2Weak == "Laurea") multiplier = 2;
								if (E2Res == "Laurea") multiplier2 = 2;
								E2CHP = E2CHP - (((C1A3dmg * (C1ATK / E2DEF)) * multiplier) / multiplier2);
								FinishTurn();
							}
						}
						if (Turn[0] == 2) {
							if (AttackSelected == 1) {
								multiplier = 1;
								multiplier2 = 1;
								if (E2Weak == "Lapis") multiplier = 2;
								if (E2Res == "Lapis") multiplier2 = 2;
								E2CHP = E2CHP - (((C2A1dmg * (C2ATK / E2DEF)) * multiplier) / multiplier2);
								FinishTurn();
							}
							if (AttackSelected == 2 && C2CMP >= C2A2mp) {
								C2CMP -= C2A2mp;
								multiplier = 1;
								multiplier2 = 1;
								if (E1Weak == "Lapis") multiplier = 2;
								if (E1Res == "Lapis") multiplier2 = 2;
								E1CHP = E1CHP - (((C2A2dmg * (C2ATK / E1DEF)) * multiplier) / multiplier2);
								multiplier = 1;
								multiplier2 = 1;
								if (E2Weak == "Lapis") multiplier = 2;
								if (E2Res == "Lapis") multiplier2 = 2;
								E2CHP = E2CHP - (((C2A2dmg * (C2ATK / E2DEF)) * multiplier) / multiplier2);
								multiplier = 1;
								multiplier2 = 1;
								if (E3Weak == "Lapis") multiplier = 2;
								if (E3Res == "Lapis") multiplier2 = 2;
								E3CHP = E3CHP - (((C2A2dmg * (C2ATK / E3DEF)) * multiplier) / multiplier2);
								FinishTurn();
							}
							if (AttackSelected == 3 && C2CMP >= C2A3mp && limitCount2 == LIMIT2) {
								limitCount2 = 0;
								C2CMP -= C2A3mp;
								multiplier = 1;
								multiplier2 = 1;
								if (E1Weak == "Lapis") multiplier = 2;
								if (E1Res == "Lapis") multiplier2 = 2;
								E1CHP = E1CHP - (((C2A3dmg * (C2ATK / E1DEF)) * multiplier) / multiplier2);
								multiplier = 1;
								multiplier2 = 1;
								if (E2Weak == "Lapis") multiplier = 2;
								if (E2Res == "Lapis") multiplier2 = 2;
								E2CHP = E2CHP - (((C2A3dmg * (C2ATK / E2DEF)) * multiplier) / multiplier2);
								multiplier = 1;
								multiplier2 = 1;
								if (E3Weak == "Lapis") multiplier = 2;
								if (E3Res == "Lapis") multiplier2 = 2;
								E3CHP = E3CHP - (((C2A3dmg * (C2ATK / E3DEF)) * multiplier) / multiplier2);
								if (C1dead == false) {
									C1CHP += C1MHP * 0.5;
								}
								C2CHP += C2MHP * 0.5;
								if (C3dead == false) {
									C3CHP += C3MHP * 0.5;
								}
								if (C1CHP > C1MHP) {
									C1CHP = C1MHP;
								}
								if (C2CHP > C2MHP) {
									C2CHP = C2MHP;
								}
								if (C3CHP > C3MHP) {
									C3CHP = C3MHP;
								}
								FinishTurn();
							}
						}
						if (Turn[0] == 3) {
							if (AttackSelected == 1) {
								multiplier = 1;
								multiplier2 = 1;
								if (E2Weak == "Lucca") multiplier = 2;
								if (E2Res == "Lucca") multiplier2 = 2;
								E2CHP = E2CHP - (((C3A1dmg * (C3ATK / E2DEF)) * multiplier) / multiplier2);
								FinishTurn();
							}
							if (AttackSelected == 2 && C3CMP >= C3A2mp) {
								C3CMP -= C3A2mp;
								multiplier = 1;
								multiplier2 = 1;
								if (E1Weak == "Lucca") multiplier = 2;
								if (E1Res == "Lucca") multiplier2 = 2;
								E1CHP = E1CHP - (((C3A2dmg * (C3ATK / E1DEF)) * multiplier) / multiplier2);
								multiplier = 1;
								multiplier2 = 1;
								if (E2Weak == "Lucca") multiplier = 2;
								if (E2Res == "Lucca") multiplier2 = 2;
								E2CHP = E2CHP - (((C3A2dmg * (C3ATK / E2DEF)) * multiplier) / multiplier2);
								multiplier = 1;
								multiplier2 = 1;
								if (E3Weak == "Lucca") multiplier = 2;
								if (E3Res == "Lucca") multiplier2 = 2;
								E3CHP = E3CHP - (((C3A2dmg * (C3ATK / E3DEF)) * multiplier) / multiplier2);
								FinishTurn();
							}
						}
					}
					if (option == COMBATMENU::ENEMY3 && E3dead == false) {
						if (Turn[0] == 1) {
							if (AttackSelected == 1) {
								multiplier = 1;
								multiplier2 = 1;
								if (E3Weak == "Laurea") multiplier = 2;
								if (E3Res == "Laurea") multiplier2 = 2;
								E3CHP = E3CHP - (((C1A1dmg * (C1ATK / E3DEF)) * multiplier) / multiplier2);
								FinishTurn();
							}
							if (AttackSelected == 2 && C1CMP >= C1A2mp) {
								C1CMP -= C1A2mp;
								multiplier = 1;
								multiplier2 = 1;
								if (E3Weak == "Laurea") multiplier = 2;
								if (E3Res == "Laurea") multiplier2 = 2;
								E3CHP = E3CHP - (((C1A2dmg * (C1ATK / E3DEF)) * multiplier) / multiplier2);
								FinishTurn();
							}
							if (AttackSelected == 3 && C1CMP >= C1A3mp && limitCount1 == LIMIT1) {
								limitCount1 = 0;
								C1CMP -= C1A3mp;
								multiplier = 1;
								multiplier2 = 1;
								if (E3Weak == "Laurea") multiplier = 2;
								if (E3Res == "Laurea") multiplier2 = 2;
								E3CHP = E3CHP - (((C1A3dmg * (C1ATK / E3DEF)) * multiplier) / multiplier2);
								FinishTurn();
							}
						}
						if (Turn[0] == 2) {
							if (AttackSelected == 1) {
								multiplier = 1;
								multiplier2 = 1;
								if (E3Weak == "Lapis") multiplier = 2;
								if (E3Res == "Lapis") multiplier2 = 2;
								E3CHP = E3CHP - (((C2A1dmg * (C2ATK / E3DEF)) * multiplier) / multiplier2);
								FinishTurn();
							}
							if (AttackSelected == 2 && C2CMP >= C2A2mp) {
								C2CMP -= C2A2mp;
								multiplier = 1;
								multiplier2 = 1;
								if (E1Weak == "Lapis") multiplier = 2;
								if (E1Res == "Lapis") multiplier2 = 2;
								E1CHP = E1CHP - (((C2A2dmg * (C2ATK / E1DEF)) * multiplier) / multiplier2);
								multiplier = 1;
								multiplier2 = 1;
								if (E2Weak == "Lapis") multiplier = 2;
								if (E2Res == "Lapis") multiplier2 = 2;
								E2CHP = E2CHP - (((C2A2dmg * (C2ATK / E2DEF)) * multiplier) / multiplier2);
								multiplier = 1;
								multiplier2 = 1;
								if (E3Weak == "Lapis") multiplier = 2;
								if (E3Res == "Lapis") multiplier2 = 2;
								E3CHP = E3CHP - (((C2A2dmg * (C2ATK / E3DEF)) * multiplier) / multiplier2);
								FinishTurn();
							}
							if (AttackSelected == 3 && C2CMP >= C2A3mp && limitCount2 == LIMIT2) {
								limitCount2 = 0;
								C2CMP -= C2A3mp;
								multiplier = 1;
								multiplier2 = 1;
								if (E1Weak == "Lapis") multiplier = 2;
								if (E1Res == "Lapis") multiplier2 = 2;
								E1CHP = E1CHP - (((C2A3dmg * (C2ATK / E1DEF)) * multiplier) / multiplier2);
								multiplier = 1;
								multiplier2 = 1;
								if (E2Weak == "Lapis") multiplier = 2;
								if (E2Res == "Lapis") multiplier2 = 2;
								E2CHP = E2CHP - (((C2A3dmg * (C2ATK / E2DEF)) * multiplier) / multiplier2);
								multiplier = 1;
								multiplier2 = 1;
								if (E3Weak == "Lapis") multiplier = 2;
								if (E3Res == "Lapis") multiplier2 = 2;
								E3CHP = E3CHP - (((C2A3dmg * (C2ATK / E3DEF)) * multiplier) / multiplier2);
								if (C1dead == false) {
									C1CHP += C1MHP * 0.5;
								}
								C2CHP += C2MHP * 0.5;
								if (C3dead == false) {
									C3CHP += C3MHP * 0.5;
								}
								if (C1CHP > C1MHP) {
									C1CHP = C1MHP;
								}
								if (C2CHP > C2MHP) {
									C2CHP = C2MHP;
								}
								if (C3CHP > C3MHP) {
									C3CHP = C3MHP;
								}
								FinishTurn();
							}
						}
						if (Turn[0] == 3) {
							if (AttackSelected == 1) {
								multiplier = 1;
								multiplier2 = 1;
								if (E3Weak == "Lucca") multiplier = 2;
								if (E3Res == "Lucca") multiplier2 = 2;
								E3CHP = E3CHP - (((C3A1dmg * (C3ATK / E3DEF)) * multiplier) / multiplier2);
								FinishTurn();
							}
							if (AttackSelected == 2 && C3CMP >= C3A2mp) {
								C3CMP -= C3A2mp;
								multiplier = 1;
								multiplier2 = 1;
								if (E1Weak == "Lucca") multiplier = 2;
								if (E1Res == "Lucca") multiplier2 = 2;
								E1CHP = E1CHP - (((C3A2dmg * (C3ATK / E1DEF)) * multiplier) / multiplier2);
								multiplier = 1;
								multiplier2 = 1;
								if (E2Weak == "Lucca") multiplier = 2;
								if (E2Res == "Lucca") multiplier2 = 2;
								E2CHP = E2CHP - (((C3A2dmg * (C3ATK / E2DEF)) * multiplier) / multiplier2);
								multiplier = 1;
								multiplier2 = 1;
								if (E3Weak == "Lucca") multiplier = 2;
								if (E3Res == "Lucca") multiplier2 = 2;
								E3CHP = E3CHP - (((C3A2dmg * (C3ATK / E3DEF)) * multiplier) / multiplier2);
								FinishTurn();
							}
						}
					}
				}
				A_pressed = true;
			}
			else if (app->input->controllers.A == 0)
			{
				A_pressed = false;
			}
		}
		//Enemy action
		if (option == COMBATMENU::NONE) {
			if (EnemyAdone == false) {
				if ((E1dead == false && Turn[0] == 4) || (E2dead == false && Turn[0] == 5) || (E3dead == false && Turn[0] == 6)) {
					EnemyAttackTarget = rand() % CurrentCharacters + 1;
					EnemyAttackNum = rand() % 2 + 1;
					if (Turn[0] == 4 && E1BOSS == 64 && E1CHP < E1MHP) {
						EnemyAttackNum = rand() % 4 + 1;
					}
					if (Turn[0] == 4 && E1BOSS == 64 && E1CHP == E1MHP) {
						EnemyAttackNum = rand() % 3 + 1;
					}
					if (EnemyAttackTarget == 1 && C1dead == true) {
						EnemyAttackTarget = 2;
					}
					if (EnemyAttackTarget == 2 && C2dead == true) {
						EnemyAttackTarget = 3;
					}
					if (EnemyAttackTarget == 3 && C3dead == true) {
						if (C1dead == false) {
							EnemyAttackTarget = 1;
						}
						else if (C2dead == false) {
							EnemyAttackTarget = 2;
						}
					}
					if (Turn[0] == 6) {
						if (EnemyAttackTarget == 1) {
							if (EnemyAttackNum == 1) {
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E3A1target == 0) {
									if (E3class == 2) multiplier1 = 2;
									if (E3class == 3) multiplier2 = 2;
									if (C1Block == true) block = 3;
									C1CHP = C1CHP - ((E3A1dmg * (E3ATK / C1DEF) * multiplier1 / multiplier2) / block);
									if (E3A1effect == 2) C1POISON = 5;
									if (E3A1effect == 3) C1BURN = 3;
									Cname = C1NAME;
									Ename = E3name;
									Aname = E3A1name;
								}
								if (E3A1target == 1) {
									if (E3class == 2) multiplier1 = 2;
									if (E3class == 3) multiplier2 = 2;
									if (C1Block == true) block = 3;
									C1CHP = C1CHP - ((E3A1dmg * (E3ATK / C1DEF) * multiplier1 / multiplier2) / block);
									if (E3A1effect == 2) C1POISON = 5;
									if (E3A1effect == 3) C1BURN = 3;
									block = 1;
									multiplier1 = 1;
									multiplier2 = 1;
									if (E3class == 3) multiplier1 = 2;
									if (E3class == 1) multiplier2 = 2;
									if (C2Block == true) block = 3;
									C2CHP = C2CHP - ((E3A1dmg * (E3ATK / C2DEF) * multiplier1 / multiplier2) / block);
									if (E3A1effect == 2) C2POISON = 5;
									if (E3A1effect == 3) C2BURN = 3;
									block = 1;
									multiplier1 = 1;
									multiplier2 = 1;
									if (E3class == 1) multiplier1 = 2;
									if (E3class == 2) multiplier2 = 2;
									if (C3Block == true) block = 3;
									C3CHP = C3CHP - ((E3A1dmg * (E3ATK / C3DEF) * multiplier1 / multiplier2) / block);
									if (E3A1effect == 2) C3POISON = 5;
									if (E3A1effect == 3) C3BURN = 3;
									Cname = "everyone";
									Ename = E3name;
									Aname = E3A1name;
								}
							}
							if (EnemyAttackNum == 2) {
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E3A2target == 0) {
									if (E3class == 2) multiplier1 = 2;
									if (E3class == 3) multiplier2 = 2;
									if (C1Block == true) block = 3;
									C1CHP = C1CHP - ((E3A2dmg * (E3ATK / C1DEF) * multiplier1 / multiplier2) / block);
									if (E3A2effect == 2) C1POISON = 5;
									if (E3A2effect == 3) C1BURN = 3;
									Cname = C1NAME;
									Ename = E3name;
									Aname = E3A2name;
								}
								if (E3A2target == 1) {
									if (E3class == 2) multiplier1 = 2;
									if (E3class == 3) multiplier2 = 2;
									if (C1Block == true) block = 3;
									C1CHP = C1CHP - ((E3A2dmg * (E3ATK / C1DEF) * multiplier1 / multiplier2) / block);
									if (E3A2effect == 2) C1POISON = 5;
									if (E3A2effect == 3) C1BURN = 3;
									block = 1;
									multiplier1 = 1;
									multiplier2 = 1;
									if (E3class == 3) multiplier1 = 2;
									if (E3class == 1) multiplier2 = 2;
									if (C2Block == true) block = 3;
									C2CHP = C2CHP - ((E3A2dmg * (E3ATK / C2DEF) * multiplier1 / multiplier2) / block);
									if (E3A2effect == 2) C2POISON = 5;
									if (E3A2effect == 3) C2BURN = 3;
									block = 1;
									multiplier1 = 1;
									multiplier2 = 1;
									if (E3class == 1) multiplier1 = 2;
									if (E3class == 2) multiplier2 = 2;
									if (C3Block == true) block = 3;
									C3CHP = C3CHP - ((E3A2dmg * (E3ATK / C3DEF) * multiplier1 / multiplier2) / block);
									if (E3A2effect == 2) C3POISON = 5;
									if (E3A2effect == 3) C3BURN = 3;
									Cname = "everyone";
									Ename = E3name;
									Aname = E3A2name;
								}
							}
						}
						if (EnemyAttackTarget == 2) {
							if (EnemyAttackNum == 1) {
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E3A1target == 0) {
									if (E3class == 3) multiplier1 = 2;
									if (E3class == 1) multiplier2 = 2;
									if (C2Block == true) block = 3;
									C2CHP = C2CHP - ((E3A1dmg * (E3ATK / C2DEF) * multiplier1 / multiplier2) / block);
									if (E3A1effect == 2) C2POISON = 5;
									if (E3A1effect == 3) C2BURN = 3;
									Cname = C2NAME;
									Ename = E3name;
									Aname = E3A1name;
								}
								if (E3A1target == 1) {
									if (E3class == 2) multiplier1 = 2;
									if (E3class == 3) multiplier2 = 2;
									if (C1Block == true) block = 3;
									C1CHP = C1CHP - ((E3A1dmg * (E3ATK / C1DEF) * multiplier1 / multiplier2) / block);
									if (E3A1effect == 2) C1POISON = 5;
									if (E3A1effect == 3) C1BURN = 3;
									block = 1;
									multiplier1 = 1;
									multiplier2 = 1;
									if (E3class == 3) multiplier1 = 2;
									if (E3class == 1) multiplier2 = 2;
									if (C2Block == true) block = 3;
									C2CHP = C2CHP - ((E3A1dmg * (E3ATK / C2DEF) * multiplier1 / multiplier2) / block);
									if (E3A1effect == 2) C2POISON = 5;
									if (E3A1effect == 3) C2BURN = 3;
									block = 1;
									multiplier1 = 1;
									multiplier2 = 1;
									if (E3class == 1) multiplier1 = 2;
									if (E3class == 2) multiplier2 = 2;
									if (C3Block == true) block = 3;
									C3CHP = C3CHP - ((E3A1dmg * (E3ATK / C3DEF) * multiplier1 / multiplier2) / block);
									if (E3A1effect == 2) C3POISON = 5;
									if (E3A1effect == 3) C3BURN = 3;
									Cname = "everyone";
									Ename = E3name;
									Aname = E3A1name;
								}
							}
							if (EnemyAttackNum == 2) {
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E3A2target == 0) {
									if (E3class == 3) multiplier1 = 2;
									if (E3class == 1) multiplier2 = 2;
									if (C2Block == true) block = 3;
									C2CHP = C2CHP - ((E3A2dmg * (E3ATK / C2DEF) * multiplier1 / multiplier2) / block);
									if (E3A2effect == 2) C2POISON = 5;
									if (E3A2effect == 3) C2BURN = 3;
									Cname = C2NAME;
									Ename = E3name;
									Aname = E3A2name;
								}
								if (E3A2target == 1) {
									if (E3class == 2) multiplier1 = 2;
									if (E3class == 3) multiplier2 = 2;
									if (C1Block == true) block = 3;
									C1CHP = C1CHP - ((E3A2dmg * (E3ATK / C1DEF) * multiplier1 / multiplier2) / block);
									if (E3A2effect == 2) C1POISON = 5;
									if (E3A2effect == 3) C1BURN = 3;
									block = 1;
									multiplier1 = 1;
									multiplier2 = 1;
									if (E3class == 3) multiplier1 = 2;
									if (E3class == 1) multiplier2 = 2;
									if (C2Block == true) block = 3;
									C2CHP = C2CHP - ((E3A2dmg * (E3ATK / C2DEF) * multiplier1 / multiplier2) / block);
									if (E3A2effect == 2) C2POISON = 5;
									if (E3A2effect == 3) C2BURN = 3;
									block = 1;
									multiplier1 = 1;
									multiplier2 = 1;
									if (E3class == 1) multiplier1 = 2;
									if (E3class == 2) multiplier2 = 2;
									if (C3Block == true) block = 3;
									C3CHP = C3CHP - ((E3A2dmg * (E3ATK / C3DEF) * multiplier1 / multiplier2) / block);
									if (E3A2effect == 2) C3POISON = 5;
									if (E3A2effect == 3) C3BURN = 3;
									Cname = "everyone";
									Ename = E3name;
									Aname = E3A2name;
								}
							}
						}
						if (EnemyAttackTarget == 3) {
							if (EnemyAttackNum == 1) {
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E3A1target == 0) {
									if (E3class == 1) multiplier1 = 2;
									if (E3class == 2) multiplier2 = 2;
									if (C3Block == true) block = 3;
									C3CHP = C3CHP - ((E3A1dmg * (E3ATK / C3DEF) * multiplier1 / multiplier2) / block);
									if (E3A1effect == 2) C3POISON = 5;
									if (E3A1effect == 3) C3BURN = 3;
									Cname = C3NAME;
									Ename = E3name;
									Aname = E3A1name;
								}
								if (E3A1target == 1) {
									if (E3class == 2) multiplier1 = 2;
									if (E3class == 3) multiplier2 = 2;
									if (C1Block == true) block = 3;
									C1CHP = C1CHP - ((E3A1dmg * (E3ATK / C1DEF) * multiplier1 / multiplier2) / block);
									if (E3A1effect == 2) C1POISON = 5;
									if (E3A1effect == 3) C1BURN = 3;
									block = 1;
									multiplier1 = 1;
									multiplier2 = 1;
									if (E3class == 3) multiplier1 = 2;
									if (E3class == 1) multiplier2 = 2;
									if (C2Block == true) block = 3;
									C2CHP = C2CHP - ((E3A1dmg * (E3ATK / C2DEF) * multiplier1 / multiplier2) / block);
									if (E3A1effect == 2) C2POISON = 5;
									if (E3A1effect == 3) C2BURN = 3;
									block = 1;
									multiplier1 = 1;
									multiplier2 = 1;
									if (E3class == 1) multiplier1 = 2;
									if (E3class == 2) multiplier2 = 2;
									if (C3Block == true) block = 3;
									C3CHP = C3CHP - ((E3A1dmg * (E3ATK / C3DEF) * multiplier1 / multiplier2) / block);
									if (E3A1effect == 2) C3POISON = 5;
									if (E3A1effect == 3) C3BURN = 3;
									Cname = "everyone";
									Ename = E3name;
									Aname = E3A1name;
								}
							}
							if (EnemyAttackNum == 2) {
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E3A2target == 0) {
									if (E3class == 1) multiplier1 = 2;
									if (E3class == 2) multiplier2 = 2;
									if (C3Block == true) block = 3;
									C3CHP = C3CHP - ((E3A2dmg * (E3ATK / C3DEF) * multiplier1 / multiplier2) / block);
									if (E3A2effect == 2) C3POISON = 5;
									if (E3A2effect == 3) C3BURN = 3;
									Cname = C3NAME;
									Ename = E3name;
									Aname = E3A2name;
								}
								if (E3A2target == 1) {
									if (E3class == 2) multiplier1 = 2;
									if (E3class == 3) multiplier2 = 2;
									if (C1Block == true) block = 3;
									C1CHP = C1CHP - ((E3A2dmg * (E3ATK / C1DEF) * multiplier1 / multiplier2) / block);
									if (E3A2effect == 2) C1POISON = 5;
									if (E3A2effect == 3) C1BURN = 3;
									block = 1;
									multiplier1 = 1;
									multiplier2 = 1;
									if (E3class == 3) multiplier1 = 2;
									if (E3class == 1) multiplier2 = 2;
									if (C2Block == true) block = 3;
									C2CHP = C2CHP - ((E3A2dmg * (E3ATK / C2DEF) * multiplier1 / multiplier2) / block);
									if (E3A2effect == 2) C2POISON = 5;
									if (E3A2effect == 3) C2BURN = 3;
									block = 1;
									multiplier1 = 1;
									multiplier2 = 1;
									if (E3class == 1) multiplier1 = 2;
									if (E3class == 2) multiplier2 = 2;
									if (C3Block == true) block = 3;
									C3CHP = C3CHP - ((E3A2dmg * (E3ATK / C3DEF) * multiplier1 / multiplier2) / block);
									if (E3A2effect == 2) C3POISON = 5;
									if (E3A2effect == 3) C3BURN = 3;
									Cname = "everyone";
									Ename = E3name;
									Aname = E3A2name;
								}
							}
						}
					}
					else if (Turn[0] == 5) {
						if (EnemyAttackTarget == 1) {
							if (EnemyAttackNum == 1) {
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E2A1target == 0) {
									if (E2class == 2) multiplier1 = 2;
									if (E2class == 3) multiplier2 = 2;
									if (C1Block == true) block = 3;
									C1CHP = C1CHP - ((E2A1dmg * (E2ATK / C1DEF) * multiplier1 / multiplier2) / block);
									if (E2A1effect == 2) C1POISON = 5;
									if (E2A1effect == 3) C1BURN = 3;
									Cname = C1NAME;
									Ename = E2name;
									Aname = E2A1name;
								}
								if (E2A1target == 1) {
									if (E2class == 2) multiplier1 = 2;
									if (E2class == 3) multiplier2 = 2;
									if (C1Block == true) block = 3;
									C1CHP = C1CHP - ((E2A1dmg * (E2ATK / C1DEF) * multiplier1 / multiplier2) / block);
									if (E2A1effect == 2) C1POISON = 5;
									if (E2A1effect == 3) C1BURN = 3;
									block = 1;
									multiplier1 = 1;
									multiplier2 = 1;
									if (E2class == 3) multiplier1 = 2;
									if (E2class == 1) multiplier2 = 2;
									if (C2Block == true) block = 3;
									C2CHP = C2CHP - ((E2A1dmg * (E2ATK / C2DEF) * multiplier1 / multiplier2) / block);
									if (E2A1effect == 2) C2POISON = 5;
									if (E2A1effect == 3) C2BURN = 3;
									block = 1;
									multiplier1 = 1;
									multiplier2 = 1;
									if (E2class == 1) multiplier1 = 2;
									if (E2class == 2) multiplier2 = 2;
									if (C3Block == true) block = 3;
									C3CHP = C3CHP - ((E2A1dmg * (E2ATK / C3DEF) * multiplier1 / multiplier2) / block);
									if (E2A1effect == 2) C3POISON = 5;
									if (E2A1effect == 3) C3BURN = 3;
									Cname = "everyone";
									Ename = E2name;
									Aname = E2A1name;
								}
							}
							if (EnemyAttackNum == 2) {
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E2A2target == 0) {
									if (E2class == 2) multiplier1 = 2;
									if (E2class == 3) multiplier2 = 2;
									if (C1Block == true) block = 3;
									C1CHP = C1CHP - ((E2A2dmg * (E2ATK / C1DEF) * multiplier1 / multiplier2) / block);
									if (E2A2effect == 2) C1POISON = 5;
									if (E2A2effect == 3) C1BURN = 3;
									Cname = C1NAME;
									Ename = E2name;
									Aname = E2A2name;
								}
								if (E2A2target == 1) {
									if (E2class == 2) multiplier1 = 2;
									if (E2class == 3) multiplier2 = 2;
									if (C1Block == true) block = 3;
									C1CHP = C1CHP - ((E2A2dmg * (E2ATK / C1DEF) * multiplier1 / multiplier2) / block);
									if (E2A2effect == 2) C1POISON = 5;
									if (E2A2effect == 3) C1BURN = 3;
									block = 1;
									multiplier1 = 1;
									multiplier2 = 1;
									if (E2class == 3) multiplier1 = 2;
									if (E2class == 1) multiplier2 = 2;
									if (C2Block == true) block = 3;
									C2CHP = C2CHP - ((E2A2dmg * (E2ATK / C2DEF) * multiplier1 / multiplier2) / block);
									if (E2A2effect == 2) C2POISON = 5;
									if (E2A2effect == 3) C2BURN = 3;
									block = 1;
									multiplier1 = 1;
									multiplier2 = 1;
									if (E2class == 1) multiplier1 = 2;
									if (E2class == 2) multiplier2 = 2;
									if (C3Block == true) block = 3;
									C3CHP = C3CHP - ((E2A2dmg * (E2ATK / C3DEF) * multiplier1 / multiplier2) / block);
									if (E2A2effect == 2) C3POISON = 5;
									if (E2A2effect == 3) C3BURN = 3;
									Cname = "everyone";
									Ename = E2name;
									Aname = E2A2name;
								}
							}
						}
						if (EnemyAttackTarget == 2) {
							if (EnemyAttackNum == 1) {
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E2A1target == 0) {
									if (E2class == 3) multiplier1 = 2;
									if (E2class == 1) multiplier2 = 2;
									if (C2Block == true) block = 3;
									C2CHP = C2CHP - ((E2A1dmg * (E2ATK / C2DEF) * multiplier1 / multiplier2) / block);
									if (E2A1effect == 2) C2POISON = 5;
									if (E2A1effect == 3) C2BURN = 3;
									Cname = C2NAME;
									Ename = E2name;
									Aname = E2A1name;
								}
								if (E2A1target == 1) {
									if (E2class == 2) multiplier1 = 2;
									if (E2class == 3) multiplier2 = 2;
									if (C1Block == true) block = 3;
									C1CHP = C1CHP - ((E2A1dmg * (E2ATK / C1DEF) * multiplier1 / multiplier2) / block);
									if (E2A1effect == 2) C1POISON = 5;
									if (E2A1effect == 3) C1BURN = 3;
									block = 1;
									multiplier1 = 1;
									multiplier2 = 1;
									if (E2class == 3) multiplier1 = 2;
									if (E2class == 1) multiplier2 = 2;
									if (C2Block == true) block = 3;
									C2CHP = C2CHP - ((E2A1dmg * (E2ATK / C2DEF) * multiplier1 / multiplier2) / block);
									if (E2A1effect == 2) C2POISON = 5;
									if (E2A1effect == 3) C2BURN = 3;
									block = 1;
									multiplier1 = 1;
									multiplier2 = 1;
									if (E2class == 1) multiplier1 = 2;
									if (E2class == 2) multiplier2 = 2;
									if (C3Block == true) block = 3;
									C3CHP = C3CHP - ((E2A1dmg * (E2ATK / C3DEF) * multiplier1 / multiplier2) / block);
									if (E2A1effect == 2) C3POISON = 5;
									if (E2A1effect == 3) C3BURN = 3;
									Cname = "everyone";
									Ename = E2name;
									Aname = E2A1name;
								}
							}
							if (EnemyAttackNum == 2) {
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E2A2target == 0) {
									if (E2class == 3) multiplier1 = 2;
									if (E2class == 1) multiplier2 = 2;
									if (C2Block == true) block = 3;
									C2CHP = C2CHP - ((E2A2dmg * (E2ATK / C2DEF) * multiplier1 / multiplier2) / block);
									if (E2A2effect == 2) C2POISON = 5;
									if (E2A2effect == 3) C2BURN = 3;
									Cname = C2NAME;
									Ename = E2name;
									Aname = E2A2name;
								}
								if (E2A2target == 1) {
									if (E2class == 2) multiplier1 = 2;
									if (E2class == 3) multiplier2 = 2;
									if (C1Block == true) block = 3;
									C1CHP = C1CHP - ((E2A2dmg * (E2ATK / C1DEF) * multiplier1 / multiplier2) / block);
									if (E2A2effect == 2) C1POISON = 5;
									if (E2A2effect == 3) C1BURN = 3;
									block = 1;
									multiplier1 = 1;
									multiplier2 = 1;
									if (E2class == 3) multiplier1 = 2;
									if (E2class == 1) multiplier2 = 2;
									if (C2Block == true) block = 3;
									C2CHP = C2CHP - ((E2A2dmg * (E2ATK / C2DEF) * multiplier1 / multiplier2) / block);
									if (E2A2effect == 2) C2POISON = 5;
									if (E2A2effect == 3) C2BURN = 3;
									block = 1;
									multiplier1 = 1;
									multiplier2 = 1;
									if (E2class == 1) multiplier1 = 2;
									if (E2class == 2) multiplier2 = 2;
									if (C3Block == true) block = 3;
									C3CHP = C3CHP - ((E2A2dmg * (E2ATK / C3DEF) * multiplier1 / multiplier2) / block);
									if (E2A2effect == 2) C3POISON = 5;
									if (E2A2effect == 3) C3BURN = 3;
									Cname = "everyone";
									Ename = E2name;
									Aname = E2A2name;
								}
							}
						}
						if (EnemyAttackTarget == 3) {
							if (EnemyAttackNum == 1) {
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E2A1target == 0) {
									if (E2class == 1) multiplier1 = 2;
									if (E2class == 2) multiplier2 = 2;
									if (C3Block == true) block = 3;
									C3CHP = C3CHP - ((E2A1dmg * (E2ATK / C3DEF) * multiplier1 / multiplier2) / block);
									if (E2A1effect == 2) C3POISON = 5;
									if (E2A1effect == 3) C3BURN = 3;
									Cname = C3NAME;
									Ename = E2name;
									Aname = E2A1name;
								}
								if (E2A1target == 1) {
									if (E2class == 2) multiplier1 = 2;
									if (E2class == 3) multiplier2 = 2;
									if (C1Block == true) block = 3;
									C1CHP = C1CHP - ((E2A1dmg * (E2ATK / C1DEF) * multiplier1 / multiplier2) / block);
									if (E2A1effect == 2) C1POISON = 5;
									if (E2A1effect == 3) C1BURN = 3;
									block = 1;
									multiplier1 = 1;
									multiplier2 = 1;
									if (E2class == 3) multiplier1 = 2;
									if (E2class == 1) multiplier2 = 2;
									if (C2Block == true) block = 3;
									C2CHP = C2CHP - ((E2A1dmg * (E2ATK / C2DEF) * multiplier1 / multiplier2) / block);
									if (E2A1effect == 2) C2POISON = 5;
									if (E2A1effect == 3) C2BURN = 3;
									block = 1;
									multiplier1 = 1;
									multiplier2 = 1;
									if (E2class == 1) multiplier1 = 2;
									if (E2class == 2) multiplier2 = 2;
									if (C3Block == true) block = 3;
									C3CHP = C3CHP - ((E2A1dmg * (E2ATK / C3DEF) * multiplier1 / multiplier2) / block);
									if (E2A1effect == 2) C3POISON = 5;
									if (E2A1effect == 3) C3BURN = 3;
									Cname = "everyone";
									Ename = E2name;
									Aname = E2A1name;
								}
							}
							if (EnemyAttackNum == 2) {
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E2A2target == 0) {
									if (E2class == 1) multiplier1 = 2;
									if (E2class == 2) multiplier2 = 2;
									if (C3Block == true) block = 3;
									C3CHP = C3CHP - ((E2A2dmg * (E2ATK / C3DEF) * multiplier1 / multiplier2) / block);
									if (E2A2effect == 2) C3POISON = 5;
									if (E2A2effect == 3) C3BURN = 3;
									Cname = C3NAME;
									Ename = E2name;
									Aname = E2A2name;
								}
								if (E2A2target == 1) {
									if (E2class == 2) multiplier1 = 2;
									if (E2class == 3) multiplier2 = 2;
									if (C1Block == true) block = 3;
									C1CHP = C1CHP - ((E2A2dmg * (E2ATK / C1DEF) * multiplier1 / multiplier2) / block);
									if (E2A2effect == 2) C1POISON = 5;
									if (E2A2effect == 3) C1BURN = 3;
									block = 1;
									multiplier1 = 1;
									multiplier2 = 1;
									if (E2class == 3) multiplier1 = 2;
									if (E2class == 1) multiplier2 = 2;
									if (C2Block == true) block = 3;
									C2CHP = C2CHP - ((E2A2dmg * (E2ATK / C2DEF) * multiplier1 / multiplier2) / block);
									if (E2A2effect == 2) C2POISON = 5;
									if (E2A2effect == 3) C2BURN = 3;
									block = 1;
									multiplier1 = 1;
									multiplier2 = 1;
									if (E2class == 1) multiplier1 = 2;
									if (E2class == 2) multiplier2 = 2;
									if (C3Block == true) block = 3;
									C3CHP = C3CHP - ((E2A2dmg * (E2ATK / C3DEF) * multiplier1 / multiplier2) / block);
									if (E2A2effect == 2) C3POISON = 5;
									if (E2A2effect == 3) C3BURN = 3;
									Cname = "everyone";
									Ename = E2name;
									Aname = E2A2name;
								}
							}
						}
					}
					else if (Turn[0] == 4) {
						if (EnemyAttackTarget == 1) {
							if (EnemyAttackNum == 1) {
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E1A1target == 0) {
									if (E1class == 2) multiplier1 = 2;
									if (E1class == 3) multiplier2 = 2;
									if (C1Block == true) block = 3;
									C1CHP = C1CHP - ((E1A1dmg * (E1ATK / C1DEF) * multiplier1 / multiplier2) / block);
									if (E1A1effect == 2) C1POISON = 5;
									Cname = C1NAME;
									Ename = E1name;
									Aname = E1A1name;
								}
								if (E1A1target == 1) {
									if (E1class == 2) multiplier1 = 2;
									if (E1class == 3) multiplier2 = 2;
									if (C1Block == true) block = 3;
									C1CHP = C1CHP - ((E1A1dmg * (E1ATK / C1DEF) * multiplier1 / multiplier2) / block);
									if (E1A1effect == 2) C1POISON = 5;
									block = 1;
									multiplier1 = 1;
									multiplier2 = 1;
									if (E1class == 3) multiplier1 = 2;
									if (E1class == 1) multiplier2 = 2;
									if (C2Block == true) block = 3;
									C2CHP = C2CHP - ((E1A1dmg * (E1ATK / C2DEF) * multiplier1 / multiplier2) / block);
									if (E1A1effect == 2) C2POISON = 5;
									block = 1;
									multiplier1 = 1;
									multiplier2 = 1;
									if (E1class == 1) multiplier1 = 2;
									if (E1class == 2) multiplier2 = 2;
									if (C3Block == true) block = 3;
									C3CHP = C3CHP - ((E1A1dmg * (E1ATK / C3DEF) * multiplier1 / multiplier2) / block);
									if (E1A1effect == 2) C3POISON = 5;
									Cname = "everyone";
									Ename = E1name;
									Aname = E1A1name;
								}
							}
							if (EnemyAttackNum == 2) {
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E1A2target == 0) {
									if (E1class == 2) multiplier1 = 2;
									if (E1class == 3) multiplier2 = 2;
									if (C1Block == true) block = 3;
									C1CHP = C1CHP - ((E1A2dmg * (E1ATK / C1DEF) * multiplier1 / multiplier2) / block);
									if (E1A2effect == 2) C1POISON = 5;
									Cname = C1NAME;
									Ename = E1name;
									Aname = E1A2name;
								}
								if (E1A2target == 1) {
									if (E1class == 2) multiplier1 = 2;
									if (E1class == 3) multiplier2 = 2;
									if (C1Block == true) block = 3;
									C1CHP = C1CHP - ((E1A2dmg * (E1ATK / C1DEF) * multiplier1 / multiplier2) / block);
									if (E1A2effect == 2) C1POISON = 5;
									block = 1;
									multiplier1 = 1;
									multiplier2 = 1;
									if (E1class == 3) multiplier1 = 2;
									if (E1class == 1) multiplier2 = 2;
									if (C2Block == true) block = 3;
									C2CHP = C2CHP - ((E1A2dmg * (E1ATK / C2DEF) * multiplier1 / multiplier2) / block);
									if (E1A2effect == 2) C2POISON = 5;
									block = 1;
									multiplier1 = 1;
									multiplier2 = 1;
									if (E1class == 1) multiplier1 = 2;
									if (E1class == 2) multiplier2 = 2;
									if (C3Block == true) block = 3;
									C3CHP = C3CHP - ((E1A2dmg * (E1ATK / C3DEF) * multiplier1 / multiplier2) / block);
									if (E1A2effect == 2) C3POISON = 5;
									Cname = "everyone";
									Ename = E1name;
									Aname = E1A2name;
								}
							}
							if (EnemyAttackNum == 3) {
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E1class == 2) multiplier1 = 2;
								if (E1class == 3) multiplier2 = 2;
								if (C1Block == true) block = 3;
								C1CHP = C1CHP - ((E1A3dmg * (E1ATK / C1DEF) * multiplier1 / multiplier2) / block);
								if (E1A3effect == 1) {
									C1FROZEN = true;
								}
								if (E1A3effect == 2) C1POISON = 5;
								Cname = C1NAME;
								Ename = E1name;
								Aname = E1A3name;
							}
							if (EnemyAttackNum == 4) {
								E1CHP += E1MHP * 0.15;
								if (E1CHP > E1MHP) E1CHP = E1MHP;
								Cname = E1name;
								Ename = E1name;
								Aname = E1A4name;
							}
						}
						if (EnemyAttackTarget == 2) {
							if (EnemyAttackNum == 1) {
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E1A1target == 0) {
									if (E1class == 3) multiplier1 = 2;
									if (E1class == 1) multiplier2 = 2;
									if (C2Block == true) block = 3;
									C2CHP = C2CHP - ((E1A1dmg * (E1ATK / C2DEF) * multiplier1 / multiplier2) / block);
									if (E1A1effect == 2) C2POISON = 5;
									Cname = C2NAME;
									Ename = E1name;
									Aname = E1A1name;
								}
								if (E1A1target == 1) {
									if (E1class == 2) multiplier1 = 2;
									if (E1class == 3) multiplier2 = 2;
									if (C1Block == true) block = 3;
									C1CHP = C1CHP - ((E1A1dmg * (E1ATK / C1DEF) * multiplier1 / multiplier2) / block);
									if (E1A1effect == 2) C1POISON = 5;
									block = 1;
									multiplier1 = 1;
									multiplier2 = 1;
									if (E1class == 3) multiplier1 = 2;
									if (E1class == 1) multiplier2 = 2;
									if (C2Block == true) block = 3;
									C2CHP = C2CHP - ((E1A1dmg * (E1ATK / C2DEF) * multiplier1 / multiplier2) / block);
									if (E1A1effect == 2) C2POISON = 5;
									block = 1;
									multiplier1 = 1;
									multiplier2 = 1;
									if (E1class == 1) multiplier1 = 2;
									if (E1class == 2) multiplier2 = 2;
									if (C3Block == true) block = 3;
									C3CHP = C3CHP - ((E1A1dmg * (E1ATK / C3DEF) * multiplier1 / multiplier2) / block);
									if (E1A1effect == 2) C3POISON = 5;
									Cname = "everyone";
									Ename = E1name;
									Aname = E1A1name;
								}
							}
							if (EnemyAttackNum == 2) {
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E1A2target == 0) {
									if (E1class == 3) multiplier1 = 2;
									if (E1class == 1) multiplier2 = 2;
									if (C2Block == true) block = 3;
									C2CHP = C2CHP - ((E1A2dmg * (E1ATK / C2DEF) * multiplier1 / multiplier2) / block);
									if (E1A2effect == 2) C2POISON = 5;
									Cname = C2NAME;
									Ename = E1name;
									Aname = E1A2name;
								}
								if (E1A2target == 1) {
									if (E1class == 2) multiplier1 = 2;
									if (E1class == 3) multiplier2 = 2;
									if (C1Block == true) block = 3;
									C1CHP = C1CHP - ((E1A2dmg * (E1ATK / C1DEF) * multiplier1 / multiplier2) / block);
									if (E1A2effect == 2) C1POISON = 5;
									block = 1;
									multiplier1 = 1;
									multiplier2 = 1;
									if (E1class == 3) multiplier1 = 2;
									if (E1class == 1) multiplier2 = 2;
									if (C2Block == true) block = 3;
									C2CHP = C2CHP - ((E1A2dmg * (E1ATK / C2DEF) * multiplier1 / multiplier2) / block);
									if (E1A2effect == 2) C2POISON = 5;
									block = 1;
									multiplier1 = 1;
									multiplier2 = 1;
									if (E1class == 1) multiplier1 = 2;
									if (E1class == 2) multiplier2 = 2;
									if (C3Block == true) block = 3;
									C3CHP = C3CHP - ((E1A2dmg * (E1ATK / C3DEF) * multiplier1 / multiplier2) / block);
									if (E1A2effect == 2) C3POISON = 5;
									Cname = "everyone";
									Ename = E1name;
									Aname = E1A2name;
								}
							}
							if (EnemyAttackNum == 3) {
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E1class == 3) multiplier1 = 2;
								if (E1class == 1) multiplier2 = 2;
								if (C2Block == true) block = 3;
								C2CHP = C2CHP - ((E1A3dmg * (E1ATK / C2DEF) * multiplier1 / multiplier2) / block);
								if (E1A3effect == 1) {
									C2FROZEN = true;
								}
								if (E1A3effect == 2) C2POISON = 5;
								Cname = C2NAME;
								Ename = E1name;
								Aname = E1A3name;
							}
							if (EnemyAttackNum == 4) {
								E1CHP += E1MHP * 0.15;
								if (E1CHP > E1MHP) E1CHP = E1MHP;
								Cname = E1name;
								Ename = E1name;
								Aname = E1A4name;
							}
						}
						if (EnemyAttackTarget == 3) {
							if (EnemyAttackNum == 1) {
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E1A1target == 0) {
									if (E1class == 1) multiplier1 = 2;
									if (E1class == 2) multiplier2 = 2;
									if (C3Block == true) block = 3;
									C3CHP = C3CHP - ((E1A1dmg * (E1ATK / C3DEF) * multiplier1 / multiplier2) / block);
									if (E1A1effect == 2) C3POISON = 5;
									Cname = C3NAME;
									Ename = E1name;
									Aname = E1A1name;
								}
								if (E1A1target == 1) {
									if (E1class == 2) multiplier1 = 2;
									if (E1class == 3) multiplier2 = 2;
									if (C1Block == true) block = 3;
									C1CHP = C1CHP - ((E1A1dmg * (E1ATK / C1DEF) * multiplier1 / multiplier2) / block);
									if (E1A1effect == 2) C1POISON = 5;
									block = 1;
									multiplier1 = 1;
									multiplier2 = 1;
									if (E1class == 3) multiplier1 = 2;
									if (E1class == 1) multiplier2 = 2;
									if (C2Block == true) block = 3;
									C2CHP = C2CHP - ((E1A1dmg * (E1ATK / C2DEF) * multiplier1 / multiplier2) / block);
									if (E1A1effect == 2) C2POISON = 5;
									block = 1;
									multiplier1 = 1;
									multiplier2 = 1;
									if (E1class == 1) multiplier1 = 2;
									if (E1class == 2) multiplier2 = 2;
									if (C3Block == true) block = 3;
									C3CHP = C3CHP - ((E1A1dmg * (E1ATK / C3DEF) * multiplier1 / multiplier2) / block);
									if (E1A1effect == 2) C3POISON = 5;
									Cname = "everyone";
									Ename = E1name;
									Aname = E1A1name;
								}
							}
							if (EnemyAttackNum == 2) {
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E1A2target == 0) {
									if (E1class == 1) multiplier1 = 2;
									if (E1class == 2) multiplier2 = 2;
									if (C3Block == true) block = 3;
									C3CHP = C3CHP - ((E1A2dmg * (E1ATK / C3DEF) * multiplier1 / multiplier2) / block);
									if (E1A2effect == 2) C3POISON = 5;
									Cname = C3NAME;
									Ename = E1name;
									Aname = E1A2name;
								}
								if (E1A2target == 1) {
									if (E1class == 2) multiplier1 = 2;
									if (E1class == 3) multiplier2 = 2;
									if (C1Block == true) block = 3;
									C1CHP = C1CHP - ((E1A2dmg * (E1ATK / C1DEF) * multiplier1 / multiplier2) / block);
									if (E1A2effect == 2) C1POISON = 5;
									block = 1;
									multiplier1 = 1;
									multiplier2 = 1;
									if (E1class == 3) multiplier1 = 2;
									if (E1class == 1) multiplier2 = 2;
									if (C2Block == true) block = 3;
									C2CHP = C2CHP - ((E1A2dmg * (E1ATK / C2DEF) * multiplier1 / multiplier2) / block);
									if (E1A2effect == 2) C2POISON = 5;
									block = 1;
									multiplier1 = 1;
									multiplier2 = 1;
									if (E1class == 1) multiplier1 = 2;
									if (E1class == 2) multiplier2 = 2;
									if (C3Block == true) block = 3;
									C3CHP = C3CHP - ((E1A2dmg * (E1ATK / C3DEF) * multiplier1 / multiplier2) / block);
									if (E1A2effect == 2) C3POISON = 5;
									Cname = "everyone";
									Ename = E1name;
									Aname = E1A2name;
								}
							}
							if (EnemyAttackNum == 3) {
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E1class == 1) multiplier1 = 2;
								if (E1class == 2) multiplier2 = 2;
								if (C3Block == true) block = 3;
								C3CHP = C3CHP - ((E1A3dmg * (E1ATK / C3DEF) * multiplier1 / multiplier2) / block);
								if (E1A3effect == 1) {
									C3FROZEN = true;
								}
								if (E1A3effect == 2) C3POISON = 5;
								Cname = C3NAME;
								Ename = E1name;
								Aname = E1A3name;
							}
							if (EnemyAttackNum == 4) {
								E1CHP += E1MHP * 0.15;
								if (E1CHP > E1MHP) E1CHP = E1MHP;
								Cname = E1name;
								Ename = E1name;
								Aname = E1A4name;
							}
						}
					}
				}
				EnemyAdone = true;
			}
			app->render->DrawText(10 * app->ScalingMultiplier, 100 * app->ScalingMultiplier, YF, Ename, 16);
			app->render->DrawText(10 * app->ScalingMultiplier, 110 * app->ScalingMultiplier, WF, "used", 16);
			app->render->DrawText(10 * app->ScalingMultiplier, 120 * app->ScalingMultiplier, YF, Aname, 16);
			app->render->DrawText(10 * app->ScalingMultiplier, 130 * app->ScalingMultiplier, WF, "against", 16);
			app->render->DrawText(10 * app->ScalingMultiplier, 140 * app->ScalingMultiplier, YF, Cname, 16);
			if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
				FinishTurn();
			}
			if (app->input->controllers.A != 0)
			{
				FinishTurn(); 
			}
			
		}

		if (Turn[0] >= 4) option = COMBATMENU::NONE;

		//Combat end
		if (E1dead == true && E2dead == true && E3dead == true) {
			option = COMBATMENU::WIN;
		}
		if (C1dead == true && C2dead == true && C3dead == true) {
			option = COMBATMENU::LOSE;
		}
	}
	return true;
}

// Called each loop iteration
bool Combat::PostUpdate()
{
	bool ret = true;

	if (option == COMBATMENU::WIN) {
		app->render->DrawText(10 * app->ScalingMultiplier, 100 * app->ScalingMultiplier, YF, "You won!", 16);
		app->render->DrawText(10 * app->ScalingMultiplier, 110 * app->ScalingMultiplier, YF, "and got:", 16);
		sprintf_s(Aux, "%d", EXPwon);
		app->render->DrawText(10 * app->ScalingMultiplier, 120 * app->ScalingMultiplier, YF, Aux, 16);
		app->render->DrawText(10 * app->ScalingMultiplier + 24, 120 * app->ScalingMultiplier, YF, "exp", 16);
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
			ExitCombat();
		}
		if (app->input->controllers.A != 0 && !A_pressed)
		{
			ExitCombat();
			A_pressed = true;
		}
		else if (app->input->controllers.A == 0)
		{
			A_pressed = false;
		}
	}
	if (option == COMBATMENU::LOSE) {
		app->render->DrawText(10 * app->ScalingMultiplier, 100 * app->ScalingMultiplier, YF, "You lost...", 16);

		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
			EXPwon = 0;
			SaveInstance = 3;
			app->scene->player->tpHouse = true;
			ExitCombat();
		}
		if (app->input->controllers.A != 0 && !A_pressed)
		{
			EXPwon = 0;
			SaveInstance = 3;
			app->scene->player->tpHouse = true;
			ExitCombat();
			A_pressed = true;
		}
		else if (app->input->controllers.A == 0)
		{
			A_pressed = false;
		}
	
	}

	return ret;
}

void Combat::StartCombat()  
{
	//Disable map print
	SaveInstance = app->Instance;
	app->Instance = -1;
	app->scene->player->active = false;
	InCombat = true;
	TeamTurn = 1;
	LoadLaurea(app->scene->player->laurea);
	LoadLapis(app->scene->player->lapis);
	if (Preset == 1) {
		LoadEnemy(app->entityManager->slimeFrog);
		E2dead = true;
		E3dead = true;
	}
	if (Preset == 2) {
		LoadEnemy(app->entityManager->waterlilyfish);
		E2dead = true;
		E3dead = true;
	}
	if (Preset == 3) {
		LoadEnemy(app->entityManager->slimeFrog);
		LoadEnemy(app->entityManager->slimeFrog);
		E3dead = true;
	}
	if (Preset == 4) {
		LoadEnemy(app->entityManager->waterlilyfish);
		LoadEnemy(app->entityManager->waterlilyfish);
		E3dead = true;
	}
	if (Preset == 5) {
		LoadEnemy(app->entityManager->slimeFrog);
		LoadEnemy(app->entityManager->waterlilyfish);
		E3dead = true;
	}
	if (Preset == 6) {
		LoadEnemy(app->entityManager->slimeFrog);
		LoadEnemy(app->entityManager->slimeFrog);
		LoadEnemy(app->entityManager->slimeFrog);
	}
	if (Preset == 7) {
		LoadEnemy(app->entityManager->waterlilyfish);
		LoadEnemy(app->entityManager->waterlilyfish);
		LoadEnemy(app->entityManager->waterlilyfish);
	}
	if (Preset == 8) {
		LoadEnemy(app->entityManager->slimeFrog);
		LoadEnemy(app->entityManager->slimeFrog);
		LoadEnemy(app->entityManager->waterlilyfish);
	}
	if (Preset == 9) {
		LoadEnemy(app->entityManager->waterlilyfish);
		LoadEnemy(app->entityManager->waterlilyfish);
		LoadEnemy(app->entityManager->slimeFrog);
	}
	if (Preset == 10) {
		LoadEnemy(app->entityManager->naiadongoddess);
		LoadEnemy(app->entityManager->waterlilyfish);
		LoadEnemy(app->entityManager->slimeFrog);
	}
	if (Preset == 11) {
		LoadEnemy(app->entityManager->poisonSlimeFrog);
		E2dead = true;
		E3dead = true;
	}
	if (Preset == 12) {
		LoadEnemy(app->entityManager->waterlilyfish);
		E2dead = true;
		E3dead = true;
	}
	if (Preset == 13) {
		LoadEnemy(app->entityManager->poisonSlimeFrog);
		LoadEnemy(app->entityManager->poisonSlimeFrog);
		E3dead = true;
	}
	if (Preset == 14) {
		LoadEnemy(app->entityManager->waterlilyfish);
		LoadEnemy(app->entityManager->infernalSlimeFrog);
		E3dead = true;
	}
	if (Preset == 15) {
		LoadEnemy(app->entityManager->poisonSlimeFrog);
		LoadEnemy(app->entityManager->waterlilyfish);
		E3dead = true;
	}
	if (Preset == 16) {
		LoadEnemy(app->entityManager->poisonSlimeFrog);
		LoadEnemy(app->entityManager->slimeFrog);
		LoadEnemy(app->entityManager->infernalSlimeFrog);
	}
	if (Preset == 17) {
		LoadEnemy(app->entityManager->waterlilyfish);
		LoadEnemy(app->entityManager->waterlilyfish);
		LoadEnemy(app->entityManager->infernalSlimeFrog);
	}
	if (Preset == 18) {
		LoadEnemy(app->entityManager->slimeFrog);
		LoadEnemy(app->entityManager->infernalSlimeFrog);
		LoadEnemy(app->entityManager->waterlilyfish);
	}
	if (Preset == 19) {
		LoadEnemy(app->entityManager->waterlilyfish);
		LoadEnemy(app->entityManager->waterlilyfish);
		LoadEnemy(app->entityManager->slimeFrog);
	}
	if (Preset == 20) {
		LoadEnemy(app->entityManager->naiadongoddess);
		LoadEnemy(app->entityManager->waterlilyfish);
		LoadEnemy(app->entityManager->poisonSlimeFrog);
	}
	TurnOrder();
	EXPwon = E1EXP + E2EXP + E3EXP;
	limitCount1 = LIMIT1;
	limitCount2 = LIMIT2;
	limitCount3 = LIMIT3;
}

void Combat::ExitCombat()
{
	//Enable map print
	app->Instance = SaveInstance;
	app->scene->player->active = true;
	InCombat = false;
	AttackMenu = false;
	EnemySelect = false;
	EnemyAdone = false;
	option = COMBATMENU::ATTACK;
	CurrentCharacters = 0;
	CurrentEnemies = 0;
	TeamTurn = 0;
	offset = 0;
	M1 = 0;
	M2 = 0;
	M3 = 0;

	C1dead = false;
	C2dead = false;
	C3dead = false;
	E1dead = false;
	E2dead = false;
	E3dead = false;
	C1Check = false;
	C2Check = false;
	C3Check = false;
	E1Check = false;
	E2Check = false;
	E3Check = false;
	
	//Save stats to characters
	app->scene->player->laurea.chp = C1CHP;
	app->scene->player->laurea.cmp = C1CMP;
	app->scene->player->lapis.chp = C2CHP;
	app->scene->player->lapis.cmp = C2CMP;
	app->scene->player->lucca.chp = C3CHP;
	app->scene->player->lucca.cmp = C3CMP;

	//Add exp to characters
	if (charactersLoaded == 1) {
		app->scene->player->laurea.exp += EXPwon;
	}
	if (charactersLoaded == 2) {
		app->scene->player->laurea.exp += EXPwon;
		app->scene->player->lapis.exp += EXPwon;
	}
	if (charactersLoaded == 3) {
		app->scene->player->laurea.exp += EXPwon;
		app->scene->player->lapis.exp += EXPwon;
		app->scene->player->lucca.exp += EXPwon;
	}
	charactersLoaded = 0;
	EXPwon = 0;

	C1FROZEN = false;
	C2FROZEN = false;
	C3FROZEN = false;
	C1POISON = 0;
	C2POISON = 0;
	C3POISON = 0;
	C1BURN = 0;
	C2BURN = 0;
	C3BURN = 0;

	Turn[0] = 0;
	Turn[1] = 0;
	Turn[2] = 0;
	Turn[3] = 0;
	Turn[4] = 0;
	Turn[5] = 0;

	CleanUp();
}

void Combat::FinishTurn()
{
	if (Turn[0] == 1) {
		if (limitCount1 != LIMIT1) {
			limitCount1++;
		}
	}
	if (Turn[0] == 2) {
		if (limitCount2 != LIMIT2) {
			limitCount2++;
		}
	}
	if (Turn[0] == 3) {
		if (limitCount3 != LIMIT3) {
			limitCount3++;
		}
	}
	int Aux = Turn[0];
	for (int i = 0; i < 6; i++) {
		if (i <= 4) {
			Turn[i] = Turn[i + 1];
		}
		else {
			Turn[i] = Aux;
		}
	};
	if (Turn[0] == 1) {
		C1Block = false;
	}
	if (Turn[0] == 2) {
		C2Block = false;
	}
	if (Turn[0] == 3) {
		C3Block = false;
	}
	AttackMenu = false;
	EnemySelect = false;
	option = COMBATMENU::ATTACK;
	EnemyAdone = false;

	if (Turn[0] >= 1 && Turn[0] <= 3) {
		TeamTurn = 1;
	}
	FinishedTurn1 = false;
	FinishedTurn2 = false;
	FinishedTurn3 = false;
	FinishedTurnB1 = false;
	FinishedTurnB2 = false;
	FinishedTurnB3 = false;
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
	
	// Ordering by speed
	while (ordered == false) {
		int a = 0;
		for (int i = 0; i < 5; i++) {
			//LOG("%d %d %d %d %d %d", Turn[0], Turn[1], Turn[2], Turn[3], Turn[4], Turn[5]);
			if (Turn[i] < Turn[i + 1]) {
				a++;
			}

			if (a == 0) ordered = true;
		}
		for (int i = 0; i < 5; i++) {
			int b;
			if (Turn[i] <= Turn[i + 1]) {
				b = Turn[i];
				Turn[i] = Turn[i + 1];
				Turn[i + 1] = b;
			}
		}
	}

	//Assigning each entity their position
	bool C1 = false, C2 = false, C3 = false, E1 = false, E2 = false, E3 = false;
	for (int i = 0; i < 6; i++) {
		if (Turn[i] == C1speed && C1 == false) {
			Turn[i] = 1;
			C1 = true;
			if (C1speed == 0) {
				Turn[i] = 0;
				C1dead = true;
			}
		}
		else if (Turn[i] == C2speed && C2 == false) {
			Turn[i] = 2;
			C2 = true;
			if (C2speed == 0) {
				Turn[i] = 0;
				C2dead = true;
			}
		}
		else if (Turn[i] == C3speed && C3 == false) {
			Turn[i] = 3;
			C3 = true;
			if (C3speed == 0) {
				Turn[i] = 0;
				C3dead = true;
			}
		}
		else if (Turn[i] == E1speed && E1 == false) {
			Turn[i] = 4;
			E1 = true;
			if (E1speed == 0) {
				Turn[i] = 0;
				E1dead = true;
			}
		}
		else if (Turn[i] == E2speed && E2 == false) {
			Turn[i] = 5;
			E2 = true;
			if (E2speed == 0) {
				Turn[i] = 0;
				E2dead = true;
			}
		}
		else if (Turn[i] == E3speed && E3 == false) {
			Turn[i] = 6;
			E3 = true;
			if (E3speed == 0) {
				Turn[i] = 0;
				E3dead = true;
			}
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
	C1lvl = laurea.lvl;
	charactersLoaded++;
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
	C2lvl = lapis.lvl;
	charactersLoaded++;
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
	C3lvl = lucca.lvl;
	charactersLoaded++;
}

void Combat::LoadEnemy(EntityManager::CombatEnemy enemy)
{
	//Loading speed
	if (CurrentEnemies == 2) {
		E3speed = enemy.spe;
		CurrentEnemies++;

		E3EXP = enemy.exp;
		E3MHP = enemy.hp;
		E3CHP = enemy.chp;
		E3MES = enemy.es;
		E3CES = enemy.ces;
		E3ATK = enemy.atk;
		E3DEF = enemy.def;
		E3Weak = enemy.weakness;
		E3Res = enemy.resistance;
		E3name = enemy.name;
		E3A1dmg = enemy.A1dmg;
		E3A1name = enemy.A1name;
		E3A1target = enemy.A1target;
		E3A1effect = enemy.A1effect;
		E3A2dmg = enemy.A2dmg;
		E3A2name = enemy.A2name;
		E3A2target = enemy.A2target;
		E3A2effect = enemy.A2effect;
		E3A3dmg = enemy.A3dmg;
		E3A3name = enemy.A3name;
		E3A3target = enemy.A3target;
		E3A3effect = enemy.A3effect;
		E3A4dmg = enemy.A4dmg;
		E3A4name = enemy.A4name;
		E3A4target = enemy.A4target;
		E3A4effect = enemy.A4effect;
		E3class = enemy.Eclass;
		if (enemy.boss == 2) {
			E3BOSS = 64;
		}
		if (enemy.boss == 1) {
			E3BOSS = 0;
		}
		//Selecting enemy asset
		if (enemy.asset == 1) {
			E3asset = Enemy1;
		}
		if (enemy.asset == 2) {
			E3asset = Enemy2;
		}
		if (enemy.asset == 3) {
			E3asset = Enemy3;
		}
		if (enemy.asset == 4) {
			E3asset = Enemy4;
		}
		if (enemy.asset == 5) {
			E3asset = Enemy5;
		}
	}
	if (CurrentEnemies == 1) {
		E2speed = enemy.spe;
		CurrentEnemies++;

		E2EXP = enemy.exp;
		E2MHP = enemy.hp;
		E2CHP = enemy.chp;
		E2MES = enemy.es;
		E2CES = enemy.ces;
		E2ATK = enemy.atk;
		E2DEF = enemy.def;
		E2Weak = enemy.weakness;
		E2Res = enemy.resistance;
		E2name = enemy.name;
		E2A1dmg = enemy.A1dmg;
		E2A1name = enemy.A1name;
		E2A1target = enemy.A1target;
		E2A1effect = enemy.A1effect;
		E2A2dmg = enemy.A2dmg;
		E2A2name = enemy.A2name;
		E2A2target = enemy.A2target;
		E2A2effect = enemy.A2effect;
		E2A3dmg = enemy.A3dmg;
		E2A3name = enemy.A3name;
		E2A3target = enemy.A3target;
		E2A3effect = enemy.A3effect;
		E2A4dmg = enemy.A4dmg;
		E2A4name = enemy.A4name;
		E2A4target = enemy.A4target;
		E2A4effect = enemy.A4effect;
		E2class = enemy.Eclass;
		if (enemy.boss == 2) {
			E2BOSS = 64;
		}
		if (enemy.boss == 1) {
			E2BOSS = 0;
		}
		//Selecting enemy asset
		if (enemy.asset == 1) {
			E2asset = Enemy1;
		}
		if (enemy.asset == 2) {
			E2asset = Enemy2;
		}
		if (enemy.asset == 3) {
			E2asset = Enemy3;
		}
		if (enemy.asset == 4) {
			E2asset = Enemy4;
		}
		if (enemy.asset == 5) {
			E2asset = Enemy5;
		}
	}
	if (CurrentEnemies == 0) {
		E1speed = enemy.spe;
		CurrentEnemies++;

		E1EXP = enemy.exp;
		E1MHP = enemy.hp;
		E1CHP = enemy.chp;
		E1MES = enemy.es;
		E1CES = enemy.ces;
		E1ATK = enemy.atk;
		E1DEF = enemy.def;
		E1Weak = enemy.weakness;
		E1Res = enemy.resistance;
		E1name = enemy.name;
		E1A1dmg = enemy.A1dmg;
		E1A1name = enemy.A1name;
		E1A1target = enemy.A1target;
		E1A1effect = enemy.A1effect;
		E1A2dmg = enemy.A2dmg;
		E1A2name = enemy.A2name;
		E1A2target = enemy.A2target;
		E1A2effect = enemy.A2effect;
		E1A3dmg = enemy.A3dmg;
		E1A3name = enemy.A3name;
		E1A3target = enemy.A3target;
		E1A3effect = enemy.A3effect;
		E1A4dmg = enemy.A4dmg;
		E1A4name = enemy.A4name;
		E1A4target = enemy.A4target;
		E1A4effect = enemy.A4effect;
		E1class = enemy.Eclass;
		if (enemy.boss == 2) {
			E1BOSS = 64;
		}
		if (enemy.boss == 1) {
			E1BOSS = 0;
		}
		//Selecting enemy asset
		if (enemy.asset == 1) {
			E1asset = Enemy1;
		}
		if (enemy.asset == 2) {
			E1asset = Enemy2;
		}
		if (enemy.asset == 3) {
			E1asset = Enemy3;
		}
		if (enemy.asset == 4) {
			E1asset = Enemy4;
		}
		if (enemy.asset == 5) {
			E1asset = Enemy5;
		}
	}
}

void Combat::RemoveEntityFromList(int id) {
	for (int i = 0; i < 6; i++) {
		if (Turn[i] == id) {
			Turn[i] = 0;
		}
	}
}

// Called before quitting
bool Combat::CleanUp()
{
	LOG("Freeing combat");

	C1speed = NULL;
	CurrentCharacters = NULL;
	C1MHP = NULL;
	C1CHP = NULL;
	C1MMP = NULL;
	C1CMP = NULL;
	C1ATK = NULL;
	C1DEF = NULL;
	LIMIT1 = NULL;
	C1NAME = NULL;
	C1lvl = NULL;
	charactersLoaded = NULL;
	C2speed = NULL;
	C2MHP = NULL;
	C2CHP = NULL;
	C2MMP = NULL;
	C2CMP = NULL;
	C2ATK = NULL;
	C2DEF = NULL;
	LIMIT2 = NULL;
	C2NAME = NULL;
	C2lvl = NULL;
	E3speed = NULL;
	CurrentEnemies = NULL;

	E3EXP = NULL;
	E3MHP = NULL;
	E3CHP = NULL;
	E3MES = NULL;
	E3CES = NULL;
	E3ATK = NULL;
	E3DEF = NULL;
	E3Weak = NULL;
	E3Res = NULL;
	E3name = NULL;
	E3A1dmg = NULL;
	E3A1name = NULL;
	E3A1target = NULL;
	E3A1effect = NULL;
	E3A2dmg = NULL;
	E3A2name = NULL;
	E3A2target = NULL;
	E3A2effect = NULL;
	E3A3dmg = NULL;
	E3A3name = NULL;
	E3A3target = NULL;
	E3A3effect = NULL;
	E3A4dmg = NULL;
	E3A4name = NULL;
	E3A4target = NULL;
	E3A4effect = NULL;
	E3class = NULL;
	E2speed = NULL;

	E2EXP = NULL;
	E2MHP = NULL;
	E2CHP = NULL;
	E2MES = NULL;
	E2CES = NULL;
	E2ATK = NULL;
	E2DEF = NULL;
	E2Weak = NULL;
	E2Res = NULL;
	E2name = NULL;
	E2A1dmg = NULL;
	E2A1name = NULL;
	E2A1target = NULL;
	E2A1effect = NULL;
	E2A2dmg = NULL;
	E2A2name = NULL;
	E2A2target = NULL;
	E2A2effect = NULL;
	E2A3dmg = NULL;
	E2A3name = NULL;
	E2A3target = NULL;
	E2A3effect = NULL;
	E2A4dmg = NULL;
	E2A4name = NULL;
	E2A4target = NULL;
	E2A4effect = NULL;
	E2class = NULL;


	return true;
}
