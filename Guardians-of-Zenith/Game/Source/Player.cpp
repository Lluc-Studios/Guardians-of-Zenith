#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Window.h"
#include "DeathMenu.h"
#include "ModuleFonts.h"
#include "EntityManager.h"
#include "Map.h"
#include "Tavern.h"
#include "Combat.h"
#include "time.h"
#include "External/SDL/include/SDL_gamecontroller.h"
#include "Animation.h"
#include "Inventory.h"

#include "GuiManager.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");

}

Player::~Player() {

}

bool Player::Awake() {

	InitializePlayers();

	//LOG("stats Laurea: %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d", laurea.hp, laurea.mp, laurea.atk, laurea.def, laurea.spe, laurea.limit, laurea.lvl, laurea.hpGrowth, laurea.mpGrowth, laurea.atkGrowth, laurea.defGrowth);

	srand((unsigned int)time(NULL));

	return true;
}

bool Player::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	//pbody = app->physics->CreateRectangle(position.x + width/2, position.y + height/2, width-55, height-2, bodyType::DYNAMIC);
	pbody = app->physics->CreateRectangle(position.x, position.y, 18, 10, bodyType::DYNAMIC);
	//pbody = app->physics->CreateCircle(position.x + width / 2, position.y + height / 2,14/2, bodyType::DYNAMIC);
	pbody->body->SetFixedRotation(true);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	alive = true; 
	

	attackHitbox = app->physics->CreateRectangleSensor(0, 0, 25, 15, bodyType::STATIC);
	attackHitbox->ctype = ColliderType::PLAYER_ATTACK;

	//Sounds
	//Step1 = app->audio->LoadFx("Assets/Sounds/Player/FootGravel1.wav");
	//Step2 = app->audio->LoadFx("Assets/Sounds/Player/FootGravel2.wav");
	//StepMetalic1 = app->audio->LoadFx("Assets/Sounds/Player/StepMetal1.wav");
	//StepMetalic2 = app->audio->LoadFx("Assets/Sounds/Player/StepMetal2.wav");
	//DeathSound = app->audio->LoadFx("Assets/Sounds/Player/Fire2.wav");
	//Swing = app->audio->LoadFx("Assets/Sounds/Player/Swing.wav");
	//DamageFx = app->audio->LoadFx("Assets/Sounds/Enemy/Dead2.wav");
	//tpFX = app->audio->LoadFx("Assets/Sounds/TeleportSound.wav");

	//Load FX
	doorFX = app->audio->LoadFx("Assets/Soundtrack/Fx/Gameplay/Door Fx.wav");
	startBattleFX = app->audio->LoadFx("Assets/Soundtrack/Fx/Gameplay/Start battle FX.wav");
	MonolithFX = app->audio->LoadFx("Assets/Soundtrack/Fx/Gameplay/Monolith FX.wav");
	puzzleFx = app->audio->LoadFx("Assets/Soundtrack/Fx/Gameplay/Puzzle sound.wav");
	winSound = app->audio->LoadFx("Assets/Soundtrack/Fx/Gameplay/QuestFx.wav");
	BedFx = app->audio->LoadFx("Assets/Soundtrack/Fx/Player/Health potion.wav");
	select = app->audio->LoadFx("Assets/Soundtrack/Fx/Menu/Select.wav");

	////Textures
	//LFHH = app->tex->Load("Assets/Textures/FULL.png");
	//LFH = app->tex->Load("Assets/Textures/HALF.png");
	//LFL = app->tex->Load("Assets/Textures/LOW.png");
	//LFE = app->tex->Load("Assets/Textures/EMPTY.png");
	Dialogue = app->tex->Load("Assets/Textures/UI/globotexto_small.png");
	PressE = app->tex->Load("Assets/Textures/Objects/Interact_Botton.png");
	MenuQuest = app->tex->Load("Assets/Textures/UI/menu_quests_final.png");

	char lookupTable[] = { "abcdefghijklmnopqrstuvwxyz0123456789" };
	WF = app->font->Load("Assets/Fonts/FontWhiteDef.png", lookupTable, 1);
	YF = app->font->Load("Assets/Fonts/FontYellowDef.png", lookupTable, 1);
	


	////Animations Idle
	playerIdleL.PushBack({ 0, 0, 32, 32 });
	playerIdleL.PushBack({ 32, 0, 32, 32 });
	playerIdleL.loop = true;
	playerIdleL.speed = 0.04f;

	playerIdleR.PushBack({ 32*2, 0, 32, 32 });
	playerIdleR.PushBack({ 32*3, 0, 32, 32 });
	playerIdleR.loop = true;
	playerIdleR.speed = 0.04f;

	playerIdleDown.PushBack({ 0, 32, 32, 32 });
	playerIdleDown.PushBack({ 32, 32, 32, 32 });
	playerIdleDown.loop = true;
	playerIdleDown.speed = 0.04f;

	playerIdleUp.PushBack({ 32 * 2, 32, 32, 32 });
	playerIdleUp.PushBack({ 32 * 3, 32, 32, 32 });
	playerIdleUp.loop = true;
	playerIdleUp.speed = 0.04f;

	////Animations Run

	playerRunL.PushBack({ 32 * 0, 32 * 2, 32, 32 });
	playerRunL.PushBack({ 32 * 1, 32 * 2, 32, 32 });
	playerRunL.PushBack({ 32 * 2, 32 * 2, 32, 32 });
	playerRunL.PushBack({ 32 * 3, 32 * 2, 32, 32 });
	playerRunL.loop = true;
	playerRunL.speed = 0.07f;

	playerRunR.PushBack({ 32 * 0, 32 * 3, 32, 32 });
	playerRunR.PushBack({ 32 * 1, 32 * 3, 32, 32 });
	playerRunR.PushBack({ 32 * 2, 32 * 3, 32, 32 });
	playerRunR.PushBack({ 32 * 3, 32 * 3, 32, 32 });
	playerRunR.loop = true;
	playerRunR.speed = 0.07f;

	playerRunDown.PushBack({ 32 * 0, 32 * 4, 32, 32 });
	playerRunDown.PushBack({ 32 * 1, 32 * 4, 32, 32 });
	playerRunDown.PushBack({ 32 * 2, 32 * 4, 32, 32 });
	playerRunDown.PushBack({ 32 * 3, 32 * 4, 32, 32 });
	playerRunDown.loop = true;
	playerRunDown.speed = 0.07f;

	playerRunUp.PushBack({ 32 * 0, 32 * 5, 32, 32 });
	playerRunUp.PushBack({ 32 * 1, 32 * 5, 32, 32 });
	playerRunUp.PushBack({ 32 * 2, 32 * 5, 32, 32 });
	playerRunUp.PushBack({ 32 * 3, 32 * 5, 32, 32 });
	playerRunUp.loop = true;
	playerRunUp.speed = 0.07f;

	currentAnim = &playerIdleL;
	
	//playerAttackR.PushBack({ 0 * width,6 * height,width,height });
	//playerAttackL.PushBack({ 0 * width,7 * height,width,height });

	//
	//for (int i = 0; i < 6; i++) {
	//	playerRunR.PushBack({ i * width,2 * height,width,height });
	//}
	//playerRunR.loop = true;
	//playerRunR.speed = 0.3f;
	//
	//for (int i = 0; i < 6; i++) {
	//	playerRunL.PushBack({ i * width,3 * height,width,height });
	//}
	//playerRunL.loop = true;
	//playerRunL.speed = 0.3f;

	//for (int i = 0; i < 6; i++) {
	//	playerDie.PushBack({ i * width,8 * height,width,height });
	//}
	//playerDie.loop = false;
	//playerDie.speed = 0.3f;


	return true;
}



bool Player::Update(float dt)
{
	//Characters level up
	if (laurea.exp >= laurea.EXPneeded && laurea.lvl <= 20) {
		laurea.lvl++;
		laurea.EXPneeded += (laurea.EXPneeded * 0.1);
		laurea.exp = 0;
		laurea.hp += (laurea.HPG * laurea.HPC);
		laurea.chp += (laurea.HPG * laurea.HPC);
		laurea.mp += (laurea.MPG * laurea.MPC);
		laurea.cmp += (laurea.MPG * laurea.MPC);
		laurea.atk += (laurea.ATKG * laurea.ATKC);
		laurea.def += (laurea.DEFG * laurea.DEFC);
	}
	if (lapis.exp >= lapis.EXPneeded && lapis.lvl <= 20) {
		lapis.lvl++;
		lapis.EXPneeded += (lapis.EXPneeded * 0.1);
		lapis.exp = 0;
		lapis.hp += (lapis.HPG * lapis.HPC);
		lapis.chp += (lapis.HPG * lapis.HPC);
		lapis.mp += (lapis.MPG * lapis.MPC);
		lapis.cmp += (lapis.MPG * lapis.MPC);
		lapis.atk += (lapis.ATKG * lapis.ATKC);
		lapis.def += (lapis.DEFG * lapis.DEFC);
	}
	if (lucca.exp >= lucca.EXPneeded && lucca.lvl <= 20) {
		lucca.lvl++;
		lucca.EXPneeded += (lucca.EXPneeded * 0.1);
		lucca.exp = 0;
		lucca.hp += (lucca.HPG * lucca.HPC);
		lucca.chp += (lucca.HPG * lucca.HPC);
		lucca.mp += (lucca.MPG * lucca.MPC);
		lucca.cmp += (lucca.MPG * lucca.MPC);
		lucca.atk += (lucca.ATKG * lucca.ATKC);
		lucca.def += (lucca.DEFG * lucca.DEFC);
	}

	//Characters level up

	if (auxBool == true) {
		auxCounter++;
		if (auxCounter == 100) {
			auxCounter = 0;
			auxBool = false;
		}
	}

	// Level Up Fx
	if (lvlUpFx)
	{
		if(lvlupy == 500)
			app->audio->PlayFx(winSound);

		app->render->DrawText(200, lvlupy, { 34, 113, 179 }, "LEVEL UP!", 24);
		if (lvlupy > -100)
		{
			lvlupy -= 4;
		}
		else
		{
			lvlUpFx = false;
		}

	}
	else
	{
		lvlupy = 500;
	}

	debugKeys();

	if (playerState != State::ATTACKING)
		Move();

	////TotalTime = SDL_GetTicks();

	//if ((app->input->GetMouseButtonDown(1) && attackCD <= 0) || (attackFrames > 0 && playerState == State::ATTACKING)) {
	//	playerState = State::ATTACKING;
	//	app->audio->PlayFxWithVolume(Swing, 0, 25);
	//	Attack(attackFrames);
	//	
	//	attackFrames--;
	//}
	//else {
	//	playerState = State::IDLE;
	//	attackFrames = 10;
	//}
	//attackCD--;
	
	//Death
	if (alive != true) {
		Death(); 
	}
	
	//SDL_Rect rect = currentAnim->GetCurrentFrame();
	//currentAnim->Update();

	position.x = METERS_TO_PIXELS((pbody->body->GetTransform().p.x) - width / 2);
	position.y = METERS_TO_PIXELS((pbody->body->GetTransform().p.y) - height / 2);

	//Tp
	if (tp) {

		position.x = tp_pos[0];
		position.y = tp_pos[1];
		pbody->body->SetTransform({ PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y) }, 0);
		//printf("Instance = %d \n Pos.x = %d \n Pos.y = %d \n", future_instance, tp[0], tp[1]);
		tp = false;
	}

	//Tp when you lose a combat
	if (tpHouse) {
		laurea.chp = laurea.hp;
		laurea.cmp = laurea.mp;
		lapis.chp = lapis.hp;
		lapis.cmp = lapis.mp;
		lucca.chp = lucca.hp;
		lucca.cmp = lucca.mp;
		position.x = tpHouse_pos[0];
		position.y = tpHouse_pos[1];
		pbody->body->SetTransform({ PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y) }, 0);
		tpHouse = false;
	}


	app->render->camera.x = ((-1 * (position.x * app->win->GetScale() - app->render->camera.w / 2))-60);
	app->render->camera.y = -1 * (position.y * app->win->GetScale() - app->render->camera.h / 2);

	//Player draw

	currentAnim->Update();
	SDL_Rect rect = currentAnim->GetCurrentFrame();

	app->render->DrawTexture(texture, position.x + 16, position.y - 19, &rect);

	if (facing == DIRECTION::RIGHT) {
		//app->render->DrawTexture(texture, position.x + 16, position.y - 19, &rectRight);
	}
	if (facing == DIRECTION::LEFT) {
		/*app->render->DrawTexture(texture, position.x + 16, position.y - 19, &rectLeft);*/
	}
	if (facing == DIRECTION::DOWN) {
		//app->render->DrawTexture(texture, position.x + 16, position.y - 19, &rectDown);
	}
	if (facing == DIRECTION::UP) {
		//app->render->DrawTexture(texture, position.x + 16, position.y - 19, &rectUp);
	}
	

	if (buttonE && position.x < (copypos.x + 15) && position.x > (copypos.x - 15) && position.y < (copypos.y + 15) && position.y >(copypos.y - 15))
	{
		/*app->render->DrawTexture(PressE, position.x + 25, position.y - 35);*/

			//NPCs
		if (NPC_01 )
		{
			app->render->DrawTexture(PressE, 148 + 8, -280 - 15);
			
			if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
			{
				buttonE = false;
				NPC_02 = false;
				NPC_03 = false;
				NPC_04 = false;
				isBill = true;
				NPCname = 1;
				Quest1 = true;
			}
			if (app->input->controllers.A != 0)
			{
				buttonE = false;
				NPC_02 = false;
				NPC_03 = false;
				NPC_04 = false;
				isBill = true;
				NPCname = 1;
				Quest1 = true;
			}
		}
		if (NPC_02 )
		{
			app->render->DrawTexture(PressE, 224 + 8, -710 - 15);
			
			if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
			{
				buttonE = false;
				//isBill = true;
				NPC_01 = false;
				NPC_03 = false;
				NPC_04 = false;
				isDialogue = false;
				NPC2 = true;
				NPCname = 2;
				Quest2 = true;
			}
			if (app->input->controllers.A != 0)
			{
				buttonE = false;
				//isBill = true;
				NPC_01 = false;
				NPC_03 = false;
				NPC_04 = false;
				isDialogue = false;
				NPC2 = true;
				NPCname = 2;
				Quest2 = true;
			}

		}
		if (NPC_03 )
		{
			app->render->DrawTexture(PressE, 483 + 8, 545 - 15);
			
			if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
			{
				buttonE = false;
				//isBill = true;
				NPC_01 = false;
				NPC_02 = false;
				NPC_04 = false;
				NPC = true;
				NPCname = 3;
				
			}
			if (app->input->controllers.A != 0)
			{
				buttonE = false;
				//isBill = true;
				NPC_01 = false;
				NPC_02 = false;
				NPC_04 = false;
				NPC = true;
				NPCname = 3;
				
			}
		}
		if (NPC_04 )
		{
			app->render->DrawTexture(PressE, 1110 + 8, 790 - 15);

			if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
			{
				buttonE = false;
				//isBill = true;
				NPC_01 = false;
				NPC_03 = false;
				NPC_02 = false;
				isDialogue = true;
				NPCname = 4;

			}
			if (app->input->controllers.A != 0)
			{
				buttonE = false;
				//isBill = true;
				NPC_01 = false;
				NPC_03 = false;
				NPC_02 = false;
				isDialogue = true;
				NPCname = 4;
			}
		}

	}
	//if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	//{
	//	win = true;
	//}
	//WIN
	if (win)
	{
		app->render->DrawText(200, winy, { 34, 113, 179 },"MISSION COMPLETE", 24);
		if (winy > -100)
		{
			winy -= 4;
		}
		else
		{
			win = false;
		}
		
	}
	else
	{
		winy = 500;
	}

	//Lapis
	if (isDialogue && app->dialogueSystem->activeTree == nullptr)
	{
		dialogueID = app->dialogueSystem->LoadDialogue("vs_dialogues.xml", 0);
		lapisMission = true;
	}
	if (!isDialogue && !isBill && !NPC2 && !NPC)
	{
		app->dialogueSystem->CleanUp();
	}
	if (isDialogue )
	{
		app->render->DrawTexture(Dialogue, position.x - 270, position.y - 160);

		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN
			|| app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			isDialogue = false;
			NPCname = 0;
		}
		if (app->input->controllers.B != 0)
		{
			isDialogue = false;
		}
	}
	//Bill
	if (isBill && app->dialogueSystem->activeTree == nullptr)
	{

		if (lapisMission)
		{
			dialogueID = app->dialogueSystem->LoadDialogue("vs_dialogues.xml", 6);
			win = true;
			Quest1Completed = true;
			app->audio->PlayFx(winSound);
		}
		else
		{
			dialogueID = app->dialogueSystem->LoadDialogue("vs_dialogues.xml", 1);
		}
		
	}

	if (!isBill && !isDialogue && !NPC2 && !NPC)
	{
		app->dialogueSystem->CleanUp();
	}

	if (isBill)
	{
		app->render->DrawTexture(Dialogue, position.x - 270, position.y - 160);

		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN 
			|| app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			isBill = false;
			NPCname = 0;
		}
		if (app->input->controllers.B != 0)
		{
			isBill = false;
		}
	}

	//NPC2
	if (NPC2 && app->dialogueSystem->activeTree == nullptr)
	{

		if (app->inventory->necklace == 1)
		{
			dialogueID = app->dialogueSystem->LoadDialogue("vs_dialogues.xml", 5);
			win = true;
			Quest2Completed = true;
			app->audio->PlayFx(winSound);
		}
		else
		{
			dialogueID = app->dialogueSystem->LoadDialogue("vs_dialogues.xml", 2);
		}
		
	}

	if (!isBill && !isDialogue && !NPC2 && !NPC)
	{
		app->dialogueSystem->CleanUp();
	}

	if (NPC2)
	{
		app->render->DrawTexture(Dialogue, position.x - 270, position.y - 160);

		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN 
			|| app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			NPC2 = false;
			NPCname = 0;
		}
		if (app->input->controllers.B != 0)
		{
			NPC2 = false;
		}
	}

	//NPC
	if (NPC && app->dialogueSystem->activeTree == nullptr)
	{
		if (app->inventory->manuscript==1)
		{
			dialogueID = app->dialogueSystem->LoadDialogue("vs_dialogues.xml", 4);
			win = true;
			app->audio->PlayFx(winSound);
		}
		else
		{
			dialogueID = app->dialogueSystem->LoadDialogue("vs_dialogues.xml", 3);
		}
		
	
	}
	
	if (!isBill && !isDialogue && !NPC2 && !NPC)
	{
		app->dialogueSystem->CleanUp();
	}

	if (NPC)
	{
		app->render->DrawTexture(Dialogue, position.x - 270, position.y - 160);

		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN 
			|| app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			NPC = false;
			NPCname = 0;
		}
		if (app->input->controllers.B != 0)
		{
			NPC = false;
		}
	}

	//MenuQuest
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		Qmenu = false;
	}
	if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN && !app->combat->InCombat && !isDialogue)
	{
		Qmenu = !Qmenu;
		app->inventory->inventoryOn = false;
		app->scene->isPaused = false;
		app->audio->PlayFxWithVolume(select, 0, app->audio->fxvolume);
	}
	if (Qmenu)
	{
		!app->inventory->inventoryOn;
		//app->render->DrawTexture(MenuQuest, 237, 152);
		app->render->DrawTexture(MenuQuest, position.x - 290, position.y - 180);
		app->render->DrawText(170, 160, { 0, 0, 0 }, "Main mission", 16);
		app->render->DrawText(300, 160, { 0, 0, 0 }, "Defeat bosses", 16);

		if (!Quest1)
		{
			app->render->DrawText(200, 220, { 0, 0, 0 }, "?", 18);
			app->render->DrawText(380, 220, { 0, 0, 0 }, "?", 18);
		}

		if (!Quest2)
		{
			app->render->DrawText(200, 280, { 0, 0, 0 }, "?", 18);
			app->render->DrawText(380, 280, { 0, 0, 0 }, "?", 18);
		}

		if (Quest1)
		{
			app->render->DrawText(170, 220, { 0, 0, 0 }, "Bill", 16);
			app->render->DrawText(300, 220, { 0, 0, 0 }, "Return the pendant to Bill", 16);
		}

		if (Quest2)
		{
			app->render->DrawText(170, 280, { 0, 0, 0 }, "Timmy", 16);
			app->render->DrawText(300, 280, { 0, 0, 0 }, "Find Timmy's Necklace", 16);
		}

		if (Quest1Completed)
		{
			app->render->DrawText(170, 220, { 0, 255, 0 }, "Bill", 16);
			app->render->DrawText(300, 220, { 0, 255, 0 }, "Return the pendant to Bill", 16);
		}
		if (Quest2Completed)
		{
			app->render->DrawText(170, 280, { 0, 255, 0 }, "Timmy", 16);
			app->render->DrawText(300, 280, { 0, 255, 0 }, "Find Timmy's Necklace", 16);
		}

	}



	return true;
}

bool Player::CleanUp()
{
	return true;
}

void Player::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	case ColliderType::ENEMYSLIME:
		LOG("Collision Slime");
		PresetChance = rand() % 100 + 1;
		PresetVariation = rand() % 100 + 1;
		if (PresetChance >= 1 && PresetChance <= 20) app->combat->Preset = 1;
		if (PresetChance >= 21 && PresetChance <= 40) app->combat->Preset = 2;
		if (PresetChance >= 41 && PresetChance <= 60) app->combat->Preset = 3;
		if (PresetChance >= 61 && PresetChance <= 75) app->combat->Preset = 4;
		if (PresetChance >= 76 && PresetChance <= 90) app->combat->Preset = 5;
		if (PresetChance >= 91 && PresetChance <= 100) app->combat->Preset = 6;
		if (PresetVariation <= 85) app->combat->EnemyVariation = 0;
		if (PresetVariation >= 86 && PresetVariation <= 89) app->combat->EnemyVariation = 1;
		if (PresetVariation >= 90 && PresetVariation <= 93) app->combat->EnemyVariation = 2;
		if (PresetVariation >= 94 && PresetVariation <= 97) app->combat->EnemyVariation = 3;
		if (PresetVariation >= 98 && PresetVariation <= 100) app->combat->EnemyVariation = 4;
		app->audio->PlayFxWithVolume(startBattleFX, 0, app->audio->fxvolume);
		app->audio->PlayMusic("Assets/Soundtrack/Music/Tension.ogg");
		app->combat->StartCombat();
		break;
	case ColliderType::ENEMYLILY:
		LOG("Collision Lily");
		PresetChance = rand() % 100 + 1;
		PresetVariation = rand() % 100 + 1;
		if (PresetChance >= 1 && PresetChance <= 20) app->combat->Preset = 7;
		if (PresetChance >= 21 && PresetChance <= 40) app->combat->Preset = 8;
		if (PresetChance >= 41 && PresetChance <= 60) app->combat->Preset = 9;
		if (PresetChance >= 61 && PresetChance <= 75) app->combat->Preset = 10;
		if (PresetChance >= 76 && PresetChance <= 90) app->combat->Preset = 11;
		if (PresetChance >= 91 && PresetChance <= 100) app->combat->Preset = 12;
		if (PresetVariation <= 85) app->combat->EnemyVariation = 0;
		if (PresetVariation >= 86 && PresetVariation <= 89) app->combat->EnemyVariation = 1;
		if (PresetVariation >= 90 && PresetVariation <= 93) app->combat->EnemyVariation = 2;
		if (PresetVariation >= 94 && PresetVariation <= 97) app->combat->EnemyVariation = 3;
		if (PresetVariation >= 98 && PresetVariation <= 100) app->combat->EnemyVariation = 4;
		app->audio->PlayFxWithVolume(startBattleFX, 0, app->audio->fxvolume);
		app->audio->PlayMusic("Assets/Soundtrack/Music/Tension.ogg");
		app->combat->StartCombat();
		break;
	case ColliderType::ENEMYNAIADON:
		LOG("Collision Naiadon");
		PresetVariation = rand() % 100 + 1;
		app->combat->Preset = 13;
		if (PresetVariation <= 85) app->combat->EnemyVariation = 0;
		if (PresetVariation >= 86 && PresetVariation <= 89) app->combat->EnemyVariation = 1;
		if (PresetVariation >= 90 && PresetVariation <= 93) app->combat->EnemyVariation = 2;
		if (PresetVariation >= 94 && PresetVariation <= 97) app->combat->EnemyVariation = 3;
		if (PresetVariation >= 98 && PresetVariation <= 100) app->combat->EnemyVariation = 4;
		app->scene->BWC = true;
		app->audio->PlayFxWithVolume(startBattleFX, 0, app->audio->fxvolume);
		app->audio->PlayMusic("Assets/Soundtrack/Music/Tension.ogg");
		app->combat->StartCombat();
		break;
	case ColliderType::ENEMYDRIADON:
		LOG("Collision Dryadon");
		PresetVariation = rand() % 100 + 1;
		app->combat->Preset = 14;
		if (PresetVariation <= 85) app->combat->EnemyVariation = 0;
		if (PresetVariation >= 86 && PresetVariation <= 89) app->combat->EnemyVariation = 1;
		if (PresetVariation >= 90 && PresetVariation <= 93) app->combat->EnemyVariation = 2;
		if (PresetVariation >= 94 && PresetVariation <= 97) app->combat->EnemyVariation = 3;
		if (PresetVariation >= 98 && PresetVariation <= 100) app->combat->EnemyVariation = 4;
		app->scene->BFC = true;
		app->audio->PlayFxWithVolume(startBattleFX, 0, app->audio->fxvolume);
		app->audio->PlayMusic("Assets/Soundtrack/Music/Tension.ogg");
		app->combat->StartCombat();
		break;
	case ColliderType::ENEMYGASHA:
		LOG("Collision Gasha");
		PresetVariation = rand() % 100 + 1;
		app->combat->Preset = 15;
		if (PresetVariation <= 85) app->combat->EnemyVariation = 0;
		if (PresetVariation >= 86 && PresetVariation <= 89) app->combat->EnemyVariation = 1;
		if (PresetVariation >= 90 && PresetVariation <= 93) app->combat->EnemyVariation = 2;
		if (PresetVariation >= 94 && PresetVariation <= 97) app->combat->EnemyVariation = 3;
		if (PresetVariation >= 98 && PresetVariation <= 100) app->combat->EnemyVariation = 4;
		app->scene->BCC = true;
		app->audio->PlayFxWithVolume(startBattleFX, 0, app->audio->fxvolume);
		app->audio->PlayMusic("Assets/Soundtrack/Music/Tension.ogg");
		app->combat->StartCombat();
		break;
	case ColliderType::TAVERN:
		LOG("Teleport_Tavern");
		if (auxBool == false) {
			app->audio->PlayFxWithVolume(doorFX, 0, app->audio->fxvolume);
			app->scene->fade = true;
			Teleport_Point(1, tp1);
			auxBool = true;
		}
		break;
	case ColliderType::TOWNTAVERN:
		LOG("Collision Town");
		if (auxBool == false) {
			app->audio->PlayFxWithVolume(doorFX, 0, app->audio->fxvolume);
			app->scene->fade = true;
			Teleport_Point(0, tp2);
			auxBool = true;
		}
		break;
	case ColliderType::BLACKSMITH:
		LOG("Collision Blacksmith");
		if (auxBool == false) {
			app->audio->PlayFxWithVolume(doorFX, 0, app->audio->fxvolume);
			app->scene->fade = true;
			Teleport_Point(2, tp3);
			auxBool = true;
		}
		break;
	case ColliderType::TOWNBLACKSMITH:
		LOG("Collision Town");
		if (auxBool == false) {
			app->audio->PlayFxWithVolume(doorFX, 0, app->audio->fxvolume);
			app->scene->fade = true;
			Teleport_Point(0, tp4);
			auxBool = true;
		}
		break;
	case ColliderType::HOUSE:
		LOG("Collision House");
		if (auxBool == false) {
			app->audio->PlayFxWithVolume(doorFX, 0, app->audio->fxvolume);
			app->scene->fade = true;
			Teleport_Point(3, tp5);
			auxBool = true;
		}
		break;
	case ColliderType::TOWNHOUSE:
		LOG("Collision Town");
		if (auxBool == false) {
			app->audio->PlayFxWithVolume(doorFX, 0, app->audio->fxvolume);
			app->scene->fade = true;
			Teleport_Point(0, tp6);
			auxBool = true;
		}
		break;
	case ColliderType::LAKEDUNGEON:
		LOG("Collision Lake Dungeon");
		if (auxBool == false) {
			app->audio->PlayFxWithVolume(MonolithFX, 0, app->audio->fxvolume);
			app->scene->fade = true;
			Teleport_Point(4, tp7);
			auxBool = true;
		}
		break;
	case ColliderType::TOWNLAKEDUNGEON:
		LOG("Collision Town");
		if (auxBool == false) {
			app->audio->PlayFxWithVolume(MonolithFX, 0, app->audio->fxvolume);
			app->scene->fade = true;
			Teleport_Point(7, tp8);
			auxBool = true;
		}
		break;
	case ColliderType::FORESTDUNGEON:
		LOG("Collision Forest Dungeon");
		if (auxBool == false) {
			app->audio->PlayFxWithVolume(MonolithFX, 0, app->audio->fxvolume);
			app->scene->fade = true;
			Teleport_Point(5, tp9);
			auxBool = true;
		}
		break;
	case ColliderType::TOWNFORESTDUNGEON:
		LOG("Collision Town");
		if (auxBool == false) {
			app->audio->PlayFxWithVolume(MonolithFX, 0, app->audio->fxvolume);
			app->scene->fade = true;
			Teleport_Point(7, tp10);
			auxBool = true;
		}
		break;
	case ColliderType::CAVEDUNGEON:
		LOG("Collision Cave Dungeon");
		if (auxBool == false) {
			app->audio->PlayFxWithVolume(MonolithFX, 0, app->audio->fxvolume);
			app->scene->fade = true;
			Teleport_Point(6, tp11);
			auxBool = true;
		}
		break;
	case ColliderType::TOWNCAVEDUNGEON:
		LOG("Collision Town");
		if (auxBool == false) {
			app->audio->PlayFxWithVolume(MonolithFX, 0, app->audio->fxvolume);
			app->scene->fade = true;
			Teleport_Point(7, tp12);
			auxBool = true;
		}
		break;
	case ColliderType::MONOLITH:
		LOG("Collision Monolith");
		if (auxBool == false) {
			app->scene->fade = true;
			Teleport_Point(7, tp13);
			auxBool = true;
		}
		break;
	case ColliderType::TOWNMONOLITH:
		LOG("Collision Town");
		if (auxBool == false) {
			app->scene->fade = true;
			Teleport_Point(0, tp14);
			auxBool = true;
		}
		break;
	case ColliderType::PILAR1:
		LOG("Collision Pilar 1");
		app->scene->P1Active = true;

		if (pilar1Fx == false) {
			app->audio->PlayFxWithVolume(puzzleFx, 0, app->audio->fxvolume);
			pilar1Fx = true;
		}
		break;
	case ColliderType::PILAR2:
		LOG("Collision Pilar 2");
		app->scene->P2Active = true;

		if (pilar2Fx == false) {
			app->audio->PlayFxWithVolume(puzzleFx, 0, app->audio->fxvolume);
			pilar2Fx = true;
		}
		break;
	case ColliderType::PILAR3:
		LOG("Collision Pilar 3");
		app->scene->P3Active = true;

		if (pilar3Fx == false) {
			app->audio->PlayFxWithVolume(puzzleFx, 0, app->audio->fxvolume);
			pilar3Fx = true;
		}
		break;
	case ColliderType::BED:
		LOG("Collision Bed");
		laurea.chp = laurea.hp;
		laurea.cmp = laurea.mp;
		lapis.chp = lapis.hp;
		lapis.cmp = lapis.mp;
		lucca.chp = lucca.hp;
		lucca.cmp = lucca.mp;
		app->audio->PlayFxWithVolume(BedFx, 0, app->audio->fxvolume);
		break;
	case ColliderType::NPC1:
		//Collision in tabern
		//Draw E
		copypos = position;
		buttonE = true;
		NPC_02 = false;
		NPC_03 = false;
		NPC_04 = false;
		NPC_01 = true;
		break;
	case ColliderType::NPC2:
		//Collision in blacksmith
		copypos = position;
		buttonE = true;
		NPC_01 = false;
		NPC_03 = false;
		NPC_04 = false;
		NPC_02 = true;
		break;
	case ColliderType::NPC3:
		//Collision in town
		copypos = position;
		buttonE = true;
		NPC_01 = false;
		NPC_02 = false;
		NPC_04 = false;
		NPC_03 = true;
		break;
	case ColliderType::LAPIS:
		//Collision in town
		copypos = position;
		buttonE = true;
		NPC_01 = false;
		NPC_03 = false;
		NPC_02 = false;
		NPC_04 = true;
		break;
	}
		
}


bool Player::IsAlive() {
	return alive; 
}


bool Player::LoadState(pugi::xml_node& data) {

	facing = (DIRECTION)data.child("player_stats").attribute("facing").as_int();
	tp_savegame[0] = data.child("player_stats").attribute("position_x").as_int();
	tp_savegame[1] = data.child("player_stats").attribute("position_y").as_int();
	future_instance = data.child("player_stats").attribute("instance").as_int();
	
	//Teleport player last position save
	if (auxBool == false) {
		app->scene->fade = true;
		Teleport_Point(future_instance, tp_savegame);
		auxBool = true;
	}

	laurea.chp = data.child("laurea_stats").append_attribute("laurea_CHP").as_float();
	laurea.cmp = data.child("laurea_stats").append_attribute("laurea_CMP").as_float();
	laurea.lvl = data.child("laurea_stats").append_attribute("laurea_LVL").as_float();
	laurea.exp = data.child("laurea_stats").append_attribute("laurea_EXP").as_float();

	lapis.chp = data.child("lapis_stats").append_attribute("lapis_CHP").as_float();
	lapis.cmp = data.child("lapis_stats").append_attribute("lapis_CMP").as_float();
	lapis.lvl = data.child("lapis_stats").append_attribute("lapis_LVL").as_float();
	lapis.exp = data.child("lapis_stats").append_attribute("lapis_EXP").as_float();

	lucca.chp = data.child("lucca_stats").append_attribute("lucca_CHP").as_float();
	lucca.cmp = data.child("lucca_stats").append_attribute("lucca_CMP").as_float();
	lucca.lvl = data.child("lucca_stats").append_attribute("lucca_LVL").as_float();
	lucca.exp = data.child("lucca_stats").append_attribute("lucca_EXP").as_float();

	//playerState = (State)data.child("player_stats").attribute("state").as_int();
	return true;
}

bool Player::SaveState(pugi::xml_node& data) {

	pugi::xml_node player_stats = data.append_child("player_stats");
	data.child("player_stats").append_attribute("facing") = (int)facing;
	data.child("player_stats").append_attribute("position_x") = position.x +32;
	data.child("player_stats").append_attribute("position_y") = position.y +8;
	data.child("player_stats").append_attribute("instance") = future_instance;

	pugi::xml_node laurea_stats = data.append_child("laurea_stats");
	data.child("laurea_stats").append_attribute("laurea_CHP") = laurea.chp;
	data.child("laurea_stats").append_attribute("laurea_CMP") = laurea.cmp;
	data.child("laurea_stats").append_attribute("laurea_LVL") = laurea.lvl;
	data.child("laurea_stats").append_attribute("laurea_EXP") = laurea.exp;

	pugi::xml_node lapis_stats = data.append_child("lapis_stats");
	data.child("lapis_stats").append_attribute("lapis_CHP") = lapis.chp;
	data.child("lapis_stats").append_attribute("lapis_CMP") = lapis.cmp;
	data.child("lapis_stats").append_attribute("lapis_LVL") = lapis.lvl;
	data.child("lapis_stats").append_attribute("lapis_EXP") = lapis.exp;

	pugi::xml_node lucca_stats = data.append_child("lucca_stats");
	data.child("lucca_stats").append_attribute("lucca_CHP") = lucca.chp;
	data.child("lucca_stats").append_attribute("lucca_CMP") = lucca.cmp;
	data.child("lucca_stats").append_attribute("lucca_LVL") = lucca.lvl;
	data.child("lucca_stats").append_attribute("lucca_EXP") = lucca.exp;

	//data.child("player_stats").append_attribute("state") =(int)playerState;
	return true; 
}

void Player::InitializePlayers()
{
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	width = parameters.attribute("width").as_int();
	height = parameters.attribute("height").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	laurea = { 350,350,50,50,100,125,10,2,1,0 };
	lapis = { 250,250,100,100,125,90,12,4,1,0 };
	lucca = { 200,200,115,115,150,60,26,3,1,0 };
}

void Player::LevelToMax() {
	laurea.exp = laurea.EXPneeded;
	lapis.exp = lapis.EXPneeded;
	lucca.exp = lucca.EXPneeded;
}

void Player::Move() {

	float speed = 5;

	vel = b2Vec2(0, 0);

	if ((app->input->GetKey(SDL_SCANCODE_W) == KEY_IDLE && app->input->GetKey(SDL_SCANCODE_S) == KEY_IDLE && app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE && app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE && app->scene->CanPlayerMove == true && !Qmenu && !isDialogue) && app->combat->InCombat == false) {
		vel = b2Vec2(0, 0);
	}
	if (((app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT )&& app->scene->CanPlayerMove == true && !Qmenu && !isDialogue && !isBill && !NPC && !NPC2 && !app->scene->isPaused) && app->combat->InCombat == false) {
		vel = b2Vec2(-speed, 0);
		facing = DIRECTION::LEFT;
		currentAnim = &playerRunL;
	}
	if (((app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) && app->scene->CanPlayerMove == true && !Qmenu && !isDialogue && !isBill && !NPC && !NPC2 && !app->scene->isPaused) && app->combat->InCombat == false) {
		vel = b2Vec2(speed, 0);
		facing = DIRECTION::RIGHT;
		currentAnim = &playerRunR;
	}
	if (((app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) && app->scene->CanPlayerMove == true && !Qmenu && !isDialogue && !isBill && !NPC && !NPC2 && !app->scene->isPaused) && app->combat->InCombat == false) {
		vel = b2Vec2(0, -speed);
		facing = DIRECTION::UP;
		currentAnim = &playerRunUp;
	}
	if (((app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_DOWN ) == KEY_REPEAT)&& app->scene->CanPlayerMove == true && !Qmenu && !isDialogue && !isBill && !NPC && !NPC2 && !app->scene->isPaused) && app->combat->InCombat == false) {
		vel = b2Vec2(0, speed);
		facing = DIRECTION::DOWN;
		currentAnim = &playerRunDown;
	}

	//Debug to max level
	if (app->input->GetKey(SDL_SCANCODE_Y) == KEY_DOWN) {
		LevelToMax();
	}

	if (app->input->controllers.j1_x > 0 && app->scene->CanPlayerMove == true && !isDialogue && !app->scene->isPaused && !app->inventory->inventoryOn)
	{
		vel = b2Vec2(speed, 0);
		facing = DIRECTION::RIGHT;
		currentAnim = &playerRunR;
	}
	if (app->input->controllers.j1_x < 0 && app->scene->CanPlayerMove == true && !isDialogue && !app->scene->isPaused && !app->inventory->inventoryOn)
	{
		vel = b2Vec2(-speed, 0);
		facing = DIRECTION::LEFT;
		currentAnim = &playerRunL;
	}
	if (app->input->controllers.j1_y > 0 && app->scene->CanPlayerMove == true && !isDialogue && !app->scene->isPaused && !app->inventory->inventoryOn)
	{
		vel = b2Vec2(0, speed);
		facing = DIRECTION::DOWN;
		currentAnim = &playerRunDown;
	}
	if (app->input->controllers.j1_y < 0 && app->scene->CanPlayerMove == true && !isDialogue && !app->scene->isPaused && !app->inventory->inventoryOn)
	{
		vel = b2Vec2(0, -speed);
		facing = DIRECTION::UP;
		currentAnim = &playerRunUp;
	}
	

	//Fx
	//if (vel.x != 0 && vel.y == 0) {
	//	aux++;
	//	if (aux == 0) {
	//		app->audio->PlayFxWithVolume(Step1, 0, 25);
	//	}
	//	if (aux == 10) {
	//		app->audio->PlayFxWithVolume(Step2, 0, 25);
	//		aux = -10;
	//	}
	//}


	//Return to idle 
	if (app->input->GetKey(SDL_SCANCODE_W) == KeyState::KEY_IDLE
		&& app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_IDLE
		&& app->input->GetKey(SDL_SCANCODE_S) == KeyState::KEY_IDLE
		&& app->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_IDLE
		&& app->input->GetKey(SDL_SCANCODE_LEFT) == KeyState::KEY_IDLE
		&& app->input->GetKey(SDL_SCANCODE_RIGHT) == KeyState::KEY_IDLE
		&& app->input->GetKey(SDL_SCANCODE_UP) == KeyState::KEY_IDLE
		&& app->input->GetKey(SDL_SCANCODE_DOWN) == KeyState::KEY_IDLE){

		if (facing == DIRECTION::RIGHT && vel.x == 0) {
			currentAnim = &playerIdleR;
		}
		if (facing == DIRECTION::LEFT && vel.x == 0) {
			currentAnim = &playerIdleL;
		}
		if (facing == DIRECTION::UP && vel.x == 0) {
			currentAnim = &playerIdleUp;
		}
		if (facing == DIRECTION::DOWN && vel.x == 0) {
			currentAnim = &playerIdleL;
		}

	}
	//Set the velocity of the pbody of the player
	pbody->body->SetLinearVelocity(vel);
}

//void Player::Attack(int frames) {
//
//	
//
//	if (frames > 1) {
//		
//		if (facing == DIRECTION::RIGHT) {
//			attackHitbox->body->SetTransform({ PIXEL_TO_METERS((position.x+50)),  PIXEL_TO_METERS((position.y+8)) }, 0);
//			//app->render->DrawRectangle({ position.x+21, position.y, 50, 20 }, 255, 0, 0, 200);
//			//currentAnim = &playerAttackR;
//		}
//		if (facing == DIRECTION::LEFT) {
//			attackHitbox->body->SetTransform({ PIXEL_TO_METERS((position.x +15)), PIXEL_TO_METERS((position.y+8)) }, 0);
//			//app->render->DrawRectangle({ position.x-39, position.y, 50, 20 }, 255, 0, 0, 200);
//			//currentAnim = &playerAttackL;
//		}
//	}
//	else {
//
//		attackHitbox->body->SetTransform({0, 0}, 0);
//		attackCD = 40;
//	}
//	
//	
//
//	
//}

void Player::debugKeys() {

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {
		if (invincible == false) {
			invincible = true;
		}
		else if (invincible == true) {
			invincible = false;
		}
		if (pbody->body->GetType() == b2_dynamicBody) {
			pbody->body->SetType(b2_kinematicBody);
		}
		else if (pbody->body->GetType() == b2_kinematicBody) {
			pbody->body->SetType(b2_dynamicBody);
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
	{
		limitFPS = !limitFPS;
	}


	if (app->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
	{
		PresetChance = rand() % 100 + 1;
		PresetVariation = rand() % 100 + 1;
		if (PresetChance >= 1 && PresetChance <= 20) app->combat->Preset = 1;
		if (PresetChance >= 21 && PresetChance <= 40) app->combat->Preset = 2;
		if (PresetChance >= 41 && PresetChance <= 60) app->combat->Preset = 3;
		if (PresetChance >= 61 && PresetChance <= 75) app->combat->Preset = 4;
		if (PresetChance >= 76 && PresetChance <= 90) app->combat->Preset = 5;
		if (PresetChance >= 91 && PresetChance <= 100) app->combat->Preset = 6;
		if (PresetVariation <= 85) app->combat->EnemyVariation = 0;
		if (PresetVariation >= 86 && PresetVariation <= 89) app->combat->EnemyVariation = 1;
		if (PresetVariation >= 90 && PresetVariation <= 93) app->combat->EnemyVariation = 2;
		if (PresetVariation >= 94 && PresetVariation <= 97) app->combat->EnemyVariation = 3;
		if (PresetVariation >= 98 && PresetVariation <= 100) app->combat->EnemyVariation = 4;
		app->audio->PlayFxWithVolume(startBattleFX, 0, app->audio->fxvolume);
		app->audio->PlayMusic("Assets/Soundtrack/Music/Tension.ogg"); 
		app->combat->StartCombat();
	}

	//Boss Lake
	if (app->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN)
	{
		LOG("Boss Naiadon");
		PresetVariation = rand() % 100 + 1;
		app->combat->Preset = 13;
		if (PresetVariation <= 85) app->combat->EnemyVariation = 0;
		if (PresetVariation >= 86 && PresetVariation <= 89) app->combat->EnemyVariation = 1;
		if (PresetVariation >= 90 && PresetVariation <= 93) app->combat->EnemyVariation = 2;
		if (PresetVariation >= 94 && PresetVariation <= 97) app->combat->EnemyVariation = 3;
		if (PresetVariation >= 98 && PresetVariation <= 100) app->combat->EnemyVariation = 4;

		app->audio->PlayFxWithVolume(startBattleFX, 0, app->audio->fxvolume);
		app->audio->PlayMusic("Assets/Soundtrack/Music/Tension.ogg");
		app->combat->StartCombat();
	}

	//Boss Forest
	if (app->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
	{
		LOG("Boss Dryadon");
		PresetVariation = rand() % 100 + 1;
		app->combat->Preset = 14;
		if (PresetVariation <= 85) app->combat->EnemyVariation = 0;
		if (PresetVariation >= 86 && PresetVariation <= 89) app->combat->EnemyVariation = 1;
		if (PresetVariation >= 90 && PresetVariation <= 93) app->combat->EnemyVariation = 2;
		if (PresetVariation >= 94 && PresetVariation <= 97) app->combat->EnemyVariation = 3;
		if (PresetVariation >= 98 && PresetVariation <= 100) app->combat->EnemyVariation = 4;

		app->audio->PlayFxWithVolume(startBattleFX, 0, app->audio->fxvolume);
		app->audio->PlayMusic("Assets/Soundtrack/Music/Tension.ogg");
		app->combat->StartCombat();
	}

	// Boss Cave 
	if (app->input->GetKey(SDL_SCANCODE_SEMICOLON) == KEY_DOWN)
	{
		LOG("Boss Gasha");
		PresetVariation = rand() % 100 + 1;
		app->combat->Preset = 15;
		if (PresetVariation <= 85) app->combat->EnemyVariation = 0;
		if (PresetVariation >= 86 && PresetVariation <= 89) app->combat->EnemyVariation = 1;
		if (PresetVariation >= 90 && PresetVariation <= 93) app->combat->EnemyVariation = 2;
		if (PresetVariation >= 94 && PresetVariation <= 97) app->combat->EnemyVariation = 3;
		if (PresetVariation >= 98 && PresetVariation <= 100) app->combat->EnemyVariation = 4;

		app->audio->PlayFxWithVolume(startBattleFX, 0, app->audio->fxvolume);
		app->audio->PlayMusic("Assets/Soundtrack/Music/Tension.ogg");
		app->combat->StartCombat();
	}

	/*if (app->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN)
	{
		LOG("Collision Lily");
		app->scene->PresetChanceS = rand() % 100 + 1;
		app->scene->PresetVariationS = rand() % 100 + 1;
		if (app->scene->PresetChanceS >= 1 && app->scene->PresetChanceS <= 20) app->combat->Preset = 7;
		if (app->scene->PresetChanceS >= 21 && app->scene->PresetChanceS <= 40) app->combat->Preset = 8;
		if (app->scene->PresetChanceS >= 41 && app->scene->PresetChanceS <= 60) app->combat->Preset = 9;
		if (app->scene->PresetChanceS >= 61 && app->scene->PresetChanceS <= 75) app->combat->Preset = 10;
		if (app->scene->PresetChanceS >= 76 && app->scene->PresetChanceS <= 90) app->combat->Preset = 11;
		if (app->scene->PresetChanceS >= 91 && app->scene->PresetChanceS <= 100) app->combat->Preset = 12;
		if (app->scene->PresetVariationS <= 85) app->combat->EnemyVariation = 0;
		if (app->scene->PresetVariationS >= 86 && app->scene->PresetVariationS <= 89) app->combat->EnemyVariation = 1;
		if (app->scene->PresetVariationS >= 90 && app->scene->PresetVariationS <= 93) app->combat->EnemyVariation = 2;
		if (app->scene->PresetVariationS >= 94 && app->scene->PresetVariationS <= 97) app->combat->EnemyVariation = 3;
		if (app->scene->PresetVariationS >= 98 && app->scene->PresetVariationS <= 100) app->combat->EnemyVariation = 4;
		app->audio->PlayFxWithVolume(app->scene->player->startBattleFX, 0, app->audio->fxvolume);
		app->audio->PlayMusic("Assets/Soundtrack/Music/Tension.ogg");
		app->combat->StartCombat();
	}
	if (app->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN)
	{
		LOG("Collision Forest enemy");
		app->scene->PresetChanceS = rand() % 100 + 1;
		if (app->scene->PresetChanceS >= 1 && app->scene->PresetChanceS <= 25) app->combat->Preset = 16;
		if (app->scene->PresetChanceS >= 26 && app->scene->PresetChanceS <= 50) app->combat->Preset = 17;
		if (app->scene->PresetChanceS >= 51 && app->scene->PresetChanceS <= 60) app->combat->Preset = 18;
		if (app->scene->PresetChanceS >= 61 && app->scene->PresetChanceS <= 70) app->combat->Preset = 19;
		if (app->scene->PresetChanceS >= 71 && app->scene->PresetChanceS <= 80) app->combat->Preset = 20;
		if (app->scene->PresetChanceS >= 81 && app->scene->PresetChanceS <= 85) app->combat->Preset = 21;
		if (app->scene->PresetChanceS >= 86 && app->scene->PresetChanceS <= 90) app->combat->Preset = 22;
		if (app->scene->PresetChanceS >= 91 && app->scene->PresetChanceS <= 95) app->combat->Preset = 23;
		if (app->scene->PresetChanceS >= 96 && app->scene->PresetChanceS <= 100) app->combat->Preset = 24;
		app->audio->PlayFxWithVolume(app->scene->player->startBattleFX, 0, app->audio->fxvolume);
		app->audio->PlayMusic("Assets/Soundtrack/Music/Tension.ogg");
		app->combat->StartCombat();
	}
	if (app->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
	{
		LOG("Collision Cave enemy");
		app->scene->PresetChanceS = rand() % 100 + 1;
		if (app->scene->PresetChanceS >= 1 && app->scene->PresetChanceS <= 25) app->combat->Preset = 25;
		if (app->scene->PresetChanceS >= 26 && app->scene->PresetChanceS <= 50) app->combat->Preset = 26;
		if (app->scene->PresetChanceS >= 51 && app->scene->PresetChanceS <= 60) app->combat->Preset = 27;
		if (app->scene->PresetChanceS >= 61 && app->scene->PresetChanceS <= 70) app->combat->Preset = 28;
		if (app->scene->PresetChanceS >= 71 && app->scene->PresetChanceS <= 80) app->combat->Preset = 29;
		if (app->scene->PresetChanceS >= 81 && app->scene->PresetChanceS <= 85) app->combat->Preset = 30;
		if (app->scene->PresetChanceS >= 86 && app->scene->PresetChanceS <= 90) app->combat->Preset = 31;
		if (app->scene->PresetChanceS >= 91 && app->scene->PresetChanceS <= 95) app->combat->Preset = 32;
		if (app->scene->PresetChanceS >= 96 && app->scene->PresetChanceS <= 100) app->combat->Preset = 33;
		app->audio->PlayFxWithVolume(app->scene->player->startBattleFX, 0, app->audio->fxvolume);
		app->audio->PlayMusic("Assets/Soundtrack/Music/Tension.ogg");
		app->combat->StartCombat();
	}*/

	//Teleport Daigeons
	//Lake
	if (app->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN)
	{
		if (auxBool == false) {
			app->scene->fade = true;
			Teleport_Point(4, tp7);
			auxBool = true;
		}
	}
	//Forest
	if (app->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN)
	{
		if (auxBool == false) {
			app->scene->fade = true;
			Teleport_Point(5, tp9);
			auxBool = true;
		}
	}
	//Cave
	if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
	{
		if (auxBool == false) {
			app->scene->fade = true;
			Teleport_Point(6, tp11);
			auxBool = true;
		}
	}

}

void Player::Death() {
	pbody->body->SetActive(false);
	currentAnim = &playerDie;
	if (currentAnim->HasFinished()) {
		app->audio->PlayFxWithVolume(DeathSound, 0, 50);
		this->Disable();
		DeathAnimationFinished = true;
		app->deathmenu->active = true;
	}
}

//Take the instance and the coordinates of the teleportation point
void Player::Teleport_Point(int Instance ,int TELEPORT_POSITION[2]) {

	future_instance = Instance;
	tp_pos[0] = TELEPORT_POSITION[0];
	tp_pos[1] = TELEPORT_POSITION[1];

}


