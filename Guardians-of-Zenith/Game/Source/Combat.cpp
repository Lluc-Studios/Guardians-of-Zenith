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
	WF = app->font->Load("Assets/Fonts/FontWhiteDef.png", lookupTable, 1);
	GF = app->font->Load("Assets/Fonts/FontGreyDef.png", lookupTable, 1);
	YF = app->font->Load("Assets/Fonts/FontYellowDef.png", lookupTable, 1);
	BG = app->tex->Load("Assets/Textures/Temporary.png");
	Character1 = app->tex->Load("Assets/Entities/Characters/Character1.png");
	Character2 = app->tex->Load("Assets/Entities/Characters/Character2.png");
	Character3 = app->tex->Load("Assets/Entities/Characters/Character1.png");
	Enemy1 = app->tex->Load("Assets/Entities/Enemies/Frog.png");
	Enemy2 = app->tex->Load("Assets/Entities/Enemies/LilyFish.png");
	Enemy3 = app->tex->Load("Assets/Entities/Enemies/NaiadonGoddess.png");
	E1asset = app->tex->Load("Assets/Entities/Enemies/Frog.png");
	E2asset = app->tex->Load("Assets/Entities/Enemies/Frog.png");
	E3asset = app->tex->Load("Assets/Entities/Enemies/Frog.png");
	ClassTank = app->tex->Load("Assets/Textures/YELLOW.png");
	ClassMage = app->tex->Load("Assets/Textures/BLUE.png");
	ClassArcher = app->tex->Load("Assets/Textures/GREEN.png");
	ClassChart = app->tex->Load("Assets/Textures/ClassChart.png");

	//Prevent memory leaks
	E1 = app->tex->Load("Assets/Textures/YELLOW.png");
	E2 = app->tex->Load("Assets/Textures/YELLOW.png");
	E3 = app->tex->Load("Assets/Textures/YELLOW.png");


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
		//Render text
		app->render->DrawTexture(BG, app->scene->player->position.x - 290, app->scene->player->position.y - 250);
		app->render->DrawTexture(ClassChart, app->scene->player->position.x-280, app->scene->player->position.y -170);
		app->render->DrawRectangle({ app->scene->player->position.x - 280,app->scene->player->position.y,100,160 }, 0, 0, 255, 150);
		app->font->BlitText(160 * app->ScalingMultiplier, 20 * app->ScalingMultiplier, WF, "turn");
		if (option != COMBATMENU::NONE) {
			if (option == COMBATMENU::ATTACK && AttackMenu == false) {
				app->font->BlitText(10 * app->ScalingMultiplier, 100 * app->ScalingMultiplier, WF, "attack");
				app->font->BlitText(10 * app->ScalingMultiplier, 120 * app->ScalingMultiplier, GF, "defend");
				app->font->BlitText(10 * app->ScalingMultiplier, 140 * app->ScalingMultiplier, GF, "inventory");
				app->font->BlitText(10 * app->ScalingMultiplier, 160 * app->ScalingMultiplier, GF, "escape");
				app->render->DrawRectangle({ app->scene->player->position.x - 280,app->scene->player->position.y + 15,100,20 }, 255, 255, 255, WhiteFading);
			}
			if (option == COMBATMENU::DEFEND && AttackMenu == false) {
				app->font->BlitText(10 * app->ScalingMultiplier, 100 * app->ScalingMultiplier, GF, "attack");
				app->font->BlitText(10 * app->ScalingMultiplier, 120 * app->ScalingMultiplier, WF, "defend");
				app->font->BlitText(10 * app->ScalingMultiplier, 140 * app->ScalingMultiplier, GF, "inventory");
				app->font->BlitText(10 * app->ScalingMultiplier, 160 * app->ScalingMultiplier, GF, "escape");
				app->render->DrawRectangle({ app->scene->player->position.x - 280,app->scene->player->position.y + 55,100,20 }, 255, 255, 255, WhiteFading);
			}
			if (option == COMBATMENU::INVENTORY && AttackMenu == false) {
				app->font->BlitText(10 * app->ScalingMultiplier, 100 * app->ScalingMultiplier, GF, "attack");
				app->font->BlitText(10 * app->ScalingMultiplier, 120 * app->ScalingMultiplier, GF, "defend");
				app->font->BlitText(10 * app->ScalingMultiplier, 140 * app->ScalingMultiplier, WF, "inventory");
				app->font->BlitText(10 * app->ScalingMultiplier, 160 * app->ScalingMultiplier, GF, "escape");
				app->render->DrawRectangle({ app->scene->player->position.x - 280,app->scene->player->position.y + 95,100,20 }, 255, 255, 255, WhiteFading);
			}
			if (option == COMBATMENU::ESCAPE && AttackMenu == false) {
				app->font->BlitText(10 * app->ScalingMultiplier, 100 * app->ScalingMultiplier, GF, "attack");
				app->font->BlitText(10 * app->ScalingMultiplier, 120 * app->ScalingMultiplier, GF, "defend");
				app->font->BlitText(10 * app->ScalingMultiplier, 140 * app->ScalingMultiplier, GF, "inventory");
				app->font->BlitText(10 * app->ScalingMultiplier, 160 * app->ScalingMultiplier, WF, "escape");
				app->render->DrawRectangle({ app->scene->player->position.x - 280,app->scene->player->position.y + 135,100,20 }, 255, 255, 255, WhiteFading);
			}
			if (option == COMBATMENU::ATTACK1 && AttackMenu == true && EnemySelect == false) {
				app->font->BlitText(10 * app->ScalingMultiplier, 100 * app->ScalingMultiplier, WF, Attack1);
				app->font->BlitText(10 * app->ScalingMultiplier, 120 * app->ScalingMultiplier, GF, Attack2);
				app->font->BlitText(10 * app->ScalingMultiplier+78, 100 * app->ScalingMultiplier, WF, M1);
				app->font->BlitText(10 * app->ScalingMultiplier+78, 120 * app->ScalingMultiplier, GF, M2);
				app->render->DrawRectangle({ app->scene->player->position.x - 280,app->scene->player->position.y + 15,100,20 }, 255, 255, 255, WhiteFading);
			}
			if (option == COMBATMENU::ATTACK2 && AttackMenu == true && EnemySelect == false) {
				app->font->BlitText(10 * app->ScalingMultiplier, 100 * app->ScalingMultiplier, GF, Attack1);
				app->font->BlitText(10 * app->ScalingMultiplier, 120 * app->ScalingMultiplier, WF, Attack2);
				app->font->BlitText(10 * app->ScalingMultiplier+78, 100 * app->ScalingMultiplier, GF, M1);
				app->font->BlitText(10 * app->ScalingMultiplier+78, 120 * app->ScalingMultiplier, WF, M2);
				app->render->DrawRectangle({ app->scene->player->position.x - 280,app->scene->player->position.y + 55,100,20 }, 255, 255, 255, WhiteFading);
			}
			if (option == COMBATMENU::ENEMY1 && EnemySelect == true) {
				if (E1dead == true) option = COMBATMENU::ENEMY2;
				app->font->BlitText(10 * app->ScalingMultiplier, 100 * app->ScalingMultiplier, WF, E1name);
				app->font->BlitText(10 * app->ScalingMultiplier, 120 * app->ScalingMultiplier, GF, E2name);
				app->font->BlitText(10 * app->ScalingMultiplier, 140 * app->ScalingMultiplier, GF, E3name);
				app->render->DrawRectangle({ app->scene->player->position.x - 280,app->scene->player->position.y + 15,100,20 }, 255, 255, 255, WhiteFading);
				if (E1dead == false) {
					app->render->DrawRectangle({ app->scene->player->position.x + 114 , app->scene->player->position.y - 71,66,66 }, 255, 255, 255, 120);
				}
			}
			if (option == COMBATMENU::ENEMY2 && EnemySelect == true) {
				if (E2dead == true) option = COMBATMENU::ENEMY3;
				app->font->BlitText(10 * app->ScalingMultiplier, 100 * app->ScalingMultiplier, GF, E1name);
				app->font->BlitText(10 * app->ScalingMultiplier, 120 * app->ScalingMultiplier, WF, E2name);
				app->font->BlitText(10 * app->ScalingMultiplier, 140 * app->ScalingMultiplier, GF, E3name);
				app->render->DrawRectangle({ app->scene->player->position.x - 280,app->scene->player->position.y + 55,100,20 }, 255, 255, 255, WhiteFading);
				if (E2dead == false) {
					app->render->DrawRectangle({ app->scene->player->position.x + 199 , app->scene->player->position.y - 131,66,66 }, 255, 255, 255, 120);
				}
			}
			if (option == COMBATMENU::ENEMY3 && EnemySelect == true) {
				if (E3dead == true) option = COMBATMENU::ENEMY1;
				app->font->BlitText(10 * app->ScalingMultiplier, 100 * app->ScalingMultiplier, GF, E1name);
				app->font->BlitText(10 * app->ScalingMultiplier, 120 * app->ScalingMultiplier, GF, E2name);
				app->font->BlitText(10 * app->ScalingMultiplier, 140 * app->ScalingMultiplier, WF, E3name);
				app->render->DrawRectangle({ app->scene->player->position.x - 280,app->scene->player->position.y + 95,100,20 }, 255, 255, 255, WhiteFading);
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
			}
			app->font->BlitText(100 * app->ScalingMultiplier, 130 * app->ScalingMultiplier, WF, C1NAME);
			app->render->DrawTexture(ClassTank,app->scene->player->position.x-110, app->scene->player->position.y+77);
			app->font->BlitText(100 * app->ScalingMultiplier, 140 * app->ScalingMultiplier, WF, "hp");
			//Int to string convert
			sprintf_s(Aux, "%.0f", C1MHP);
			app->font->BlitText(135 * app->ScalingMultiplier, 140 * app->ScalingMultiplier, WF, Aux);
			sprintf_s(Aux, "%.0f", C1CHP);
			app->font->BlitText(120 * app->ScalingMultiplier, 140 * app->ScalingMultiplier, WF, Aux);
			app->render->DrawRectangle({ app->scene->player->position.x - 90,app->scene->player->position.y + 115,100,10 }, 0, 0, 0);
			//Calculate hp bar length
			HpBarLengthC1 = (C1CHP * 98) / C1MHP;
			app->render->DrawRectangle({ app->scene->player->position.x - 89,app->scene->player->position.y + 116,HpBarLengthC1,8 }, 0, 200, 0);
			app->font->BlitText(100 * app->ScalingMultiplier, 160 * app->ScalingMultiplier, WF, "mp");
			sprintf_s(Aux, "%.0f", C1MMP);
			app->font->BlitText(135 * app->ScalingMultiplier, 160 * app->ScalingMultiplier, WF, Aux);
			sprintf_s(Aux, "%.0f", C1CMP);
			app->font->BlitText(120 * app->ScalingMultiplier, 160 * app->ScalingMultiplier, WF, Aux);
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
				}
				app->font->BlitText(180 * app->ScalingMultiplier, 130 * app->ScalingMultiplier, WF, C2NAME);
				app->render->DrawTexture(ClassMage, app->scene->player->position.x + 50, app->scene->player->position.y + 77);
				app->font->BlitText(180 * app->ScalingMultiplier, 140 * app->ScalingMultiplier, WF, "hp");
				//Int to string convert
				sprintf_s(Aux, "%.0f", C2MHP);
				app->font->BlitText(215 * app->ScalingMultiplier, 140 * app->ScalingMultiplier, WF, Aux);
				sprintf_s(Aux, "%.0f", C2CHP);
				app->font->BlitText(200 * app->ScalingMultiplier, 140 * app->ScalingMultiplier, WF, Aux);
				app->render->DrawRectangle({ app->scene->player->position.x + 70,app->scene->player->position.y + 115,100,10 }, 0, 0, 0);
				//Calculate hp bar length 
				HpBarLengthC2 = (C2CHP * 98) / C2MHP;
				app->render->DrawRectangle({ app->scene->player->position.x + 71,app->scene->player->position.y + 116,HpBarLengthC2,8 }, 0, 200, 0);
				app->font->BlitText(180 * app->ScalingMultiplier, 160 * app->ScalingMultiplier, WF, "mp");
				sprintf_s(Aux, "%.0f", C2MMP);
				app->font->BlitText(215 * app->ScalingMultiplier, 160 * app->ScalingMultiplier, WF, Aux);
				sprintf_s(Aux, "%.0f", C2CMP);
				app->font->BlitText(200 * app->ScalingMultiplier, 160 * app->ScalingMultiplier, WF, Aux);
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
					}
					app->font->BlitText(260 * app->ScalingMultiplier, 130 * app->ScalingMultiplier, WF, C3NAME);
					app->render->DrawTexture(ClassArcher, app->scene->player->position.x + 210, app->scene->player->position.y + 77);
					app->font->BlitText(260 * app->ScalingMultiplier, 140 * app->ScalingMultiplier, WF, "hp");
					//Int to string convert
					sprintf_s(Aux, "%.0f", C3MHP);
					app->font->BlitText(295 * app->ScalingMultiplier, 140 * app->ScalingMultiplier, WF, Aux);
					sprintf_s(Aux, "%.0f", C3CHP);
					app->font->BlitText(280 * app->ScalingMultiplier, 140 * app->ScalingMultiplier, WF, Aux);
					app->render->DrawRectangle({ app->scene->player->position.x + 230,app->scene->player->position.y + 115,100,10 }, 0, 0, 0);
					//Calculate hp bar length 
					HpBarLengthC3 = (C3CHP * 98) / C3MHP;
					app->render->DrawRectangle({ app->scene->player->position.x + 231,app->scene->player->position.y + 116,HpBarLengthC3,8 }, 0, 200, 0);
					app->font->BlitText(260 * app->ScalingMultiplier, 160 * app->ScalingMultiplier, WF, "mp");
					sprintf_s(Aux, "%.0f", C3MMP);
					app->font->BlitText(295 * app->ScalingMultiplier, 160 * app->ScalingMultiplier, WF, Aux);
					sprintf_s(Aux, "%.0f", C3CMP);
					app->font->BlitText(280 * app->ScalingMultiplier, 160 * app->ScalingMultiplier, WF, Aux);
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
					app->render->DrawRectangle({ app->scene->player->position.x + 114 , app->scene->player->position.y - 71,66,66 }, 255, 255, 255, 120);
				}
				app->render->DrawTexture(E1asset, app->scene->player->position.x + 115-E1BOSS, app->scene->player->position.y - 70-E1BOSS);
				//Int to string convert
				sprintf_s(Aux, "%.0f", E1MHP);
				app->render->DrawRectangle({ app->scene->player->position.x + 115,app->scene->player->position.y,64,10 }, 0, 0, 0);
				HpBarLengthE1 = (E1CHP * 62) / E1MHP;
				app->render->DrawRectangle({ app->scene->player->position.x + 116,app->scene->player->position.y + 1,HpBarLengthE1,8 }, 0, 200, 0);
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



		//Load the characters attacks
		if (AttackMenu == true && TeamTurn == 1) {
			if (Turn[0] == 1) {
				Attack1 = C1A1name;
				Attack2 = C1A2name;
				sprintf_s(Aux1, "%.0f", C1A2mp);
				M2 = Aux1;
				//Attack3 = C1A3name;
				//Attack4 = C1A4name;
				//Attack5 = C1A5name;
				//Attack6 = C1A6name;
			}
			if (Turn[0] == 2) {
				Attack1 = C2A1name;
				Attack2 = C2A2name;
				sprintf_s(Aux1, "%.0f", C2A2mp);
				M2 = Aux1;
				//Attack3 = C2A3name;
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
				}
				if (EnemySelect == true) {
					if (option == COMBATMENU::ENEMY2) {
						option = COMBATMENU::ENEMY1;
					}
					if (option == COMBATMENU::ENEMY3) {
						option = COMBATMENU::ENEMY2;
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
					if (option == COMBATMENU::ATTACK1) {
						option = COMBATMENU::ATTACK2;
					}
				}
				if (EnemySelect == true) {
					if (option == COMBATMENU::ENEMY2) {
						option = COMBATMENU::ENEMY3;
					}
					if (option == COMBATMENU::ENEMY1) {
						option = COMBATMENU::ENEMY2;
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
				}
				//Perform the attack
				if (EnemySelect == true && cd == 0) {
					if (option == COMBATMENU::ENEMY1 && E1dead == false) {
						if (Turn[0] == 1) {
							if (AttackSelected == 1) {
								int multiplier = 1, multiplier2 = 1;
								if (E1Weak == "laurea") multiplier = 2;
								if (E1Res == "laurea") multiplier2 = 2;
								E1CHP = E1CHP - (((C1A1dmg * (C1ATK / E1DEF)) * multiplier) / multiplier2);
								FinishTurn();
							}
							if (AttackSelected == 2 && C1CMP >= C1A2mp) {
								C1CMP -= C1A2mp;
								int multiplier = 1, multiplier2 = 1;
								if (E1Weak == "laurea") multiplier = 2;
								if (E1Res == "laurea") multiplier2 = 2;
								E1CHP = E1CHP - (((C1A2dmg * (C1ATK / E1DEF)) * multiplier) / multiplier2);
								FinishTurn();
							}
						}
						if (Turn[0] == 2) {
							if (AttackSelected == 1) {
								int multiplier = 1, multiplier2 = 1;
								if (E1Weak == "lapis") multiplier = 2;
								if (E1Res == "lapis") multiplier2 = 2;
								E1CHP = E1CHP - (((C2A1dmg * (C2ATK / E1DEF)) * multiplier) / multiplier2);
								FinishTurn();
							}
							if (AttackSelected == 2 && C2CMP >= C2A2mp) {
								C2CMP -= C2A2mp;
								int multiplier = 1, multiplier2 = 1;
								if (E1Weak == "lapis") multiplier = 2;
								if (E1Res == "lapis") multiplier2 = 2;
								E1CHP = E1CHP - (((C2A2dmg * (C2ATK / E1DEF)) * multiplier) / multiplier2);
								if (E2Weak == "lapis") multiplier = 2;
								if (E2Res == "lapis") multiplier2 = 2;
								E2CHP = E2CHP - (((C2A2dmg * (C2ATK / E2DEF)) * multiplier) / multiplier2);
								if (E3Weak == "lapis") multiplier = 2;
								if (E3Res == "lapis") multiplier2 = 2;
								E3CHP = E3CHP - (((C2A2dmg * (C2ATK / E3DEF)) * multiplier) / multiplier2);
								FinishTurn();
							}
						}
						if (Turn[0] == 3) {
							if (AttackSelected == 1) {
								int multiplier = 1, multiplier2 = 1;
								if (E1Weak == "lucca") multiplier = 2;
								if (E1Res == "lucca") multiplier2 = 2;
								E1CHP = E1CHP - (((C3A1dmg * (C3ATK / E1DEF)) * multiplier) / multiplier2);
								FinishTurn();
							}
							if (AttackSelected == 2 && C3CMP >= C3A2mp) {
								C3CMP -= C3A2mp;
								int multiplier = 1, multiplier2 = 1;
								if (E1Weak == "lucca") multiplier = 2;
								if (E1Res == "lucca") multiplier2 = 2;
								E1CHP = E1CHP - (((C3A2dmg * (C3ATK / E1DEF)) * multiplier) / multiplier2);
								if (E2Weak == "lucca") multiplier = 2;
								if (E2Res == "lucca") multiplier2 = 2;
								E2CHP = E2CHP - (((C3A2dmg * (C3ATK / E2DEF)) * multiplier) / multiplier2);
								if (E3Weak == "lucca") multiplier = 2;
								if (E3Res == "lucca") multiplier2 = 2;
								E3CHP = E3CHP - (((C3A2dmg * (C3ATK / E3DEF)) * multiplier) / multiplier2);
								FinishTurn();
							}
						}
					}
					if (option == COMBATMENU::ENEMY2 && E2dead == false) {
						if (Turn[0] == 1) {
							if (AttackSelected == 1) {
								int multiplier = 1, multiplier2 = 1;
								if (E2Weak == "laurea") multiplier = 2;
								if (E2Res == "laurea") multiplier2 = 2;
								E2CHP = E2CHP - (((C1A1dmg * (C1ATK / E2DEF)) * multiplier) / multiplier2);
								FinishTurn();
							}
							if (AttackSelected == 2 && C1CMP >= C1A2mp) {
								C1CMP -= C1A2mp;
								int multiplier = 1, multiplier2 = 1;
								if (E2Weak == "laurea") multiplier = 2;
								if (E2Res == "laurea") multiplier2 = 2;
								E2CHP = E2CHP - (((C1A2dmg * (C1ATK / E2DEF)) * multiplier) / multiplier2);
								FinishTurn();
							}
						}
						if (Turn[0] == 2) {
							if (AttackSelected == 1) {
								int multiplier = 1, multiplier2 = 1;
								if (E2Weak == "lapis") multiplier = 2;
								if (E2Res == "lapis") multiplier2 = 2;
								E2CHP = E2CHP - (((C2A1dmg * (C2ATK / E2DEF)) * multiplier) / multiplier2);
								FinishTurn();
							}
							if (AttackSelected == 2 && C2CMP >= C2A2mp) {
								C2CMP -= C2A2mp;
								int multiplier = 1, multiplier2 = 1;
								if (E1Weak == "lapis") multiplier = 2;
								if (E1Res == "lapis") multiplier2 = 2;
								E1CHP = E1CHP - (((C2A2dmg * (C2ATK / E1DEF)) * multiplier) / multiplier2);
								if (E2Weak == "lapis") multiplier = 2;
								if (E2Res == "lapis") multiplier2 = 2;
								E2CHP = E2CHP - (((C2A2dmg * (C2ATK / E2DEF)) * multiplier) / multiplier2);
								if (E3Weak == "lapis") multiplier = 2;
								if (E3Res == "lapis") multiplier2 = 2;
								E3CHP = E3CHP - (((C2A2dmg * (C2ATK / E3DEF)) * multiplier) / multiplier2);
								FinishTurn();
							}
						}
						if (Turn[0] == 3) {
							if (AttackSelected == 1) {
								int multiplier = 1, multiplier2 = 1;
								if (E2Weak == "lucca") multiplier = 2;
								if (E2Res == "lucca") multiplier2 = 2;
								E2CHP = E2CHP - (((C3A1dmg * (C3ATK / E2DEF)) * multiplier) / multiplier2);
								FinishTurn();
							}
							if (AttackSelected == 2 && C3CMP >= C3A2mp) {
								C3CMP -= C3A2mp;
								int multiplier = 1, multiplier2 = 1;
								if (E1Weak == "lucca") multiplier = 2;
								if (E1Res == "lucca") multiplier2 = 2;
								E1CHP = E1CHP - (((C3A2dmg * (C3ATK / E1DEF)) * multiplier) / multiplier2);
								if (E2Weak == "lucca") multiplier = 2;
								if (E2Res == "lucca") multiplier2 = 2;
								E2CHP = E2CHP - (((C3A2dmg * (C3ATK / E2DEF)) * multiplier) / multiplier2);
								if (E3Weak == "lucca") multiplier = 2;
								if (E3Res == "lucca") multiplier2 = 2;
								E3CHP = E3CHP - (((C3A2dmg * (C3ATK / E3DEF)) * multiplier) / multiplier2);
								FinishTurn();
							}
						}
					}
					if (option == COMBATMENU::ENEMY3 && E3dead == false) {
						if (Turn[0] == 1) {
							if (AttackSelected == 1) {
								int multiplier = 1, multiplier2 = 1;
								if (E3Weak == "laurea") multiplier = 2;
								if (E3Res == "laurea") multiplier2 = 2;
								E3CHP = E3CHP - (((C1A1dmg * (C1ATK / E3DEF)) * multiplier) / multiplier2);
								FinishTurn();
							}
							if (AttackSelected == 2 && C1CMP >= C1A2mp) {
								C1CMP -= C1A2mp;
								int multiplier = 1, multiplier2 = 1;
								if (E3Weak == "laurea") multiplier = 2;
								if (E3Res == "laurea") multiplier2 = 2;
								E3CHP = E3CHP - (((C1A2dmg * (C1ATK / E3DEF)) * multiplier) / multiplier2);
								FinishTurn();
							}
						}
						if (Turn[0] == 2) {
							if (AttackSelected == 1) {
								int multiplier = 1, multiplier2 = 1;
								if (E3Weak == "lapis") multiplier = 2;
								if (E3Res == "lapis") multiplier2 = 2;
								E3CHP = E3CHP - (((C2A1dmg * (C2ATK / E3DEF)) * multiplier) / multiplier2);
								FinishTurn();
							}
							if (AttackSelected == 2 && C2CMP >= C2A2mp) {
								C2CMP -= C2A2mp;
								int multiplier = 1, multiplier2 = 1;
								if (E1Weak == "lapis") multiplier = 2;
								if (E1Res == "lapis") multiplier2 = 2;
								E1CHP = E1CHP - (((C2A2dmg * (C2ATK / E1DEF)) * multiplier) / multiplier2);
								if (E2Weak == "lapis") multiplier = 2;
								if (E2Res == "lapis") multiplier2 = 2;
								E2CHP = E2CHP - (((C2A2dmg * (C2ATK / E2DEF)) * multiplier) / multiplier2);
								if (E3Weak == "lapis") multiplier = 2;
								if (E3Res == "lapis") multiplier2 = 2;
								E3CHP = E3CHP - (((C2A2dmg * (C2ATK / E3DEF)) * multiplier) / multiplier2);
								FinishTurn();
							}
						}
						if (Turn[0] == 3) {
							if (AttackSelected == 1) {
								int multiplier = 1, multiplier2 = 1;
								if (E3Weak == "lucca") multiplier = 2;
								if (E3Res == "lucca") multiplier2 = 2;
								E3CHP = E3CHP - (((C3A1dmg * (C3ATK / E3DEF)) * multiplier) / multiplier2);
								FinishTurn();
							}
							if (AttackSelected == 2 && C3CMP >= C3A2mp) {
								C3CMP -= C3A2mp;
								int multiplier = 1, multiplier2 = 1;
								if (E1Weak == "lucca") multiplier = 2;
								if (E1Res == "lucca") multiplier2 = 2;
								E1CHP = E1CHP - (((C3A2dmg * (C3ATK / E1DEF)) * multiplier) / multiplier2);
								if (E2Weak == "lucca") multiplier = 2;
								if (E2Res == "lucca") multiplier2 = 2;
								E2CHP = E2CHP - (((C3A2dmg * (C3ATK / E2DEF)) * multiplier) / multiplier2);
								if (E3Weak == "lucca") multiplier = 2;
								if (E3Res == "lucca") multiplier2 = 2;
								E3CHP = E3CHP - (((C3A2dmg * (C3ATK / E3DEF)) * multiplier) / multiplier2);
								FinishTurn();
							}
						}
					}
				}

			}
		}
		//Enemy action
		if (option == COMBATMENU::NONE) {
			if (EnemyAdone == false) {
				int EnemyAttackTarget = rand() % CurrentCharacters + 1;
				int EnemyAttackNum = rand() % 2 + 1;
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
							int block = 1, multiplier1 = 1, multiplier2 = 1;
							if (E3A1target == 0) {
								if (E3class == 2) multiplier1 = 2;
								if (E3class == 3) multiplier2 = 2;
								if (C1Block == true) block = 3;
								C1CHP = C1CHP - ((E3A1dmg * (E3ATK / C1DEF) * multiplier1 / multiplier2) / block);
								Cname = C1NAME;
								Ename = E3name;
								Aname = E3A1name;
							}
							if (E3A1target == 1) {
								if (E3class == 2) multiplier1 = 2;
								if (E3class == 3) multiplier2 = 2;
								if (C1Block == true) block = 3;
								C1CHP = C1CHP - ((E3A1dmg * (E3ATK / C1DEF) * multiplier1 / multiplier2) / block);
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E3class == 3) multiplier1 = 2;
								if (E3class == 1) multiplier2 = 2;
								if (C2Block == true) block = 3;
								C2CHP = C2CHP - ((E3A1dmg * (E3ATK / C2DEF) * multiplier1 / multiplier2) / block);
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E3class == 1) multiplier1 = 2;
								if (E3class == 2) multiplier2 = 2;
								if (C3Block == true) block = 3;
								C3CHP = C3CHP - ((E3A1dmg * (E3ATK / C3DEF) * multiplier1 / multiplier2) / block);
								Cname = "everyone";
								Ename = E3name;
								Aname = E3A1name;
							}
						}
						if (EnemyAttackNum == 2) {
							int block = 1, multiplier1 = 1, multiplier2 = 1;
							if (E3A2target == 0) {
								if (E3class == 2) multiplier1 = 2;
								if (E3class == 3) multiplier2 = 2;
								if (C1Block == true) block = 3;
								C1CHP = C1CHP - ((E3A2dmg * (E3ATK / C1DEF) * multiplier1 / multiplier2) / block);
								Cname = C1NAME;
								Ename = E3name;
								Aname = E3A2name;
							}
							if (E3A2target == 1) {
								if (E3class == 2) multiplier1 = 2;
								if (E3class == 3) multiplier2 = 2;
								if (C1Block == true) block = 3;
								C1CHP = C1CHP - ((E3A2dmg * (E3ATK / C1DEF) * multiplier1 / multiplier2) / block);
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E3class == 3) multiplier1 = 2;
								if (E3class == 1) multiplier2 = 2;
								if (C2Block == true) block = 3;
								C2CHP = C2CHP - ((E3A2dmg * (E3ATK / C2DEF) * multiplier1 / multiplier2) / block);
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E3class == 1) multiplier1 = 2;
								if (E3class == 2) multiplier2 = 2;
								if (C3Block == true) block = 3;
								C3CHP = C3CHP - ((E3A2dmg * (E3ATK / C3DEF) * multiplier1 / multiplier2) / block);
								Cname = "everyone";
								Ename = E3name;
								Aname = E3A2name;
							}
						}
					}
					if (EnemyAttackTarget == 2) {
						if (EnemyAttackNum == 1) {
							int block = 1, multiplier1 = 1, multiplier2 = 1;
							if (E3A1target == 0) {
								if (E3class == 3) multiplier1 = 2;
								if (E3class == 1) multiplier2 = 2;
								if (C2Block == true) block = 3;
								C2CHP = C2CHP - ((E3A1dmg * (E3ATK / C2DEF) * multiplier1 / multiplier2) / block);
								Cname = C2NAME;
								Ename = E3name;
								Aname = E3A1name;
							}
							if (E3A1target == 1) {
								if (E3class == 2) multiplier1 = 2;
								if (E3class == 3) multiplier2 = 2;
								if (C1Block == true) block = 3;
								C1CHP = C1CHP - ((E3A1dmg * (E3ATK / C1DEF) * multiplier1 / multiplier2) / block);
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E3class == 3) multiplier1 = 2;
								if (E3class == 1) multiplier2 = 2;
								if (C2Block == true) block = 3;
								C2CHP = C2CHP - ((E3A1dmg * (E3ATK / C2DEF) * multiplier1 / multiplier2) / block);
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E3class == 1) multiplier1 = 2;
								if (E3class == 2) multiplier2 = 2;
								if (C3Block == true) block = 3;
								C3CHP = C3CHP - ((E3A1dmg * (E3ATK / C3DEF) * multiplier1 / multiplier2) / block);
								Cname = "everyone";
								Ename = E3name;
								Aname = E3A1name;
							}
						}
						if (EnemyAttackNum == 2) {
							int block = 1, multiplier1 = 1, multiplier2 = 1;
							if (E3A2target == 0) {
								if (E3class == 3) multiplier1 = 2;
								if (E3class == 1) multiplier2 = 2;
								if (C2Block == true) block = 3;
								C2CHP = C2CHP - ((E3A2dmg * (E3ATK / C2DEF) * multiplier1 / multiplier2) / block);
								Cname = C2NAME;
								Ename = E3name;
								Aname = E3A2name;
							}
							if (E3A2target == 1) {
								if (E3class == 2) multiplier1 = 2;
								if (E3class == 3) multiplier2 = 2;
								if (C1Block == true) block = 3;
								C1CHP = C1CHP - ((E3A2dmg * (E3ATK / C1DEF) * multiplier1 / multiplier2) / block);
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E3class == 3) multiplier1 = 2;
								if (E3class == 1) multiplier2 = 2;
								if (C2Block == true) block = 3;
								C2CHP = C2CHP - ((E3A2dmg * (E3ATK / C2DEF) * multiplier1 / multiplier2) / block);
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E3class == 1) multiplier1 = 2;
								if (E3class == 2) multiplier2 = 2;
								if (C3Block == true) block = 3;
								C3CHP = C3CHP - ((E3A2dmg * (E3ATK / C3DEF) * multiplier1 / multiplier2) / block);
								Cname = "everyone";
								Ename = E3name;
								Aname = E3A2name;
							}
						}
					}
					if (EnemyAttackTarget == 3) {
						if (EnemyAttackNum == 1) {
							int block = 1, multiplier1 = 1, multiplier2 = 1;
							if (E3A1target == 0) {
								if (E3class == 1) multiplier1 = 2;
								if (E3class == 2) multiplier2 = 2;
								if (C3Block == true) block = 3;
								C3CHP = C3CHP - ((E3A1dmg * (E3ATK / C3DEF) * multiplier1 / multiplier2) / block);
								Cname = C3NAME;
								Ename = E3name;
								Aname = E3A1name;
							}
							if (E3A1target == 1) {
								if (E3class == 2) multiplier1 = 2;
								if (E3class == 3) multiplier2 = 2;
								if (C1Block == true) block = 3;
								C1CHP = C1CHP - ((E3A1dmg * (E3ATK / C1DEF) * multiplier1 / multiplier2) / block);
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E3class == 3) multiplier1 = 2;
								if (E3class == 1) multiplier2 = 2;
								if (C2Block == true) block = 3;
								C2CHP = C2CHP - ((E3A1dmg * (E3ATK / C2DEF) * multiplier1 / multiplier2) / block);
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E3class == 1) multiplier1 = 2;
								if (E3class == 2) multiplier2 = 2;
								if (C3Block == true) block = 3;
								C3CHP = C3CHP - ((E3A1dmg * (E3ATK / C3DEF) * multiplier1 / multiplier2) / block);
								Cname = "everyone";
								Ename = E3name;
								Aname = E3A1name;
							}
						}
						if (EnemyAttackNum == 2) {
							int block = 1, multiplier1 = 1, multiplier2 = 1;
							if (E3A2target == 0) {
								if (E3class == 1) multiplier1 = 2;
								if (E3class == 2) multiplier2 = 2;
								if (C3Block == true) block = 3;
								C3CHP = C3CHP - ((E3A2dmg * (E3ATK / C3DEF) * multiplier1 / multiplier2) / block);
								Cname = C3NAME;
								Ename = E3name;
								Aname = E3A2name;
							}
							if (E3A2target == 1) {
								if (E3class == 2) multiplier1 = 2;
								if (E3class == 3) multiplier2 = 2;
								if (C1Block == true) block = 3;
								C1CHP = C1CHP - ((E3A2dmg * (E3ATK / C1DEF) * multiplier1 / multiplier2) / block);
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E3class == 3) multiplier1 = 2;
								if (E3class == 1) multiplier2 = 2;
								if (C2Block == true) block = 3;
								C2CHP = C2CHP - ((E3A2dmg * (E3ATK / C2DEF) * multiplier1 / multiplier2) / block);
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E3class == 1) multiplier1 = 2;
								if (E3class == 2) multiplier2 = 2;
								if (C3Block == true) block = 3;
								C3CHP = C3CHP - ((E3A2dmg * (E3ATK / C3DEF) * multiplier1 / multiplier2) / block);
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
							int block = 1, multiplier1 = 1, multiplier2 = 1;
							if (E2A1target == 0) {
								if (E2class == 2) multiplier1 = 2;
								if (E2class == 3) multiplier2 = 2;
								if (C1Block == true) block = 3;
								C1CHP = C1CHP - ((E2A1dmg * (E2ATK / C1DEF) * multiplier1 / multiplier2) / block);
								Cname = C1NAME;
								Ename = E2name;
								Aname = E2A1name;
							}
							if (E2A1target == 1) {
								if (E2class == 2) multiplier1 = 2;
								if (E2class == 3) multiplier2 = 2;
								if (C1Block == true) block = 3;
								C1CHP = C1CHP - ((E2A1dmg * (E2ATK / C1DEF) * multiplier1 / multiplier2) / block);
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E2class == 3) multiplier1 = 2;
								if (E2class == 1) multiplier2 = 2;
								if (C2Block == true) block = 3;
								C2CHP = C2CHP - ((E2A1dmg * (E2ATK / C2DEF) * multiplier1 / multiplier2) / block);
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E2class == 1) multiplier1 = 2;
								if (E2class == 2) multiplier2 = 2;
								if (C3Block == true) block = 3;
								C3CHP = C3CHP - ((E2A1dmg * (E2ATK / C3DEF) * multiplier1 / multiplier2) / block);
								Cname = "everyone";
								Ename = E2name;
								Aname = E2A1name;
							}
						}
						if (EnemyAttackNum == 2) {
							int block = 1, multiplier1 = 1, multiplier2 = 1;
							if (E2A2target == 0) {
								if (E2class == 2) multiplier1 = 2;
								if (E2class == 3) multiplier2 = 2;
								if (C1Block == true) block = 3;
								C1CHP = C1CHP - ((E2A2dmg * (E2ATK / C1DEF) * multiplier1 / multiplier2) / block);
								Cname = C1NAME;
								Ename = E2name;
								Aname = E2A2name;
							}
							if (E2A2target == 1) {
								if (E2class == 2) multiplier1 = 2;
								if (E2class == 3) multiplier2 = 2;
								if (C1Block == true) block = 3;
								C1CHP = C1CHP - ((E2A2dmg * (E2ATK / C1DEF) * multiplier1 / multiplier2) / block);
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E2class == 3) multiplier1 = 2;
								if (E2class == 1) multiplier2 = 2;
								if (C2Block == true) block = 3;
								C2CHP = C2CHP - ((E2A2dmg * (E2ATK / C2DEF) * multiplier1 / multiplier2) / block);
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E2class == 1) multiplier1 = 2;
								if (E2class == 2) multiplier2 = 2;
								if (C3Block == true) block = 3;
								C3CHP = C3CHP - ((E2A2dmg * (E2ATK / C3DEF) * multiplier1 / multiplier2) / block);
								Cname = "everyone";
								Ename = E2name;
								Aname = E2A2name;
							}
						}
					}
					if (EnemyAttackTarget == 2) {
						if (EnemyAttackNum == 1) {
							int block = 1, multiplier1 = 1, multiplier2 = 1;
							if (E2A1target == 0) {
								if (E2class == 3) multiplier1 = 2;
								if (E2class == 1) multiplier2 = 2;
								if (C2Block == true) block = 3;
								C2CHP = C2CHP - ((E2A1dmg * (E2ATK / C2DEF) * multiplier1 / multiplier2) / block);
								Cname = C2NAME;
								Ename = E2name;
								Aname = E2A1name;
							}
							if (E2A1target == 1) {
								if (E2class == 2) multiplier1 = 2;
								if (E2class == 3) multiplier2 = 2;
								if (C1Block == true) block = 3;
								C1CHP = C1CHP - ((E2A1dmg * (E2ATK / C1DEF) * multiplier1 / multiplier2) / block);
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E2class == 3) multiplier1 = 2;
								if (E2class == 1) multiplier2 = 2;
								if (C2Block == true) block = 3;
								C2CHP = C2CHP - ((E2A1dmg * (E2ATK / C2DEF) * multiplier1 / multiplier2) / block);
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E2class == 1) multiplier1 = 2;
								if (E2class == 2) multiplier2 = 2;
								if (C3Block == true) block = 3;
								C3CHP = C3CHP - ((E2A1dmg * (E2ATK / C3DEF) * multiplier1 / multiplier2) / block);
								Cname = "everyone";
								Ename = E2name;
								Aname = E2A1name;
							}
						}
						if (EnemyAttackNum == 2) {
							int block = 1, multiplier1 = 1, multiplier2 = 1;
							if (E2A2target == 0) {
								if (E2class == 3) multiplier1 = 2;
								if (E2class == 1) multiplier2 = 2;
								if (C2Block == true) block = 3;
								C2CHP = C2CHP - ((E2A2dmg * (E2ATK / C2DEF) * multiplier1 / multiplier2) / block);
								Cname = C2NAME;
								Ename = E2name;
								Aname = E2A2name;
							}
							if (E2A2target == 1) {
								if (E2class == 2) multiplier1 = 2;
								if (E2class == 3) multiplier2 = 2;
								if (C1Block == true) block = 3;
								C1CHP = C1CHP - ((E2A2dmg * (E2ATK / C1DEF) * multiplier1 / multiplier2) / block);
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E2class == 3) multiplier1 = 2;
								if (E2class == 1) multiplier2 = 2;
								if (C2Block == true) block = 3;
								C2CHP = C2CHP - ((E2A2dmg * (E2ATK / C2DEF) * multiplier1 / multiplier2) / block);
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E2class == 1) multiplier1 = 2;
								if (E2class == 2) multiplier2 = 2;
								if (C3Block == true) block = 3;
								C3CHP = C3CHP - ((E2A2dmg * (E2ATK / C3DEF) * multiplier1 / multiplier2) / block);
								Cname = "everyone";
								Ename = E2name;
								Aname = E2A2name;
							}
						}
					}
					if (EnemyAttackTarget == 3) {
						if (EnemyAttackNum == 1) {
							int block = 1, multiplier1 = 1, multiplier2 = 1;
							if (E2A1target == 0) {
								if (E2class == 1) multiplier1 = 2;
								if (E2class == 2) multiplier2 = 2;
								if (C3Block == true) block = 3;
								C3CHP = C3CHP - ((E2A1dmg * (E2ATK / C3DEF) * multiplier1 / multiplier2) / block);
								Cname = C3NAME;
								Ename = E2name;
								Aname = E2A1name;
							}
							if (E2A1target == 1) {
								if (E2class == 2) multiplier1 = 2;
								if (E2class == 3) multiplier2 = 2;
								if (C1Block == true) block = 3;
								C1CHP = C1CHP - ((E2A1dmg * (E2ATK / C1DEF) * multiplier1 / multiplier2) / block);
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E2class == 3) multiplier1 = 2;
								if (E2class == 1) multiplier2 = 2;
								if (C2Block == true) block = 3;
								C2CHP = C2CHP - ((E2A1dmg * (E2ATK / C2DEF) * multiplier1 / multiplier2) / block);
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E2class == 1) multiplier1 = 2;
								if (E2class == 2) multiplier2 = 2;
								if (C3Block == true) block = 3;
								C3CHP = C3CHP - ((E2A1dmg * (E2ATK / C3DEF) * multiplier1 / multiplier2) / block);
								Cname = "everyone";
								Ename = E2name;
								Aname = E2A1name;
							}
						}
						if (EnemyAttackNum == 2) {
							int block = 1, multiplier1 = 1, multiplier2 = 1;
							if (E2A2target == 0) {
								if (E2class == 1) multiplier1 = 2;
								if (E2class == 2) multiplier2 = 2;
								if (C3Block == true) block = 3;
								C3CHP = C3CHP - ((E2A2dmg * (E2ATK / C3DEF) * multiplier1 / multiplier2) / block);
								Cname = C3NAME;
								Ename = E2name;
								Aname = E2A2name;
							}
							if (E2A2target == 1) {
								if (E2class == 2) multiplier1 = 2;
								if (E2class == 3) multiplier2 = 2;
								if (C1Block == true) block = 3;
								C1CHP = C1CHP - ((E2A2dmg * (E2ATK / C1DEF) * multiplier1 / multiplier2) / block);
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E2class == 3) multiplier1 = 2;
								if (E2class == 1) multiplier2 = 2;
								if (C2Block == true) block = 3;
								C2CHP = C2CHP - ((E2A2dmg * (E2ATK / C2DEF) * multiplier1 / multiplier2) / block);
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E2class == 1) multiplier1 = 2;
								if (E2class == 2) multiplier2 = 2;
								if (C3Block == true) block = 3;
								C3CHP = C3CHP - ((E2A2dmg * (E2ATK / C3DEF) * multiplier1 / multiplier2) / block);
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
							int block = 1, multiplier1 = 1, multiplier2 = 1;
							if (E1A1target == 0) {
								if (E1class == 2) multiplier1 = 2;
								if (E1class == 3) multiplier2 = 2;
								if (C1Block == true) block = 3;
								C1CHP = C1CHP - ((E1A1dmg * (E1ATK / C1DEF) * multiplier1 / multiplier2) / block);
								Cname = C1NAME;
								Ename = E1name;
								Aname = E1A1name;
							}
							if (E1A1target == 1) {
								if (E1class == 2) multiplier1 = 2;
								if (E1class == 3) multiplier2 = 2;
								if (C1Block == true) block = 3;
								C1CHP = C1CHP - ((E1A1dmg * (E1ATK / C1DEF) * multiplier1 / multiplier2) / block);
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E1class == 3) multiplier1 = 2;
								if (E1class == 1) multiplier2 = 2;
								if (C2Block == true) block = 3;
								C2CHP = C2CHP - ((E1A1dmg * (E1ATK / C2DEF) * multiplier1 / multiplier2) / block);
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E1class == 1) multiplier1 = 2;
								if (E1class == 2) multiplier2 = 2;
								if (C3Block == true) block = 3;
								C3CHP = C3CHP - ((E1A1dmg * (E1ATK / C3DEF) * multiplier1 / multiplier2) / block);
								Cname = "everyone";
								Ename = E1name;
								Aname = E1A1name;
							}
						}
						if (EnemyAttackNum == 2) {
							int block = 1, multiplier1 = 1, multiplier2 = 1;
							if (E1A2target == 0) {
								if (E1class == 2) multiplier1 = 2;
								if (E1class == 3) multiplier2 = 2;
								if (C1Block == true) block = 3;
								C1CHP = C1CHP - ((E1A2dmg * (E1ATK / C1DEF) * multiplier1 / multiplier2) / block);
								Cname = C1NAME;
								Ename = E1name;
								Aname = E1A2name;
							}
							if (E1A2target == 1) {
								if (E1class == 2) multiplier1 = 2;
								if (E1class == 3) multiplier2 = 2;
								if (C1Block == true) block = 3;
								C1CHP = C1CHP - ((E1A2dmg * (E1ATK / C1DEF) * multiplier1 / multiplier2) / block);
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E1class == 3) multiplier1 = 2;
								if (E1class == 1) multiplier2 = 2;
								if (C2Block == true) block = 3;
								C2CHP = C2CHP - ((E1A2dmg * (E1ATK / C2DEF) * multiplier1 / multiplier2) / block);
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E1class == 1) multiplier1 = 2;
								if (E1class == 2) multiplier2 = 2;
								if (C3Block == true) block = 3;
								C3CHP = C3CHP - ((E1A2dmg * (E1ATK / C3DEF) * multiplier1 / multiplier2) / block);
								Cname = "everyone";
								Ename = E1name;
								Aname = E1A2name;
							}
						}
					}
					if (EnemyAttackTarget == 2) {
						if (EnemyAttackNum == 1) {
							int block = 1, multiplier1 = 1, multiplier2 = 1;
							if (E1A1target == 0) {
								if (E1class == 3) multiplier1 = 2;
								if (E1class == 1) multiplier2 = 2;
								if (C2Block == true) block = 3;
								C2CHP = C2CHP - ((E1A1dmg * (E1ATK / C2DEF) * multiplier1 / multiplier2) / block);
								Cname = C2NAME;
								Ename = E1name;
								Aname = E1A1name;
							}
							if (E1A1target == 1) {
								if (E1class == 2) multiplier1 = 2;
								if (E1class == 3) multiplier2 = 2;
								if (C1Block == true) block = 3;
								C1CHP = C1CHP - ((E1A1dmg * (E1ATK / C1DEF) * multiplier1 / multiplier2) / block);
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E1class == 3) multiplier1 = 2;
								if (E1class == 1) multiplier2 = 2;
								if (C2Block == true) block = 3;
								C2CHP = C2CHP - ((E1A1dmg * (E1ATK / C2DEF) * multiplier1 / multiplier2) / block);
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E1class == 1) multiplier1 = 2;
								if (E1class == 2) multiplier2 = 2;
								if (C3Block == true) block = 3;
								C3CHP = C3CHP - ((E1A1dmg * (E1ATK / C3DEF) * multiplier1 / multiplier2) / block);
								Cname = "everyone";
								Ename = E1name;
								Aname = E1A1name;
							}
						}
						if (EnemyAttackNum == 2) {
							int block = 1, multiplier1 = 1, multiplier2 = 1;
							if (E1A2target == 0) {
								if (E1class == 3) multiplier1 = 2;
								if (E1class == 1) multiplier2 = 2;
								if (C2Block == true) block = 3;
								C2CHP = C2CHP - ((E1A2dmg * (E1ATK / C2DEF) * multiplier1 / multiplier2) / block);
								Cname = C2NAME;
								Ename = E1name;
								Aname = E1A2name;
							}
							if (E1A2target == 1) {
								if (E1class == 2) multiplier1 = 2;
								if (E1class == 3) multiplier2 = 2;
								if (C1Block == true) block = 3;
								C1CHP = C1CHP - ((E1A2dmg * (E1ATK / C1DEF) * multiplier1 / multiplier2) / block);
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E1class == 3) multiplier1 = 2;
								if (E1class == 1) multiplier2 = 2;
								if (C2Block == true) block = 3;
								C2CHP = C2CHP - ((E1A2dmg * (E1ATK / C2DEF) * multiplier1 / multiplier2) / block);
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E1class == 1) multiplier1 = 2;
								if (E1class == 2) multiplier2 = 2;
								if (C3Block == true) block = 3;
								C3CHP = C3CHP - ((E1A2dmg * (E1ATK / C3DEF) * multiplier1 / multiplier2) / block);
								Cname = "everyone";
								Ename = E1name;
								Aname = E1A2name;
							}
						}
					}
					if (EnemyAttackTarget == 3) {
						if (EnemyAttackNum == 1) {
							int block = 1, multiplier1 = 1, multiplier2 = 1;
							if (E1A1target == 0) {
								if (E1class == 1) multiplier1 = 2;
								if (E1class == 2) multiplier2 = 2;
								if (C3Block == true) block = 3;
								C3CHP = C3CHP - ((E1A1dmg * (E1ATK / C3DEF) * multiplier1 / multiplier2) / block);
								Cname = C3NAME;
								Ename = E1name;
								Aname = E1A1name;
							}
							if (E1A1target == 1) {
								if (E1class == 2) multiplier1 = 2;
								if (E1class == 3) multiplier2 = 2;
								if (C1Block == true) block = 3;
								C1CHP = C1CHP - ((E1A1dmg * (E1ATK / C1DEF) * multiplier1 / multiplier2) / block);
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E1class == 3) multiplier1 = 2;
								if (E1class == 1) multiplier2 = 2;
								if (C2Block == true) block = 3;
								C2CHP = C2CHP - ((E1A1dmg * (E1ATK / C2DEF) * multiplier1 / multiplier2) / block);
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E1class == 1) multiplier1 = 2;
								if (E1class == 2) multiplier2 = 2;
								if (C3Block == true) block = 3;
								C3CHP = C3CHP - ((E1A1dmg * (E1ATK / C3DEF) * multiplier1 / multiplier2) / block);
								Cname = "everyone";
								Ename = E1name;
								Aname = E1A1name;
							}
						}
						if (EnemyAttackNum == 2) {
							int block = 1, multiplier1 = 1, multiplier2 = 1;
							if (E1A2target == 0) {
								if (E1class == 1) multiplier1 = 2;
								if (E1class == 2) multiplier2 = 2;
								if (C3Block == true) block = 3;
								C3CHP = C3CHP - ((E1A2dmg * (E1ATK / C3DEF) * multiplier1 / multiplier2) / block);
								Cname = C3NAME;
								Ename = E1name;
								Aname = E1A2name;
							}
							if (E1A2target == 1) {
								if (E1class == 2) multiplier1 = 2;
								if (E1class == 3) multiplier2 = 2;
								if (C1Block == true) block = 3;
								C1CHP = C1CHP - ((E1A2dmg * (E1ATK / C1DEF) * multiplier1 / multiplier2) / block);
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E1class == 3) multiplier1 = 2;
								if (E1class == 1) multiplier2 = 2;
								if (C2Block == true) block = 3;
								C2CHP = C2CHP - ((E1A2dmg * (E1ATK / C2DEF) * multiplier1 / multiplier2) / block);
								block = 1;
								multiplier1 = 1;
								multiplier2 = 1;
								if (E1class == 1) multiplier1 = 2;
								if (E1class == 2) multiplier2 = 2;
								if (C3Block == true) block = 3;
								C3CHP = C3CHP - ((E1A2dmg * (E1ATK / C3DEF) * multiplier1 / multiplier2) / block);
								Cname = "everyone";
								Ename = E1name;
								Aname = E1A2name;
							}
						}
					}
				}
				EnemyAdone = true;
			}
			app->font->BlitText(10 * app->ScalingMultiplier, 100 * app->ScalingMultiplier, YF, Ename);
			app->font->BlitText(10 * app->ScalingMultiplier, 110 * app->ScalingMultiplier, WF, "used");
			app->font->BlitText(10 * app->ScalingMultiplier, 120 * app->ScalingMultiplier, YF, Aname);
			app->font->BlitText(10 * app->ScalingMultiplier, 130 * app->ScalingMultiplier, WF, "against");
			app->font->BlitText(10 * app->ScalingMultiplier, 140 * app->ScalingMultiplier, YF, Cname);
			if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
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
		app->font->BlitText(10 * app->ScalingMultiplier, 100 * app->ScalingMultiplier, YF, "you");
		app->font->BlitText(10 * app->ScalingMultiplier + 24, 100 * app->ScalingMultiplier, YF, "won");
		app->font->BlitText(10 * app->ScalingMultiplier, 110 * app->ScalingMultiplier, YF, "and");
		app->font->BlitText(10 * app->ScalingMultiplier + 24, 110 * app->ScalingMultiplier, YF, "got");
		sprintf_s(Aux, "%d", EXPwon);
		app->font->BlitText(10 * app->ScalingMultiplier, 120 * app->ScalingMultiplier, YF, Aux);
		app->font->BlitText(10 * app->ScalingMultiplier + 24, 120 * app->ScalingMultiplier, YF, "exp");
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
			ExitCombat();
		}
	}
	if (option == COMBATMENU::LOSE) {
		app->font->BlitText(10 * app->ScalingMultiplier, 100 * app->ScalingMultiplier, YF, "you");
		app->font->BlitText(10 * app->ScalingMultiplier, 110 * app->ScalingMultiplier, YF, "lost");
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
			ExitCombat();
		}
	}

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
	TeamTurn = 1;
	LoadLaurea(app->scene->player->laurea);
	LoadLapis(app->scene->player->lapis);
	LoadLucca(app->scene->player->lucca);
	//LoadEnemy(app->entityManager->waterlilyfish);
	//LoadEnemy(app->entityManager->waterlilyfish);
	//LoadEnemy(app->entityManager->naiadongoddess);
	//LoadEnemy(app->entityManager->waterlilyfish);
	LoadEnemy(app->entityManager->slimeFrog);
	LoadEnemy(app->entityManager->slimeFrog);
	LoadEnemy(app->entityManager->slimeFrog);
	TurnOrder();
	EXPwon = E1EXP + E2EXP + E3EXP;
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
	M1 = 0;
	M2 = 0;

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
		E3ATK = enemy.atk;
		E3DEF = enemy.def;
		E3Weak = enemy.weakness;
		E3Res = enemy.resistance;
		E3name = enemy.name;
		E3A1dmg = enemy.A1dmg;
		E3A1name = enemy.A1name;
		E3A1target = enemy.A1target;
		E3A2dmg = enemy.A2dmg;
		E3A2name = enemy.A2name;
		E3A2target = enemy.A2target;
		E3A3dmg = enemy.A3dmg;
		E3A3name = enemy.A3name;
		E3A3target = enemy.A3target;
		E3A4dmg = enemy.A4dmg;
		E3A4name = enemy.A4name;
		E3A4target = enemy.A4target;
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
	}
	if (CurrentEnemies == 1) {
		E2speed = enemy.spe;
		CurrentEnemies++;

		E2EXP = enemy.exp;
		E2MHP = enemy.hp;
		E2CHP = enemy.chp;
		E2ATK = enemy.atk;
		E2DEF = enemy.def;
		E2Weak = enemy.weakness;
		E2Res = enemy.resistance;
		E2name = enemy.name;
		E2A1dmg = enemy.A1dmg;
		E2A1name = enemy.A1name;
		E2A1target = enemy.A1target;
		E2A2dmg = enemy.A2dmg;
		E2A2name = enemy.A2name;
		E2A2target = enemy.A2target;
		E2A3dmg = enemy.A3dmg;
		E2A3name = enemy.A3name;
		E2A3target = enemy.A3target;
		E2A4dmg = enemy.A4dmg;
		E2A4name = enemy.A4name;
		E2A4target = enemy.A4target;
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
	}
	if (CurrentEnemies == 0) {
		E1speed = enemy.spe;
		CurrentEnemies++;

		E1EXP = enemy.exp;
		E1MHP = enemy.hp;
		E1CHP = enemy.chp;
		E1ATK = enemy.atk;
		E1DEF = enemy.def;
		E1Weak = enemy.weakness;
		E1Res = enemy.resistance;
		E1name = enemy.name;
		E1A1dmg = enemy.A1dmg;
		E1A1name = enemy.A1name;
		E1A1target = enemy.A1target;
		E1A2dmg = enemy.A2dmg;
		E1A2name = enemy.A2name;
		E1A2target = enemy.A2target;
		E1A3dmg = enemy.A3dmg;
		E1A3name = enemy.A3name;
		E1A3target = enemy.A3target;
		E1A4dmg = enemy.A4dmg;
		E1A4name = enemy.A4name;
		E1A4target = enemy.A4target;
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
	}
}

void Combat::RemoveEntityFromList(int id) {
	for (int i = 0; i < 6; i++) {
		if (Turn[i] == id) {
			Turn[i] = 0;
		}
	}
}
