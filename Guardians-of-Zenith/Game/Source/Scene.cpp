#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "EntityManager.h"
#include "Map.h"
#include "Physics.h"
#include "MainMenu.h"
#include "Intro.h"
#include "Enemy.h"
#include "Combat.h"
#include "Pathfinding.h"
#include "Tavern.h"
#include "Blacksmith.h"
#include "House.h"
#include "LakeDungeon.h"
#include "ForestDungeon.h"
#include "CaveDungeon.h"
#include "Monolith.h"
#include "Inventory.h"
#include "Player.h"
#include "Item.h"
#include "time.h"
#include "External/SDL/include/SDL_gamecontroller.h"

#include "Defs.h"
#include "Log.h"
#include<iostream>

Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;


	// iterate all objects in the scene
	// Check https://pugixml.org/docs/quickstart.html#access

	for (pugi::xml_node itemNode = config.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		Item* item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item->parameters = itemNode;
	}


	//L02: DONE 3: Instantiate the player using the entity manager
	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
	player->parameters = config.child("player");

	//Load music & FX
	villageMusic = app->audio->LoadFx("Assets/Soundtrack/Music/Guidance Island OST Version.ogg");
	change = app->audio->LoadFx("Assets/Soundtrack/Fx/Menu/ChangeSelection.wav");
	select = app->audio->LoadFx("Assets/Soundtrack/Fx/Menu/Select.wav");
	play = app->audio->LoadFx("Assets/Soundtrack/Fx/Menu/Play.wav");
	puzzleFX = app->audio->LoadFx("Assets/Soundtrack/Fx/Gameplay/puzzle sound.wav");

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	//img = app->tex->Load("Assets/Textures/test.png");

	// L03: DONE: Load map
	bool retLoad = app->map->Load();
	app->render->camera.x = 0;
	app->render->camera.y = 0;
	app->tavern->Load();
	app->blacksmith->Load();
	app->house->Load();
	app->monolith->Load();
	app->lakedungeon->Load();
	app->forestdungeon->Load();
	app->cavedungeon->Load();
	// L04: DONE 7: Set the window title with map/tileset info
	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		app->map->mapData.width,
		app->map->mapData.height,
		app->map->mapData.tileWidth,
		app->map->mapData.tileHeight,
		app->map->mapData.tilesets.Count());
		//app->FPS);

	app->win->SetTitle(title.GetString());

	// L12 Create walkability map
	if (retLoad) {
		int w, h;
		uchar* data = NULL;

		bool retWalkMap = app->map->CreateWalkabilityMap(w, h, &data);
		if (retWalkMap) app->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);

	}
	invTex = app->tex->Load("Assets/Textures/UI/UI_inventory_640x360.png");
	invArrowTex = app->tex->Load("Assets/Textures/UI_inventory_arrow_20rock_2[0]0.png");

	//Tavernero
	NPC1 = app->tex->Load("Assets/Entities/NPC/npc_1.png");
	//Herrara
	NPC2 = app->tex->Load("Assets/Entities/NPC/npc_2.png");
	//Chica Wasap
	NPC3 = app->tex->Load("Assets/Entities/NPC/npc_3.png");
	LAPIS = app->tex->Load("Assets/Entities/Characters/Lapis_Directions.png");
	//Puzzles
	Stone = app->tex->Load("Assets/Textures/Puzzles/Stone_Default.png");
	StoneDeep = app->tex->Load("Assets/Textures/Puzzles/Stone_Deep.png");
	Tp = app->tex->Load("Assets/Textures/Puzzles/MonolitoTP.png");
	Unpressed = app->tex->Load("Assets/Textures/Puzzles/Plate_Unpressed.png");
	Pressed = app->tex->Load("Assets/Textures/Puzzles/Plate_Pressed.png");
	Puzzle2Wall = app->tex->Load("Assets/Textures/Puzzles/Door_lake_dungeon.png");
	DefaultTile = app->tex->Load("Assets/Textures/Puzzles/CrackedGround_Default.png");
	BrokenTile = app->tex->Load("Assets/Textures/Puzzles/CrackedGround_Broken.png");
	Pilar1A = app->tex->Load("Assets/Textures/Puzzles/Tower_1_On.png");
	Pilar1N = app->tex->Load("Assets/Textures/Puzzles/Tower_1_Off.png");
	Pilar2A = app->tex->Load("Assets/Textures/Puzzles/Tower_2_On.png");
	Pilar2N = app->tex->Load("Assets/Textures/Puzzles/Tower_2_Off.png");
	Pilar3A = app->tex->Load("Assets/Textures/Puzzles/Tower_3_On.png");
	Pilar3N = app->tex->Load("Assets/Textures/Puzzles/Tower_3_Off.png");
	ForestDoor = app->tex->Load("Assets/Textures/Puzzles/Door_forest_dungeon.png");
	Blocking = app->tex->Load("Assets/Textures/Puzzles/Door_cave_dungeon.png");
	//Enemy
	Naiadon = app->tex->Load("Assets/Entities/enemies/Naiadon_Map.png");
	Driadon = app->tex->Load("Assets/Entities/enemies/Driadon_Map.png");
	Gasha = app->tex->Load("Assets/Entities/enemies/Gashadokuro_Map.png");
	//Maps
	TownPNG = app->tex->Load("Assets/Maps/Maps_Png/Town.png");
	WaterPNG = app->tex->Load("Assets/Maps/Maps_Png/Lake_dungeon.png");
	ForestPNG = app->tex->Load("Assets/Maps/Maps_Png/Forest_dungeon.png");

	//Lake enemies
	
	//Slime1 = app->physics->CreateRectangle(3400, -20, 32, 32, STATIC);
	//Slime1->ctype = ColliderType::ENEMYSLIME;
	//Slime2 = app->physics->CreateRectangle(3624, 44, 32, 32, STATIC);
	//Slime2->ctype = ColliderType::ENEMYSLIME;
	//Slime3 = app->physics->CreateRectangle(3624, -404, 32, 32, STATIC);
	//Slime3->ctype = ColliderType::ENEMYSLIME;
	//Slime4 = app->physics->CreateRectangle(3176, -372, 32, 32, STATIC);
	//Slime4->ctype = ColliderType::ENEMYSLIME;
	//Slime5 = app->physics->CreateRectangle(3208, -212, 32, 32, STATIC);
	//Slime5->ctype = ColliderType::ENEMYSLIME;
	//Slime6 = app->physics->CreateRectangle(2952, -820, 32, 32, STATIC);
	//Slime6->ctype = ColliderType::ENEMYSLIME;
	//Slime7 = app->physics->CreateRectangle(2920, -1140, 32, 32, STATIC);
	//Slime7->ctype = ColliderType::ENEMYSLIME;
	//Slime8 = app->physics->CreateRectangle(3240, -1140, 32, 32, STATIC);
	//Slime8->ctype = ColliderType::ENEMYSLIME;
	//Slime9 = app->physics->CreateRectangle(3848, -1172, 32, 32, STATIC);
	//Slime9->ctype = ColliderType::ENEMYSLIME;
	//Slime10 = app->physics->CreateRectangle(3688, -1012, 32, 32, STATIC);
	//Slime10->ctype = ColliderType::ENEMYSLIME;
	//Slime11 = app->physics->CreateRectangle(3816, -724, 32, 32, STATIC);
	//Slime11->ctype = ColliderType::ENEMYSLIME;

	//Lily3 = app->physics->CreateRectangle(3624, -244, 64, 43, STATIC);
	//Lily3->ctype = ColliderType::ENEMYLILY;
	//Lily2 = app->physics->CreateRectangle(3784, -852, 64, 43, STATIC);
	//Lily2->ctype = ColliderType::ENEMYLILY;
	//Lily3 = app->physics->CreateRectangle(3432, -852, 64, 43, STATIC);
	//Lily3->ctype = ColliderType::ENEMYLILY;
	//Lily4 = app->physics->CreateRectangle(3240, -1012, 64, 43, STATIC);
	//Lily4->ctype = ColliderType::ENEMYLILY;
	//Lily5 = app->physics->CreateRectangle(3016, -1012, 64, 43, STATIC);
	//Lily5->ctype = ColliderType::ENEMYLILY;

	Naiadon1 = app->physics->CreateRectangle(3459, -1814, 118, 125, STATIC);
	Naiadon1->ctype = ColliderType::ENEMYNAIADON;
	Driadon1 = app->physics->CreateRectangle(1265, 2373, 118, 125, STATIC);
	Driadon1->ctype = ColliderType::ENEMYDRIADON;
	Gasha1 = app->physics->CreateRectangle(-1338+10, -400+10, 118, 125+10, STATIC);
	Gasha1->ctype = ColliderType::ENEMYGASHA;

	//Lake puzzle

	MonoTP = app->physics->CreateRectangle(3537, 448, 32, 64, STATIC);
	MonoTP->ctype = ColliderType::TOWNLAKEDUNGEON;

	colliderPuzzle1 = app->physics->CreateRectangle(3380, 430, 32, 32, DYNAMIC);
	colliderPuzzle1->body->SetFixedRotation(true);
	colliderPuzzle1->body->SetLinearDamping(12);
	colliderPuzzle2 = app->physics->CreateRectangle(3190, 320, 32, 32, DYNAMIC);
	colliderPuzzle2->body->SetFixedRotation(true);
	colliderPuzzle2->body->SetLinearDamping(12);
	colliderPuzzle3 = app->physics->CreateRectangle(3600, 300, 32, 32, DYNAMIC);
	colliderPuzzle3->body->SetFixedRotation(true);
	colliderPuzzle3->body->SetLinearDamping(12);
	colliderPuzzle4 = app->physics->CreateRectangle(3776, -289, 32, 32, DYNAMIC);
	colliderPuzzle4->body->SetFixedRotation(true);
	colliderPuzzle4->body->SetLinearDamping(12);
	colliderPuzzle5 = app->physics->CreateRectangle(3936, -705, 32, 32, DYNAMIC);
	colliderPuzzle5->body->SetFixedRotation(true);
	colliderPuzzle5->body->SetLinearDamping(12);
	colliderPuzzle6 = app->physics->CreateRectangle(2880, -1281, 32, 32, DYNAMIC);
	colliderPuzzle6->body->SetFixedRotation(true);
	colliderPuzzle6->body->SetLinearDamping(12);

	colliderPuzzle1Block = app->physics->CreateRectangle(3440, 178, 32, 120, STATIC);
	colliderPuzzle1Block->body->SetFixedRotation(true);
	colliderPuzzle1Block->body->SetLinearDamping(50);

	colliderPuzzle2Block = app->physics->CreateRectangle(3455, -1385, 128, 128, STATIC);

	//Forest puzzle

	P1 = app->physics->CreateRectangle(752 - 16, 4336, 32, 52, STATIC);
	P1->ctype = ColliderType::PILAR1;

	P2 = app->physics->CreateRectangle(2288 - 16, 4112, 32, 52, STATIC);
	P2->ctype = ColliderType::PILAR2;

	P3 = app->physics->CreateRectangle(2288 - 16, 5488, 32, 52, STATIC);
	P3->ctype = ColliderType::PILAR3;

	Fdoor = app->physics->CreateRectangle(1280, 3440, 128, 128, STATIC);

	//Cave puzzle

	Block1 = app->physics->CreateRectangle(-1248, 1968, 32, 32, STATIC);
	Block2 = app->physics->CreateRectangle(-1408, 1392, 32, 32, STATIC);
	Block3 = app->physics->CreateRectangle(-1344, 176, 32, 32, STATIC);

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{

	// L03: DONE 3: Request App to Load / Save when pressing the keys F5 (save) / F6 (load)
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->SaveGameRequest();
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->LoadGameRequest();
	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {
		app->physics->debug = !app->physics->debug;
		app->entityManager->debug = !app->entityManager->debug; 
	}
	//Only debug purpose, as it meses up the character stats and may buff or nerf them
	if (app->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN) {
		if (app->scene->player->laurea.def != 100000) {
			C1D = app->scene->player->laurea.def;
			C2D = app->scene->player->lapis.def;
			C3D = app->scene->player->lucca.def;
			C1A = app->scene->player->laurea.atk;
			C2A = app->scene->player->lapis.atk;
			C3A = app->scene->player->lucca.atk;
			app->scene->player->laurea.def = 100000;
			app->scene->player->lapis.def = 100000;
			app->scene->player->lucca.def = 100000;
			app->scene->player->laurea.atk = 100000;
			app->scene->player->lapis.atk = 100000;
			app->scene->player->lucca.atk = 100000;
		}
		else if (app->scene->player->laurea.def == 100000) {
			app->scene->player->laurea.def = C1D;
			app->scene->player->lapis.def = C2D;
			app->scene->player->lucca.def = C3D;
			app->scene->player->laurea.atk = C1A;
			app->scene->player->lapis.atk = C2A;
			app->scene->player->lucca.atk = C3A;
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) {
		PF = !PF;
		app->map->DrawPathing = !app->map->DrawPathing;
	}
	if (pathActive == true && PF == true) {
		while (app->map->DestinationFound == false) {
			app->map->PropagateDijkstra();
		}
		pathActive = false;
	}

	//Enemy encounter
	if (((app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) || (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) || (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) || (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)) && app->Instance == 4) {
		WaterTimer++;
	}
	if (((app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) || (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) || (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) || (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)) && app->Instance == 5) {
		ForestTimer++;
	}
	if (((app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) || (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) || (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) || (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)) && app->Instance == 6) {
		CaveTimer++;
	}

	if (app->scene->player->invincible == false) {

		if (WaterTimer >= 800) {
			WaterTimer = 0;
			LOG("Collision Slime");
			PresetChanceS = rand() % 100 + 1;
			PresetVariationS = rand() % 100 + 1;
			Team = rand() % 2 + 1;
			if (Team == 1) {
				if (PresetChanceS >= 1 && PresetChanceS <= 20) app->combat->Preset = 1;
				if (PresetChanceS >= 21 && PresetChanceS <= 40) app->combat->Preset = 2;
				if (PresetChanceS >= 41 && PresetChanceS <= 60) app->combat->Preset = 3;
				if (PresetChanceS >= 61 && PresetChanceS <= 75) app->combat->Preset = 4;
				if (PresetChanceS >= 76 && PresetChanceS <= 90) app->combat->Preset = 5;
				if (PresetChanceS >= 91 && PresetChanceS <= 100) app->combat->Preset = 6;
				if (PresetVariationS <= 85) app->combat->EnemyVariation = 0;
				if (PresetVariationS >= 86 && PresetVariationS <= 89) app->combat->EnemyVariation = 1;
				if (PresetVariationS >= 90 && PresetVariationS <= 93) app->combat->EnemyVariation = 2;
				if (PresetVariationS >= 94 && PresetVariationS <= 97) app->combat->EnemyVariation = 3;
				if (PresetVariationS >= 98 && PresetVariationS <= 100) app->combat->EnemyVariation = 4;
				app->audio->PlayFxWithVolume(app->scene->player->startBattleFX, 0, app->audio->fxvolume);
				app->audio->PlayMusic("Assets/Soundtrack/Music/Tension.ogg");
				app->combat->StartCombat();
			}
			if (Team == 2) {
				LOG("Collision Lily");
				PresetChanceS = rand() % 100 + 1;
				PresetVariationS = rand() % 100 + 1;
				if (PresetChanceS >= 1 && PresetChanceS <= 20) app->combat->Preset = 7;
				if (PresetChanceS >= 21 && PresetChanceS <= 40) app->combat->Preset = 8;
				if (PresetChanceS >= 41 && PresetChanceS <= 60) app->combat->Preset = 9;
				if (PresetChanceS >= 61 && PresetChanceS <= 75) app->combat->Preset = 10;
				if (PresetChanceS >= 76 && PresetChanceS <= 90) app->combat->Preset = 11;
				if (PresetChanceS >= 91 && PresetChanceS <= 100) app->combat->Preset = 12;
				if (PresetVariationS <= 85) app->combat->EnemyVariation = 0;
				if (PresetVariationS >= 86 && PresetVariationS <= 89) app->combat->EnemyVariation = 1;
				if (PresetVariationS >= 90 && PresetVariationS <= 93) app->combat->EnemyVariation = 2;
				if (PresetVariationS >= 94 && PresetVariationS <= 97) app->combat->EnemyVariation = 3;
				if (PresetVariationS >= 98 && PresetVariationS <= 100) app->combat->EnemyVariation = 4;
				app->audio->PlayFxWithVolume(app->scene->player->startBattleFX, 0, app->audio->fxvolume);
				app->audio->PlayMusic("Assets/Soundtrack/Music/Tension.ogg");
				app->combat->StartCombat();
			}
		}

		if (ForestTimer >= 800) {
			ForestTimer = 0;
			LOG("Collision Forest enemy");
			PresetChanceS = rand() % 100 + 1;
			if (PresetChanceS >= 1 && PresetChanceS <= 25) app->combat->Preset = 16;
			if (PresetChanceS >= 26 && PresetChanceS <= 50) app->combat->Preset = 17;
			if (PresetChanceS >= 51 && PresetChanceS <= 60) app->combat->Preset = 18;
			if (PresetChanceS >= 61 && PresetChanceS <= 70) app->combat->Preset = 19;
			if (PresetChanceS >= 71 && PresetChanceS <= 80) app->combat->Preset = 20;
			if (PresetChanceS >= 81 && PresetChanceS <= 85) app->combat->Preset = 21;
			if (PresetChanceS >= 86 && PresetChanceS <= 90) app->combat->Preset = 22;
			if (PresetChanceS >= 91 && PresetChanceS <= 95) app->combat->Preset = 23;
			if (PresetChanceS >= 96 && PresetChanceS <= 100) app->combat->Preset = 24;
			app->audio->PlayFxWithVolume(app->scene->player->startBattleFX, 0, app->audio->fxvolume);
			app->audio->PlayMusic("Assets/Soundtrack/Music/Tension.ogg");
			app->combat->StartCombat();
		}

		if (CaveTimer >= 800) {
			CaveTimer = 0;
			LOG("Collision Cave enemy");
			PresetChanceS = rand() % 100 + 1;
			if (PresetChanceS >= 1 && PresetChanceS <= 25) app->combat->Preset = 25;
			if (PresetChanceS >= 26 && PresetChanceS <= 50) app->combat->Preset = 26;
			if (PresetChanceS >= 51 && PresetChanceS <= 60) app->combat->Preset = 27;
			if (PresetChanceS >= 61 && PresetChanceS <= 70) app->combat->Preset = 28;
			if (PresetChanceS >= 71 && PresetChanceS <= 80) app->combat->Preset = 29;
			if (PresetChanceS >= 81 && PresetChanceS <= 85) app->combat->Preset = 30;
			if (PresetChanceS >= 86 && PresetChanceS <= 90) app->combat->Preset = 31;
			if (PresetChanceS >= 91 && PresetChanceS <= 95) app->combat->Preset = 32;
			if (PresetChanceS >= 96 && PresetChanceS <= 100) app->combat->Preset = 33;
			app->audio->PlayFxWithVolume(app->scene->player->startBattleFX, 0, app->audio->fxvolume);
			app->audio->PlayMusic("Assets/Soundtrack/Music/Tension.ogg");
			app->combat->StartCombat();

		}

	}
	else if (app->scene->player->invincible == true) {

	}



	//Cave Puzzle
	//Puzzle1 Normal Draw

	if (TBroken1 == false) app->render->DrawTexture(DefaultTile, -1296, 1984);
	if (TBroken2 == false) app->render->DrawTexture(DefaultTile, -1264, 1984);
	if (TBroken3 == false) app->render->DrawTexture(DefaultTile, -1232, 1984);
	if (TBroken4 == false) app->render->DrawTexture(DefaultTile, -1296, 2016);
	if (TBroken5 == false) app->render->DrawTexture(DefaultTile, -1264, 2016);
	if (TBroken6 == false) app->render->DrawTexture(DefaultTile, -1232, 2016);
	if (TBroken7 == false) app->render->DrawTexture(DefaultTile, -1296, 2048);
	if (TBroken8 == false) app->render->DrawTexture(DefaultTile, -1264, 2048);
	if (TBroken9 == false) app->render->DrawTexture(DefaultTile, -1232, 2048);

	//Puzzle1 Detect if pressed

	if (app->scene->player->position.x >= -1332 && app->scene->player->position.x <= -1290 && app->scene->player->position.y >= 1973 && app->scene->player->position.y <= 2008) TPressed1 = true;
	if (app->scene->player->position.x >= -1300 && app->scene->player->position.x <= -1258 && app->scene->player->position.y >= 1973 && app->scene->player->position.y <= 2008) TPressed2 = true;
	if (app->scene->player->position.x >= -1268 && app->scene->player->position.x <= -1226 && app->scene->player->position.y >= 1973 && app->scene->player->position.y <= 2008) TPressed3 = true;
	if (app->scene->player->position.x >= -1332 && app->scene->player->position.x <= -1290 && app->scene->player->position.y >= 2005 && app->scene->player->position.y <= 2040) TPressed4 = true;
	if (app->scene->player->position.x >= -1300 && app->scene->player->position.x <= -1258 && app->scene->player->position.y >= 2005 && app->scene->player->position.y <= 2040) TPressed5 = true;
	if (app->scene->player->position.x >= -1268 && app->scene->player->position.x <= -1226 && app->scene->player->position.y >= 2005 && app->scene->player->position.y <= 2040) TPressed6 = true;
	if (app->scene->player->position.x >= -1332 && app->scene->player->position.x <= -1290 && app->scene->player->position.y >= 2037 && app->scene->player->position.y <= 2072) TPressed7 = true;
	if (app->scene->player->position.x >= -1300 && app->scene->player->position.x <= -1258 && app->scene->player->position.y >= 2037 && app->scene->player->position.y <= 2072) TPressed8 = true;
	if (app->scene->player->position.x >= -1268 && app->scene->player->position.x <= -1226 && app->scene->player->position.y >= 2037 && app->scene->player->position.y <= 2072) TPressed9 = true;

	//Puzzle1 Detect if left and break

	if ((app->scene->player->position.x < -1332 || app->scene->player->position.x > -1290 || app->scene->player->position.y < 1973 || app->scene->player->position.y > 2008) && TPressed1 == true) TBroken1 = true;
	if ((app->scene->player->position.x < -1300 || app->scene->player->position.x > -1258 || app->scene->player->position.y < 1973 || app->scene->player->position.y > 2008) && TPressed2 == true) TBroken2 = true;
	if ((app->scene->player->position.x < -1268 || app->scene->player->position.x > -1226 || app->scene->player->position.y < 1973 || app->scene->player->position.y > 2008) && TPressed3 == true) TBroken3 = true;
	if ((app->scene->player->position.x < -1332 || app->scene->player->position.x > -1290 || app->scene->player->position.y < 2005 || app->scene->player->position.y > 2040) && TPressed4 == true) TBroken4 = true;
	if ((app->scene->player->position.x < -1300 || app->scene->player->position.x > -1258 || app->scene->player->position.y < 2005 || app->scene->player->position.y > 2040) && TPressed5 == true) TBroken5 = true;
	if ((app->scene->player->position.x < -1268 || app->scene->player->position.x > -1226 || app->scene->player->position.y < 2005 || app->scene->player->position.y > 2040) && TPressed6 == true) TBroken6 = true;
	if ((app->scene->player->position.x < -1332 || app->scene->player->position.x > -1290 || app->scene->player->position.y < 2037 || app->scene->player->position.y > 2072) && TPressed7 == true) TBroken7 = true;
	if ((app->scene->player->position.x < -1300 || app->scene->player->position.x > -1258 || app->scene->player->position.y < 2037 || app->scene->player->position.y > 2072) && TPressed8 == true) TBroken8 = true;
	if ((app->scene->player->position.x < -1268 || app->scene->player->position.x > -1226 || app->scene->player->position.y < 2037 || app->scene->player->position.y > 2072) && TPressed9 == true) TBroken9 = true;

	//Puzzle1 Draw Broken

	if (TBroken1 == true) app->render->DrawTexture(BrokenTile, -1296, 1984);
	if (TBroken2 == true) app->render->DrawTexture(BrokenTile, -1264, 1984);
	if (TBroken3 == true) app->render->DrawTexture(BrokenTile, -1232, 1984);
	if (TBroken4 == true) app->render->DrawTexture(BrokenTile, -1296, 2016);
	if (TBroken5 == true) app->render->DrawTexture(BrokenTile, -1264, 2016);
	if (TBroken6 == true) app->render->DrawTexture(BrokenTile, -1232, 2016);
	if (TBroken7 == true) app->render->DrawTexture(BrokenTile, -1296, 2048);
	if (TBroken8 == true) app->render->DrawTexture(BrokenTile, -1264, 2048);
	if (TBroken9 == true) app->render->DrawTexture(BrokenTile, -1232, 2048);

	//Puzzle1 Detect if fallen

	if ((app->scene->player->position.x >= -1332 && app->scene->player->position.x <= -1290 && app->scene->player->position.y >= 1973 && app->scene->player->position.y <= 2008) && TBroken1 == true) RestartCave();
	if ((app->scene->player->position.x >= -1300 && app->scene->player->position.x <= -1258 && app->scene->player->position.y >= 1973 && app->scene->player->position.y <= 2008) && TBroken2 == true) RestartCave();
	if ((app->scene->player->position.x >= -1268 && app->scene->player->position.x <= -1226 && app->scene->player->position.y >= 1973 && app->scene->player->position.y <= 2008) && TBroken3 == true) RestartCave();
	if ((app->scene->player->position.x >= -1332 && app->scene->player->position.x <= -1290 && app->scene->player->position.y >= 2005 && app->scene->player->position.y <= 2040) && TBroken4 == true) RestartCave();
	if ((app->scene->player->position.x >= -1300 && app->scene->player->position.x <= -1258 && app->scene->player->position.y >= 2005 && app->scene->player->position.y <= 2040) && TBroken5 == true) RestartCave();
	if ((app->scene->player->position.x >= -1268 && app->scene->player->position.x <= -1226 && app->scene->player->position.y >= 2005 && app->scene->player->position.y <= 2040) && TBroken6 == true) RestartCave();
	if ((app->scene->player->position.x >= -1332 && app->scene->player->position.x <= -1290 && app->scene->player->position.y >= 2037 && app->scene->player->position.y <= 2072) && TBroken7 == true) RestartCave();
	if ((app->scene->player->position.x >= -1300 && app->scene->player->position.x <= -1258 && app->scene->player->position.y >= 2037 && app->scene->player->position.y <= 2072) && TBroken8 == true) RestartCave();
	if ((app->scene->player->position.x >= -1268 && app->scene->player->position.x <= -1226 && app->scene->player->position.y >= 2037 && app->scene->player->position.y <= 2072) && TBroken9 == true) RestartCave();

	//Puzzle2 Normal Draw

	if (TBroken10 == false) app->render->DrawTexture(DefaultTile, -1520, 1408);
	if (TBroken11 == false) app->render->DrawTexture(DefaultTile, -1488, 1408);
	if (TBroken12 == false) app->render->DrawTexture(DefaultTile, -1456, 1408);
	if (TBroken13 == false) app->render->DrawTexture(DefaultTile, -1424, 1408);
	if (TBroken14 == false) app->render->DrawTexture(DefaultTile, -1392, 1408);
	if (TBroken15 == false) app->render->DrawTexture(DefaultTile, -1360, 1408);
	if (TBroken16 == false) app->render->DrawTexture(DefaultTile, -1520, 1440);
	if (TBroken17 == false) app->render->DrawTexture(DefaultTile, -1488, 1440);
	if (TBroken18 == false) app->render->DrawTexture(DefaultTile, -1456, 1440);
	if (TBroken19 == false) app->render->DrawTexture(DefaultTile, -1424, 1440);
	if (TBroken20 == false) app->render->DrawTexture(DefaultTile, -1392, 1440);
	if (TBroken21 == false) app->render->DrawTexture(DefaultTile, -1360, 1440);
	if (TBroken22 == false) app->render->DrawTexture(DefaultTile, -1520, 1472);
	if (TBroken23 == false) app->render->DrawTexture(DefaultTile, -1488, 1472);
	if (TBroken24 == false) app->render->DrawTexture(DefaultTile, -1456, 1472);
	if (TBroken25 == false) app->render->DrawTexture(DefaultTile, -1424, 1472);
	if (TBroken26 == false) app->render->DrawTexture(DefaultTile, -1392, 1472);
	if (TBroken27 == false) app->render->DrawTexture(DefaultTile, -1360, 1472);


	//Puzzle2 Detect if pressed

	if (app->scene->player->position.x >= -1556 && app->scene->player->position.x <= -1514 && app->scene->player->position.y >= 1397 && app->scene->player->position.y <= 1432) TPressed10 = true;
	if (app->scene->player->position.x >= -1524 && app->scene->player->position.x <= -1482 && app->scene->player->position.y >= 1397 && app->scene->player->position.y <= 1432) TPressed11 = true;
	if (app->scene->player->position.x >= -1492 && app->scene->player->position.x <= -1450 && app->scene->player->position.y >= 1397 && app->scene->player->position.y <= 1432) TPressed12 = true;
	if (app->scene->player->position.x >= -1460 && app->scene->player->position.x <= -1418 && app->scene->player->position.y >= 1397 && app->scene->player->position.y <= 1432) TPressed13 = true;
	if (app->scene->player->position.x >= -1428 && app->scene->player->position.x <= -1386 && app->scene->player->position.y >= 1397 && app->scene->player->position.y <= 1432) TPressed14 = true;
	if (app->scene->player->position.x >= -1396 && app->scene->player->position.x <= -1354 && app->scene->player->position.y >= 1397 && app->scene->player->position.y <= 1432) TPressed15 = true;
	if (app->scene->player->position.x >= -1556 && app->scene->player->position.x <= -1514 && app->scene->player->position.y >= 1429 && app->scene->player->position.y <= 1464) TPressed16 = true;
	if (app->scene->player->position.x >= -1524 && app->scene->player->position.x <= -1482 && app->scene->player->position.y >= 1429 && app->scene->player->position.y <= 1464) TPressed17 = true;
	if (app->scene->player->position.x >= -1492 && app->scene->player->position.x <= -1450 && app->scene->player->position.y >= 1429 && app->scene->player->position.y <= 1464) TPressed18 = true;
	if (app->scene->player->position.x >= -1460 && app->scene->player->position.x <= -1418 && app->scene->player->position.y >= 1429 && app->scene->player->position.y <= 1464) TPressed19 = true;
	if (app->scene->player->position.x >= -1428 && app->scene->player->position.x <= -1386 && app->scene->player->position.y >= 1429 && app->scene->player->position.y <= 1464) TPressed20 = true;
	if (app->scene->player->position.x >= -1396 && app->scene->player->position.x <= -1354 && app->scene->player->position.y >= 1429 && app->scene->player->position.y <= 1464) TPressed21 = true;
	if (app->scene->player->position.x >= -1556 && app->scene->player->position.x <= -1514 && app->scene->player->position.y >= 1461 && app->scene->player->position.y <= 1496) TPressed22 = true;
	if (app->scene->player->position.x >= -1524 && app->scene->player->position.x <= -1482 && app->scene->player->position.y >= 1461 && app->scene->player->position.y <= 1496) TPressed23 = true;
	if (app->scene->player->position.x >= -1492 && app->scene->player->position.x <= -1450 && app->scene->player->position.y >= 1461 && app->scene->player->position.y <= 1496) TPressed24 = true;
	if (app->scene->player->position.x >= -1460 && app->scene->player->position.x <= -1418 && app->scene->player->position.y >= 1461 && app->scene->player->position.y <= 1496) TPressed25 = true;
	if (app->scene->player->position.x >= -1428 && app->scene->player->position.x <= -1386 && app->scene->player->position.y >= 1461 && app->scene->player->position.y <= 1496) TPressed26 = true;
	if (app->scene->player->position.x >= -1396 && app->scene->player->position.x <= -1354 && app->scene->player->position.y >= 1461 && app->scene->player->position.y <= 1496) TPressed27 = true;

	//Puzzle2 Detect if left and break

	if ((app->scene->player->position.x < -1556 || app->scene->player->position.x > -1514 || app->scene->player->position.y < 1397 || app->scene->player->position.y > 1432) && TPressed10 == true) TBroken10 = true;
	if ((app->scene->player->position.x < -1524 || app->scene->player->position.x > -1482 || app->scene->player->position.y < 1397 || app->scene->player->position.y > 1432) && TPressed11 == true) TBroken11 = true;
	if ((app->scene->player->position.x < -1492 || app->scene->player->position.x > -1450 || app->scene->player->position.y < 1397 || app->scene->player->position.y > 1432) && TPressed12 == true) TBroken12 = true;
	if ((app->scene->player->position.x < -1460 || app->scene->player->position.x > -1418 || app->scene->player->position.y < 1397 || app->scene->player->position.y > 1432) && TPressed13 == true) TBroken13 = true;
	if ((app->scene->player->position.x < -1428 || app->scene->player->position.x > -1386 || app->scene->player->position.y < 1397 || app->scene->player->position.y > 1432) && TPressed14 == true) TBroken14 = true;
	if ((app->scene->player->position.x < -1396 || app->scene->player->position.x > -1354 || app->scene->player->position.y < 1397 || app->scene->player->position.y > 1432) && TPressed15 == true) TBroken15 = true;
	if ((app->scene->player->position.x < -1556 || app->scene->player->position.x > -1514 || app->scene->player->position.y < 1429 || app->scene->player->position.y > 1464) && TPressed16 == true) TBroken16 = true;
	if ((app->scene->player->position.x < -1524 || app->scene->player->position.x > -1482 || app->scene->player->position.y < 1429 || app->scene->player->position.y > 1464) && TPressed17 == true) TBroken17 = true;
	if ((app->scene->player->position.x < -1492 || app->scene->player->position.x > -1450 || app->scene->player->position.y < 1429 || app->scene->player->position.y > 1464) && TPressed18 == true) TBroken18 = true;
	if ((app->scene->player->position.x < -1460 || app->scene->player->position.x > -1418 || app->scene->player->position.y < 1429 || app->scene->player->position.y > 1464) && TPressed19 == true) TBroken19 = true;
	if ((app->scene->player->position.x < -1428 || app->scene->player->position.x > -1386 || app->scene->player->position.y < 1429 || app->scene->player->position.y > 1464) && TPressed20 == true) TBroken20 = true;
	if ((app->scene->player->position.x < -1396 || app->scene->player->position.x > -1354 || app->scene->player->position.y < 1429 || app->scene->player->position.y > 1464) && TPressed21 == true) TBroken21 = true;
	if ((app->scene->player->position.x < -1556 || app->scene->player->position.x > -1514 || app->scene->player->position.y < 1461 || app->scene->player->position.y > 1496) && TPressed22 == true) TBroken22 = true;
	if ((app->scene->player->position.x < -1524 || app->scene->player->position.x > -1482 || app->scene->player->position.y < 1461 || app->scene->player->position.y > 1496) && TPressed23 == true) TBroken23 = true;
	if ((app->scene->player->position.x < -1492 || app->scene->player->position.x > -1450 || app->scene->player->position.y < 1461 || app->scene->player->position.y > 1496) && TPressed24 == true) TBroken24 = true;
	if ((app->scene->player->position.x < -1460 || app->scene->player->position.x > -1418 || app->scene->player->position.y < 1461 || app->scene->player->position.y > 1496) && TPressed25 == true) TBroken25 = true;
	if ((app->scene->player->position.x < -1428 || app->scene->player->position.x > -1386 || app->scene->player->position.y < 1461 || app->scene->player->position.y > 1496) && TPressed26 == true) TBroken26 = true;
	if ((app->scene->player->position.x < -1396 || app->scene->player->position.x > -1354 || app->scene->player->position.y < 1461 || app->scene->player->position.y > 1496) && TPressed27 == true) TBroken27 = true;

	//Puzzle2 Draw Broken

	if (TBroken10 == true) app->render->DrawTexture(BrokenTile, -1520, 1408);
	if (TBroken11 == true) app->render->DrawTexture(BrokenTile, -1488, 1408);
	if (TBroken12 == true) app->render->DrawTexture(BrokenTile, -1456, 1408);
	if (TBroken13 == true) app->render->DrawTexture(BrokenTile, -1424, 1408);
	if (TBroken14 == true) app->render->DrawTexture(BrokenTile, -1392, 1408);
	if (TBroken15 == true) app->render->DrawTexture(BrokenTile, -1360, 1408);
	if (TBroken16 == true) app->render->DrawTexture(BrokenTile, -1520, 1440);
	if (TBroken17 == true) app->render->DrawTexture(BrokenTile, -1488, 1440);
	if (TBroken18 == true) app->render->DrawTexture(BrokenTile, -1456, 1440);
	if (TBroken19 == true) app->render->DrawTexture(BrokenTile, -1424, 1440);
	if (TBroken20 == true) app->render->DrawTexture(BrokenTile, -1392, 1440);
	if (TBroken21 == true) app->render->DrawTexture(BrokenTile, -1360, 1440);
	if (TBroken22 == true) app->render->DrawTexture(BrokenTile, -1520, 1472);
	if (TBroken23 == true) app->render->DrawTexture(BrokenTile, -1488, 1472);
	if (TBroken24 == true) app->render->DrawTexture(BrokenTile, -1456, 1472);
	if (TBroken25 == true) app->render->DrawTexture(BrokenTile, -1424, 1472);
	if (TBroken26 == true) app->render->DrawTexture(BrokenTile, -1392, 1472);
	if (TBroken27 == true) app->render->DrawTexture(BrokenTile, -1360, 1472);

	//Puzzle2 Detect if fallen

	if ((app->scene->player->position.x >= -1556 && app->scene->player->position.x <= -1514 && app->scene->player->position.y >= 1397 && app->scene->player->position.y <= 1432) && TBroken10 == true) RestartCave();
	if ((app->scene->player->position.x >= -1524 && app->scene->player->position.x <= -1482 && app->scene->player->position.y >= 1397 && app->scene->player->position.y <= 1432) && TBroken11 == true) RestartCave();
	if ((app->scene->player->position.x >= -1492 && app->scene->player->position.x <= -1450 && app->scene->player->position.y >= 1397 && app->scene->player->position.y <= 1432) && TBroken12 == true) RestartCave();
	if ((app->scene->player->position.x >= -1460 && app->scene->player->position.x <= -1418 && app->scene->player->position.y >= 1397 && app->scene->player->position.y <= 1432) && TBroken13 == true) RestartCave();
	if ((app->scene->player->position.x >= -1428 && app->scene->player->position.x <= -1386 && app->scene->player->position.y >= 1397 && app->scene->player->position.y <= 1432) && TBroken14 == true) RestartCave();
	if ((app->scene->player->position.x >= -1396 && app->scene->player->position.x <= -1354 && app->scene->player->position.y >= 1397 && app->scene->player->position.y <= 1432) && TBroken15 == true) RestartCave();
	if ((app->scene->player->position.x >= -1556 && app->scene->player->position.x <= -1514 && app->scene->player->position.y >= 1429 && app->scene->player->position.y <= 1464) && TBroken16 == true) RestartCave();
	if ((app->scene->player->position.x >= -1524 && app->scene->player->position.x <= -1482 && app->scene->player->position.y >= 1429 && app->scene->player->position.y <= 1464) && TBroken17 == true) RestartCave();
	if ((app->scene->player->position.x >= -1492 && app->scene->player->position.x <= -1450 && app->scene->player->position.y >= 1429 && app->scene->player->position.y <= 1464) && TBroken18 == true) RestartCave();
	if ((app->scene->player->position.x >= -1460 && app->scene->player->position.x <= -1418 && app->scene->player->position.y >= 1429 && app->scene->player->position.y <= 1464) && TBroken19 == true) RestartCave();
	if ((app->scene->player->position.x >= -1428 && app->scene->player->position.x <= -1386 && app->scene->player->position.y >= 1429 && app->scene->player->position.y <= 1464) && TBroken20 == true) RestartCave();
	if ((app->scene->player->position.x >= -1396 && app->scene->player->position.x <= -1354 && app->scene->player->position.y >= 1429 && app->scene->player->position.y <= 1464) && TBroken21 == true) RestartCave();
	if ((app->scene->player->position.x >= -1556 && app->scene->player->position.x <= -1514 && app->scene->player->position.y >= 1461 && app->scene->player->position.y <= 1496) && TBroken22 == true) RestartCave();
	if ((app->scene->player->position.x >= -1524 && app->scene->player->position.x <= -1482 && app->scene->player->position.y >= 1461 && app->scene->player->position.y <= 1496) && TBroken23 == true) RestartCave();
	if ((app->scene->player->position.x >= -1492 && app->scene->player->position.x <= -1450 && app->scene->player->position.y >= 1461 && app->scene->player->position.y <= 1496) && TBroken24 == true) RestartCave();
	if ((app->scene->player->position.x >= -1460 && app->scene->player->position.x <= -1418 && app->scene->player->position.y >= 1461 && app->scene->player->position.y <= 1496) && TBroken25 == true) RestartCave();
	if ((app->scene->player->position.x >= -1428 && app->scene->player->position.x <= -1386 && app->scene->player->position.y >= 1461 && app->scene->player->position.y <= 1496) && TBroken26 == true) RestartCave();
	if ((app->scene->player->position.x >= -1396 && app->scene->player->position.x <= -1354 && app->scene->player->position.y >= 1461 && app->scene->player->position.y <= 1496) && TBroken27 == true) RestartCave();

	//Puzzle3 Normal Draw

	if (TBroken28 == false) app->render->DrawTexture(DefaultTile, -1520, 192);
	if (TBroken29 == false) app->render->DrawTexture(DefaultTile, -1488, 192);
	if (TBroken30 == false) app->render->DrawTexture(DefaultTile, -1456, 192);
	if (TBroken31 == false) app->render->DrawTexture(DefaultTile, -1424, 192);
	if (TBroken32 == false) app->render->DrawTexture(DefaultTile, -1392, 192);
	if (TBroken33 == false) app->render->DrawTexture(DefaultTile, -1360, 192);
	if (TBroken34 == false) app->render->DrawTexture(DefaultTile, -1328, 192);
	if (TBroken35 == false) app->render->DrawTexture(DefaultTile, -1296, 192);
	if (TBroken36 == false) app->render->DrawTexture(DefaultTile, -1264, 192);
	if (TBroken37 == false) app->render->DrawTexture(DefaultTile, -1232, 192);
	if (TBroken38 == false) app->render->DrawTexture(DefaultTile, -1200, 192);
	if (TBroken39 == false) app->render->DrawTexture(DefaultTile, -1520, 224);
	if (TBroken40 == false) app->render->DrawTexture(DefaultTile, -1488, 224);
	if (TBroken41 == false) app->render->DrawTexture(DefaultTile, -1456, 224);
	if (TBroken42 == false) app->render->DrawTexture(DefaultTile, -1424, 224);
	if (TBroken43 == false) app->render->DrawTexture(DefaultTile, -1392, 224);
	if (TBroken44 == false) app->render->DrawTexture(DefaultTile, -1360, 224);
	if (TBroken45 == false) app->render->DrawTexture(DefaultTile, -1328, 224);
	if (TBroken46 == false) app->render->DrawTexture(DefaultTile, -1296, 224);
	if (TBroken47 == false) app->render->DrawTexture(DefaultTile, -1264, 224);
	if (TBroken48 == false) app->render->DrawTexture(DefaultTile, -1232, 224);
	if (TBroken49 == false) app->render->DrawTexture(DefaultTile, -1200, 224);
	if (TBroken50 == false) app->render->DrawTexture(DefaultTile, -1520, 256);
	if (TBroken51 == false) app->render->DrawTexture(DefaultTile, -1488, 256);
	if (TBroken52 == false) app->render->DrawTexture(DefaultTile, -1456, 256);
	if (TBroken53 == false) app->render->DrawTexture(DefaultTile, -1424, 256);
	if (TBroken54 == false) app->render->DrawTexture(DefaultTile, -1392, 256);
	if (TBroken55 == false) app->render->DrawTexture(DefaultTile, -1360, 256);
	if (TBroken56 == false) app->render->DrawTexture(DefaultTile, -1328, 256);
	if (TBroken57 == false) app->render->DrawTexture(DefaultTile, -1296, 256);
	if (TBroken58 == false) app->render->DrawTexture(DefaultTile, -1264, 256);
	if (TBroken59 == false) app->render->DrawTexture(DefaultTile, -1232, 256);
	if (TBroken60 == false) app->render->DrawTexture(DefaultTile, -1200, 256);
	if (TBroken61 == false) app->render->DrawTexture(DefaultTile, -1520, 288);
	if (TBroken62 == false) app->render->DrawTexture(DefaultTile, -1488, 288);
	if (TBroken63 == false) app->render->DrawTexture(DefaultTile, -1456, 288);
	if (TBroken64 == false) app->render->DrawTexture(DefaultTile, -1424, 288);
	if (TBroken65 == false) app->render->DrawTexture(DefaultTile, -1392, 288);
	if (TBroken66 == false) app->render->DrawTexture(DefaultTile, -1360, 288);
	if (TBroken67 == false) app->render->DrawTexture(DefaultTile, -1328, 288);
	if (TBroken68 == false) app->render->DrawTexture(DefaultTile, -1296, 288);
	if (TBroken69 == false) app->render->DrawTexture(DefaultTile, -1264, 288);
	if (TBroken70 == false) app->render->DrawTexture(DefaultTile, -1232, 288);
	if (TBroken71 == false) app->render->DrawTexture(DefaultTile, -1200, 288);


	//Puzzle3 Detect if pressed

	if (app->scene->player->position.x >= -1556 && app->scene->player->position.x <= -1514 && app->scene->player->position.y >= 181 && app->scene->player->position.y <= 216) TPressed28 = true;
	if (app->scene->player->position.x >= -1524 && app->scene->player->position.x <= -1482 && app->scene->player->position.y >= 181 && app->scene->player->position.y <= 216) TPressed29 = true;
	if (app->scene->player->position.x >= -1492 && app->scene->player->position.x <= -1450 && app->scene->player->position.y >= 181 && app->scene->player->position.y <= 216) TPressed30 = true;
	if (app->scene->player->position.x >= -1460 && app->scene->player->position.x <= -1418 && app->scene->player->position.y >= 181 && app->scene->player->position.y <= 216) TPressed31 = true;
	if (app->scene->player->position.x >= -1428 && app->scene->player->position.x <= -1386 && app->scene->player->position.y >= 181 && app->scene->player->position.y <= 216) TPressed32 = true;
	if (app->scene->player->position.x >= -1396 && app->scene->player->position.x <= -1354 && app->scene->player->position.y >= 181 && app->scene->player->position.y <= 216) TPressed33 = true;
	if (app->scene->player->position.x >= -1364 && app->scene->player->position.x <= -1322 && app->scene->player->position.y >= 181 && app->scene->player->position.y <= 216) TPressed34 = true;
	if (app->scene->player->position.x >= -1332 && app->scene->player->position.x <= -1290 && app->scene->player->position.y >= 181 && app->scene->player->position.y <= 216) TPressed35 = true;
	if (app->scene->player->position.x >= -1300 && app->scene->player->position.x <= -1258 && app->scene->player->position.y >= 181 && app->scene->player->position.y <= 216) TPressed36 = true;
	if (app->scene->player->position.x >= -1268 && app->scene->player->position.x <= -1226 && app->scene->player->position.y >= 181 && app->scene->player->position.y <= 216) TPressed37 = true;
	if (app->scene->player->position.x >= -1236 && app->scene->player->position.x <= -1194 && app->scene->player->position.y >= 181 && app->scene->player->position.y <= 216) TPressed38 = true;
	if (app->scene->player->position.x >= -1556 && app->scene->player->position.x <= -1514 && app->scene->player->position.y >= 213 && app->scene->player->position.y <= 248) TPressed39 = true;
	if (app->scene->player->position.x >= -1524 && app->scene->player->position.x <= -1482 && app->scene->player->position.y >= 213 && app->scene->player->position.y <= 248) TPressed40 = true;
	if (app->scene->player->position.x >= -1492 && app->scene->player->position.x <= -1450 && app->scene->player->position.y >= 213 && app->scene->player->position.y <= 248) TPressed41 = true;
	if (app->scene->player->position.x >= -1460 && app->scene->player->position.x <= -1418 && app->scene->player->position.y >= 213 && app->scene->player->position.y <= 248) TPressed42 = true;
	if (app->scene->player->position.x >= -1428 && app->scene->player->position.x <= -1386 && app->scene->player->position.y >= 213 && app->scene->player->position.y <= 248) TPressed43 = true;
	if (app->scene->player->position.x >= -1396 && app->scene->player->position.x <= -1354 && app->scene->player->position.y >= 213 && app->scene->player->position.y <= 248) TPressed44 = true;
	if (app->scene->player->position.x >= -1364 && app->scene->player->position.x <= -1322 && app->scene->player->position.y >= 213 && app->scene->player->position.y <= 248) TPressed45 = true;
	if (app->scene->player->position.x >= -1332 && app->scene->player->position.x <= -1290 && app->scene->player->position.y >= 213 && app->scene->player->position.y <= 248) TPressed46 = true;
	if (app->scene->player->position.x >= -1300 && app->scene->player->position.x <= -1258 && app->scene->player->position.y >= 213 && app->scene->player->position.y <= 248) TPressed47 = true;
	if (app->scene->player->position.x >= -1268 && app->scene->player->position.x <= -1226 && app->scene->player->position.y >= 213 && app->scene->player->position.y <= 248) TPressed48 = true;
	if (app->scene->player->position.x >= -1236 && app->scene->player->position.x <= -1194 && app->scene->player->position.y >= 213 && app->scene->player->position.y <= 248) TPressed49 = true;
	if (app->scene->player->position.x >= -1556 && app->scene->player->position.x <= -1514 && app->scene->player->position.y >= 245 && app->scene->player->position.y <= 280) TPressed50 = true;
	if (app->scene->player->position.x >= -1524 && app->scene->player->position.x <= -1482 && app->scene->player->position.y >= 245 && app->scene->player->position.y <= 280) TPressed51 = true;
	if (app->scene->player->position.x >= -1492 && app->scene->player->position.x <= -1450 && app->scene->player->position.y >= 245 && app->scene->player->position.y <= 280) TPressed52 = true;
	if (app->scene->player->position.x >= -1460 && app->scene->player->position.x <= -1418 && app->scene->player->position.y >= 245 && app->scene->player->position.y <= 280) TPressed53 = true;
	if (app->scene->player->position.x >= -1428 && app->scene->player->position.x <= -1386 && app->scene->player->position.y >= 245 && app->scene->player->position.y <= 280) TPressed54 = true;
	if (app->scene->player->position.x >= -1396 && app->scene->player->position.x <= -1354 && app->scene->player->position.y >= 245 && app->scene->player->position.y <= 280) TPressed55 = true;
	if (app->scene->player->position.x >= -1364 && app->scene->player->position.x <= -1322 && app->scene->player->position.y >= 245 && app->scene->player->position.y <= 280) TPressed56 = true;
	if (app->scene->player->position.x >= -1332 && app->scene->player->position.x <= -1290 && app->scene->player->position.y >= 245 && app->scene->player->position.y <= 280) TPressed57 = true;
	if (app->scene->player->position.x >= -1300 && app->scene->player->position.x <= -1258 && app->scene->player->position.y >= 245 && app->scene->player->position.y <= 280) TPressed58 = true;
	if (app->scene->player->position.x >= -1268 && app->scene->player->position.x <= -1226 && app->scene->player->position.y >= 245 && app->scene->player->position.y <= 280) TPressed59 = true;
	if (app->scene->player->position.x >= -1236 && app->scene->player->position.x <= -1194 && app->scene->player->position.y >= 245 && app->scene->player->position.y <= 280) TPressed60 = true;
	if (app->scene->player->position.x >= -1556 && app->scene->player->position.x <= -1514 && app->scene->player->position.y >= 277 && app->scene->player->position.y <= 312) TPressed61 = true;
	if (app->scene->player->position.x >= -1524 && app->scene->player->position.x <= -1482 && app->scene->player->position.y >= 277 && app->scene->player->position.y <= 312) TPressed62 = true;
	if (app->scene->player->position.x >= -1492 && app->scene->player->position.x <= -1450 && app->scene->player->position.y >= 277 && app->scene->player->position.y <= 312) TPressed63 = true;
	if (app->scene->player->position.x >= -1460 && app->scene->player->position.x <= -1418 && app->scene->player->position.y >= 277 && app->scene->player->position.y <= 312) TPressed64 = true;
	if (app->scene->player->position.x >= -1428 && app->scene->player->position.x <= -1386 && app->scene->player->position.y >= 277 && app->scene->player->position.y <= 312) TPressed65 = true;
	if (app->scene->player->position.x >= -1396 && app->scene->player->position.x <= -1354 && app->scene->player->position.y >= 277 && app->scene->player->position.y <= 312) TPressed66 = true;
	if (app->scene->player->position.x >= -1364 && app->scene->player->position.x <= -1322 && app->scene->player->position.y >= 277 && app->scene->player->position.y <= 312) TPressed67 = true;
	if (app->scene->player->position.x >= -1332 && app->scene->player->position.x <= -1290 && app->scene->player->position.y >= 277 && app->scene->player->position.y <= 312) TPressed68 = true;
	if (app->scene->player->position.x >= -1300 && app->scene->player->position.x <= -1258 && app->scene->player->position.y >= 277 && app->scene->player->position.y <= 312) TPressed69 = true;
	if (app->scene->player->position.x >= -1268 && app->scene->player->position.x <= -1226 && app->scene->player->position.y >= 277 && app->scene->player->position.y <= 312) TPressed70 = true;
	if (app->scene->player->position.x >= -1236 && app->scene->player->position.x <= -1194 && app->scene->player->position.y >= 277 && app->scene->player->position.y <= 312) TPressed71 = true;

	//Puzzle3 Detect if left and break

	if ((app->scene->player->position.x < -1556 || app->scene->player->position.x > -1514 || app->scene->player->position.y < 181 || app->scene->player->position.y > 216) && TPressed28 == true) TBroken28 = true;
	if ((app->scene->player->position.x < -1524 || app->scene->player->position.x > -1482 || app->scene->player->position.y < 181 || app->scene->player->position.y > 216) && TPressed29 == true) TBroken29 = true;
	if ((app->scene->player->position.x < -1492 || app->scene->player->position.x > -1450 || app->scene->player->position.y < 181 || app->scene->player->position.y > 216) && TPressed30 == true) TBroken30 = true;
	if ((app->scene->player->position.x < -1460 || app->scene->player->position.x > -1418 || app->scene->player->position.y < 181 || app->scene->player->position.y > 216) && TPressed31 == true) TBroken31 = true;
	if ((app->scene->player->position.x < -1428 || app->scene->player->position.x > -1386 || app->scene->player->position.y < 181 || app->scene->player->position.y > 216) && TPressed32 == true) TBroken32 = true;
	if ((app->scene->player->position.x < -1396 || app->scene->player->position.x > -1354 || app->scene->player->position.y < 181 || app->scene->player->position.y > 216) && TPressed33 == true) TBroken33 = true;
	if ((app->scene->player->position.x < -1364 || app->scene->player->position.x > -1322 || app->scene->player->position.y < 181 || app->scene->player->position.y > 216) && TPressed34 == true) TBroken34 = true;
	if ((app->scene->player->position.x < -1332 || app->scene->player->position.x > -1290 || app->scene->player->position.y < 181 || app->scene->player->position.y > 216) && TPressed35 == true) TBroken35 = true;
	if ((app->scene->player->position.x < -1300 || app->scene->player->position.x > -1258 || app->scene->player->position.y < 181 || app->scene->player->position.y > 216) && TPressed36 == true) TBroken36 = true;
	if ((app->scene->player->position.x < -1268 || app->scene->player->position.x > -1226 || app->scene->player->position.y < 181 || app->scene->player->position.y > 216) && TPressed37 == true) TBroken37 = true;
	if ((app->scene->player->position.x < -1236 || app->scene->player->position.x > -1194 || app->scene->player->position.y < 181 || app->scene->player->position.y > 216) && TPressed38 == true) TBroken38 = true;
	if ((app->scene->player->position.x < -1556 || app->scene->player->position.x > -1514 || app->scene->player->position.y < 213 || app->scene->player->position.y > 248) && TPressed39 == true) TBroken39 = true;
	if ((app->scene->player->position.x < -1524 || app->scene->player->position.x > -1482 || app->scene->player->position.y < 213 || app->scene->player->position.y > 248) && TPressed40 == true) TBroken40 = true;
	if ((app->scene->player->position.x < -1492 || app->scene->player->position.x > -1450 || app->scene->player->position.y < 213 || app->scene->player->position.y > 248) && TPressed41 == true) TBroken41 = true;
	if ((app->scene->player->position.x < -1460 || app->scene->player->position.x > -1418 || app->scene->player->position.y < 213 || app->scene->player->position.y > 248) && TPressed42 == true) TBroken42 = true;
	if ((app->scene->player->position.x < -1428 || app->scene->player->position.x > -1386 || app->scene->player->position.y < 213 || app->scene->player->position.y > 248) && TPressed43 == true) TBroken43 = true;
	if ((app->scene->player->position.x < -1396 || app->scene->player->position.x > -1354 || app->scene->player->position.y < 213 || app->scene->player->position.y > 248) && TPressed44 == true) TBroken44 = true;
	if ((app->scene->player->position.x < -1364 || app->scene->player->position.x > -1322 || app->scene->player->position.y < 213 || app->scene->player->position.y > 248) && TPressed45 == true) TBroken45 = true;
	if ((app->scene->player->position.x < -1332 || app->scene->player->position.x > -1290 || app->scene->player->position.y < 213 || app->scene->player->position.y > 248) && TPressed46 == true) TBroken46 = true;
	if ((app->scene->player->position.x < -1300 || app->scene->player->position.x > -1258 || app->scene->player->position.y < 213 || app->scene->player->position.y > 248) && TPressed47 == true) TBroken47 = true;
	if ((app->scene->player->position.x < -1268 || app->scene->player->position.x > -1226 || app->scene->player->position.y < 213 || app->scene->player->position.y > 248) && TPressed48 == true) TBroken48 = true;
	if ((app->scene->player->position.x < -1236 || app->scene->player->position.x > -1194 || app->scene->player->position.y < 213 || app->scene->player->position.y > 248) && TPressed49 == true) TBroken49 = true;
	if ((app->scene->player->position.x < -1556 || app->scene->player->position.x > -1514 || app->scene->player->position.y < 245 || app->scene->player->position.y > 280) && TPressed50 == true) TBroken50 = true;
	if ((app->scene->player->position.x < -1524 || app->scene->player->position.x > -1482 || app->scene->player->position.y < 245 || app->scene->player->position.y > 280) && TPressed51 == true) TBroken51 = true;
	if ((app->scene->player->position.x < -1492 || app->scene->player->position.x > -1450 || app->scene->player->position.y < 245 || app->scene->player->position.y > 280) && TPressed52 == true) TBroken52 = true;
	if ((app->scene->player->position.x < -1460 || app->scene->player->position.x > -1418 || app->scene->player->position.y < 245 || app->scene->player->position.y > 280) && TPressed53 == true) TBroken53 = true;
	if ((app->scene->player->position.x < -1428 || app->scene->player->position.x > -1386 || app->scene->player->position.y < 245 || app->scene->player->position.y > 280) && TPressed54 == true) TBroken54 = true;
	if ((app->scene->player->position.x < -1396 || app->scene->player->position.x > -1354 || app->scene->player->position.y < 245 || app->scene->player->position.y > 280) && TPressed55 == true) TBroken55 = true;
	if ((app->scene->player->position.x < -1364 || app->scene->player->position.x > -1322 || app->scene->player->position.y < 245 || app->scene->player->position.y > 280) && TPressed56 == true) TBroken56 = true;
	if ((app->scene->player->position.x < -1332 || app->scene->player->position.x > -1290 || app->scene->player->position.y < 245 || app->scene->player->position.y > 280) && TPressed57 == true) TBroken57 = true;
	if ((app->scene->player->position.x < -1300 || app->scene->player->position.x > -1258 || app->scene->player->position.y < 245 || app->scene->player->position.y > 280) && TPressed58 == true) TBroken58 = true;
	if ((app->scene->player->position.x < -1268 || app->scene->player->position.x > -1226 || app->scene->player->position.y < 245 || app->scene->player->position.y > 280) && TPressed59 == true) TBroken59 = true;
	if ((app->scene->player->position.x < -1236 || app->scene->player->position.x > -1194 || app->scene->player->position.y < 245 || app->scene->player->position.y > 280) && TPressed60 == true) TBroken60 = true;
	if ((app->scene->player->position.x < -1556 || app->scene->player->position.x > -1514 || app->scene->player->position.y < 277 || app->scene->player->position.y > 312) && TPressed61 == true) TBroken61 = true;
	if ((app->scene->player->position.x < -1524 || app->scene->player->position.x > -1482 || app->scene->player->position.y < 277 || app->scene->player->position.y > 312) && TPressed62 == true) TBroken62 = true;
	if ((app->scene->player->position.x < -1492 || app->scene->player->position.x > -1450 || app->scene->player->position.y < 277 || app->scene->player->position.y > 312) && TPressed63 == true) TBroken63 = true;
	if ((app->scene->player->position.x < -1460 || app->scene->player->position.x > -1418 || app->scene->player->position.y < 277 || app->scene->player->position.y > 312) && TPressed64 == true) TBroken64 = true;
	if ((app->scene->player->position.x < -1428 || app->scene->player->position.x > -1386 || app->scene->player->position.y < 277 || app->scene->player->position.y > 312) && TPressed65 == true) TBroken65 = true;
	if ((app->scene->player->position.x < -1396 || app->scene->player->position.x > -1354 || app->scene->player->position.y < 277 || app->scene->player->position.y > 312) && TPressed66 == true) TBroken66 = true;
	if ((app->scene->player->position.x < -1364 || app->scene->player->position.x > -1322 || app->scene->player->position.y < 277 || app->scene->player->position.y > 312) && TPressed67 == true) TBroken67 = true;
	if ((app->scene->player->position.x < -1332 || app->scene->player->position.x > -1290 || app->scene->player->position.y < 277 || app->scene->player->position.y > 312) && TPressed68 == true) TBroken68 = true;
	if ((app->scene->player->position.x < -1300 || app->scene->player->position.x > -1258 || app->scene->player->position.y < 277 || app->scene->player->position.y > 312) && TPressed69 == true) TBroken69 = true;
	if ((app->scene->player->position.x < -1268 || app->scene->player->position.x > -1226 || app->scene->player->position.y < 277 || app->scene->player->position.y > 312) && TPressed70 == true) TBroken70 = true;
	if ((app->scene->player->position.x < -1236 || app->scene->player->position.x > -1194 || app->scene->player->position.y < 277 || app->scene->player->position.y > 312) && TPressed71 == true) TBroken71 = true;

	//Puzzle3 Draw Broken

	if (TBroken28 == true) app->render->DrawTexture(BrokenTile, -1520, 192);
	if (TBroken29 == true) app->render->DrawTexture(BrokenTile, -1488, 192);
	if (TBroken30 == true) app->render->DrawTexture(BrokenTile, -1456, 192);
	if (TBroken31 == true) app->render->DrawTexture(BrokenTile, -1424, 192);
	if (TBroken32 == true) app->render->DrawTexture(BrokenTile, -1392, 192);
	if (TBroken33 == true) app->render->DrawTexture(BrokenTile, -1360, 192);
	if (TBroken34 == true) app->render->DrawTexture(BrokenTile, -1328, 192);
	if (TBroken35 == true) app->render->DrawTexture(BrokenTile, -1296, 192);
	if (TBroken36 == true) app->render->DrawTexture(BrokenTile, -1264, 192);
	if (TBroken37 == true) app->render->DrawTexture(BrokenTile, -1232, 192);
	if (TBroken38 == true) app->render->DrawTexture(BrokenTile, -1200, 192);
	if (TBroken39 == true) app->render->DrawTexture(BrokenTile, -1520, 224);
	if (TBroken40 == true) app->render->DrawTexture(BrokenTile, -1488, 224);
	if (TBroken41 == true) app->render->DrawTexture(BrokenTile, -1456, 224);
	if (TBroken42 == true) app->render->DrawTexture(BrokenTile, -1424, 224);
	if (TBroken43 == true) app->render->DrawTexture(BrokenTile, -1392, 224);
	if (TBroken44 == true) app->render->DrawTexture(BrokenTile, -1360, 224);
	if (TBroken45 == true) app->render->DrawTexture(BrokenTile, -1328, 224);
	if (TBroken46 == true) app->render->DrawTexture(BrokenTile, -1296, 224);
	if (TBroken47 == true) app->render->DrawTexture(BrokenTile, -1264, 224);
	if (TBroken48 == true) app->render->DrawTexture(BrokenTile, -1232, 224);
	if (TBroken49 == true) app->render->DrawTexture(BrokenTile, -1200, 224);
	if (TBroken50 == true) app->render->DrawTexture(BrokenTile, -1520, 256);
	if (TBroken51 == true) app->render->DrawTexture(BrokenTile, -1488, 256);
	if (TBroken52 == true) app->render->DrawTexture(BrokenTile, -1456, 256);
	if (TBroken53 == true) app->render->DrawTexture(BrokenTile, -1424, 256);
	if (TBroken54 == true) app->render->DrawTexture(BrokenTile, -1392, 256);
	if (TBroken55 == true) app->render->DrawTexture(BrokenTile, -1360, 256);
	if (TBroken56 == true) app->render->DrawTexture(BrokenTile, -1328, 256);
	if (TBroken57 == true) app->render->DrawTexture(BrokenTile, -1296, 256);
	if (TBroken58 == true) app->render->DrawTexture(BrokenTile, -1264, 256);
	if (TBroken59 == true) app->render->DrawTexture(BrokenTile, -1232, 256);
	if (TBroken60 == true) app->render->DrawTexture(BrokenTile, -1200, 256);
	if (TBroken61 == true) app->render->DrawTexture(BrokenTile, -1520, 288);
	if (TBroken62 == true) app->render->DrawTexture(BrokenTile, -1488, 288);
	if (TBroken63 == true) app->render->DrawTexture(BrokenTile, -1456, 288);
	if (TBroken64 == true) app->render->DrawTexture(BrokenTile, -1424, 288);
	if (TBroken65 == true) app->render->DrawTexture(BrokenTile, -1392, 288);
	if (TBroken66 == true) app->render->DrawTexture(BrokenTile, -1360, 288);
	if (TBroken67 == true) app->render->DrawTexture(BrokenTile, -1328, 288);
	if (TBroken68 == true) app->render->DrawTexture(BrokenTile, -1296, 288);
	if (TBroken69 == true) app->render->DrawTexture(BrokenTile, -1264, 288);
	if (TBroken70 == true) app->render->DrawTexture(BrokenTile, -1232, 288);
	if (TBroken71 == true) app->render->DrawTexture(BrokenTile, -1200, 288);

	//Puzzle3 Detect if fallen

	if ((app->scene->player->position.x >= -1556 && app->scene->player->position.x <= -1514 && app->scene->player->position.y >= 181 && app->scene->player->position.y <= 216) && TBroken28 == true) RestartCave();
	if ((app->scene->player->position.x >= -1524 && app->scene->player->position.x <= -1482 && app->scene->player->position.y >= 181 && app->scene->player->position.y <= 216) && TBroken29 == true) RestartCave();
	if ((app->scene->player->position.x >= -1492 && app->scene->player->position.x <= -1450 && app->scene->player->position.y >= 181 && app->scene->player->position.y <= 216) && TBroken30 == true) RestartCave();
	if ((app->scene->player->position.x >= -1460 && app->scene->player->position.x <= -1418 && app->scene->player->position.y >= 181 && app->scene->player->position.y <= 216) && TBroken31 == true) RestartCave();
	if ((app->scene->player->position.x >= -1428 && app->scene->player->position.x <= -1386 && app->scene->player->position.y >= 181 && app->scene->player->position.y <= 216) && TBroken32 == true) RestartCave();
	if ((app->scene->player->position.x >= -1396 && app->scene->player->position.x <= -1354 && app->scene->player->position.y >= 181 && app->scene->player->position.y <= 216) && TBroken33 == true) RestartCave();
	if ((app->scene->player->position.x >= -1364 && app->scene->player->position.x <= -1322 && app->scene->player->position.y >= 181 && app->scene->player->position.y <= 216) && TBroken34 == true) RestartCave();
	if ((app->scene->player->position.x >= -1332 && app->scene->player->position.x <= -1290 && app->scene->player->position.y >= 181 && app->scene->player->position.y <= 216) && TBroken35 == true) RestartCave();
	if ((app->scene->player->position.x >= -1300 && app->scene->player->position.x <= -1258 && app->scene->player->position.y >= 181 && app->scene->player->position.y <= 216) && TBroken36 == true) RestartCave();
	if ((app->scene->player->position.x >= -1268 && app->scene->player->position.x <= -1226 && app->scene->player->position.y >= 181 && app->scene->player->position.y <= 216) && TBroken37 == true) RestartCave();
	if ((app->scene->player->position.x >= -1236 && app->scene->player->position.x <= -1194 && app->scene->player->position.y >= 181 && app->scene->player->position.y <= 216) && TBroken38 == true) RestartCave();
	if ((app->scene->player->position.x >= -1556 && app->scene->player->position.x <= -1514 && app->scene->player->position.y >= 213 && app->scene->player->position.y <= 248) && TBroken39 == true) RestartCave();
	if ((app->scene->player->position.x >= -1524 && app->scene->player->position.x <= -1482 && app->scene->player->position.y >= 213 && app->scene->player->position.y <= 248) && TBroken40 == true) RestartCave();
	if ((app->scene->player->position.x >= -1492 && app->scene->player->position.x <= -1450 && app->scene->player->position.y >= 213 && app->scene->player->position.y <= 248) && TBroken41 == true) RestartCave();
	if ((app->scene->player->position.x >= -1460 && app->scene->player->position.x <= -1418 && app->scene->player->position.y >= 213 && app->scene->player->position.y <= 248) && TBroken42 == true) RestartCave();
	if ((app->scene->player->position.x >= -1428 && app->scene->player->position.x <= -1386 && app->scene->player->position.y >= 213 && app->scene->player->position.y <= 248) && TBroken43 == true) RestartCave();
	if ((app->scene->player->position.x >= -1396 && app->scene->player->position.x <= -1354 && app->scene->player->position.y >= 213 && app->scene->player->position.y <= 248) && TBroken44 == true) RestartCave();
	if ((app->scene->player->position.x >= -1364 && app->scene->player->position.x <= -1322 && app->scene->player->position.y >= 213 && app->scene->player->position.y <= 248) && TBroken45 == true) RestartCave();
	if ((app->scene->player->position.x >= -1332 && app->scene->player->position.x <= -1290 && app->scene->player->position.y >= 213 && app->scene->player->position.y <= 248) && TBroken46 == true) RestartCave();
	if ((app->scene->player->position.x >= -1300 && app->scene->player->position.x <= -1258 && app->scene->player->position.y >= 213 && app->scene->player->position.y <= 248) && TBroken47 == true) RestartCave();
	if ((app->scene->player->position.x >= -1268 && app->scene->player->position.x <= -1226 && app->scene->player->position.y >= 213 && app->scene->player->position.y <= 248) && TBroken48 == true) RestartCave();
	if ((app->scene->player->position.x >= -1236 && app->scene->player->position.x <= -1194 && app->scene->player->position.y >= 213 && app->scene->player->position.y <= 248) && TBroken49 == true) RestartCave();
	if ((app->scene->player->position.x >= -1556 && app->scene->player->position.x <= -1514 && app->scene->player->position.y >= 245 && app->scene->player->position.y <= 280) && TBroken50 == true) RestartCave();
	if ((app->scene->player->position.x >= -1524 && app->scene->player->position.x <= -1482 && app->scene->player->position.y >= 245 && app->scene->player->position.y <= 280) && TBroken51 == true) RestartCave();
	if ((app->scene->player->position.x >= -1492 && app->scene->player->position.x <= -1450 && app->scene->player->position.y >= 245 && app->scene->player->position.y <= 280) && TBroken52 == true) RestartCave();
	if ((app->scene->player->position.x >= -1460 && app->scene->player->position.x <= -1418 && app->scene->player->position.y >= 245 && app->scene->player->position.y <= 280) && TBroken53 == true) RestartCave();
	if ((app->scene->player->position.x >= -1428 && app->scene->player->position.x <= -1386 && app->scene->player->position.y >= 245 && app->scene->player->position.y <= 280) && TBroken54 == true) RestartCave();
	if ((app->scene->player->position.x >= -1396 && app->scene->player->position.x <= -1354 && app->scene->player->position.y >= 245 && app->scene->player->position.y <= 280) && TBroken55 == true) RestartCave();
	if ((app->scene->player->position.x >= -1364 && app->scene->player->position.x <= -1322 && app->scene->player->position.y >= 245 && app->scene->player->position.y <= 280) && TBroken56 == true) RestartCave();
	if ((app->scene->player->position.x >= -1332 && app->scene->player->position.x <= -1290 && app->scene->player->position.y >= 245 && app->scene->player->position.y <= 280) && TBroken57 == true) RestartCave();
	if ((app->scene->player->position.x >= -1300 && app->scene->player->position.x <= -1258 && app->scene->player->position.y >= 245 && app->scene->player->position.y <= 280) && TBroken58 == true) RestartCave();
	if ((app->scene->player->position.x >= -1268 && app->scene->player->position.x <= -1226 && app->scene->player->position.y >= 245 && app->scene->player->position.y <= 280) && TBroken59 == true) RestartCave();
	if ((app->scene->player->position.x >= -1236 && app->scene->player->position.x <= -1194 && app->scene->player->position.y >= 245 && app->scene->player->position.y <= 280) && TBroken60 == true) RestartCave();
	if ((app->scene->player->position.x >= -1556 && app->scene->player->position.x <= -1514 && app->scene->player->position.y >= 277 && app->scene->player->position.y <= 312) && TBroken61 == true) RestartCave();
	if ((app->scene->player->position.x >= -1524 && app->scene->player->position.x <= -1482 && app->scene->player->position.y >= 277 && app->scene->player->position.y <= 312) && TBroken62 == true) RestartCave();
	if ((app->scene->player->position.x >= -1492 && app->scene->player->position.x <= -1450 && app->scene->player->position.y >= 277 && app->scene->player->position.y <= 312) && TBroken63 == true) RestartCave();
	if ((app->scene->player->position.x >= -1460 && app->scene->player->position.x <= -1418 && app->scene->player->position.y >= 277 && app->scene->player->position.y <= 312) && TBroken64 == true) RestartCave();
	if ((app->scene->player->position.x >= -1428 && app->scene->player->position.x <= -1386 && app->scene->player->position.y >= 277 && app->scene->player->position.y <= 312) && TBroken65 == true) RestartCave();
	if ((app->scene->player->position.x >= -1396 && app->scene->player->position.x <= -1354 && app->scene->player->position.y >= 277 && app->scene->player->position.y <= 312) && TBroken66 == true) RestartCave();
	if ((app->scene->player->position.x >= -1364 && app->scene->player->position.x <= -1322 && app->scene->player->position.y >= 277 && app->scene->player->position.y <= 312) && TBroken67 == true) RestartCave();
	if ((app->scene->player->position.x >= -1332 && app->scene->player->position.x <= -1290 && app->scene->player->position.y >= 277 && app->scene->player->position.y <= 312) && TBroken68 == true) RestartCave();
	if ((app->scene->player->position.x >= -1300 && app->scene->player->position.x <= -1258 && app->scene->player->position.y >= 277 && app->scene->player->position.y <= 312) && TBroken69 == true) RestartCave();
	if ((app->scene->player->position.x >= -1268 && app->scene->player->position.x <= -1226 && app->scene->player->position.y >= 277 && app->scene->player->position.y <= 312) && TBroken70 == true) RestartCave();
	if ((app->scene->player->position.x >= -1236 && app->scene->player->position.x <= -1194 && app->scene->player->position.y >= 277 && app->scene->player->position.y <= 312) && TBroken71 == true) RestartCave();



    // Draw map
	if (app->Instance == 0) {
		//app->map->Draw();
		app->render->DrawTexture(TownPNG, 0, 0);
	}
	if (app->Instance == 1) {
		app->tavern->Draw();
	}
	//DO NOT TOUCH!!!
	if (app->Instance == 2) {
		app->blacksmith->Draw();
	}
	if (app->Instance == 3) {
		app->house->Draw();
	}
	if (app->Instance == 4) {
		//app->lakedungeon->Draw();
		app->render->DrawTexture(WaterPNG, 2500, -2500);
	}
	if (app->Instance == 5) {
		//app->forestdungeon->Draw();
		app->render->DrawTexture(ForestPNG, 0, 2000);
	}
	if (app->Instance == 6) {
		app->cavedungeon->Draw();
	}
	if (app->Instance == 7) {
		app->monolith->Draw();
	}

	for (int i = 0; i < 15; i++)
	{
		if (itemInstance[i] == app->Instance)
		{
			if (itemPicked[i] == false) {
				itemBody[i]->body->SetActive(true);
				app->render->DrawTexture(texturas[i], itemPos[i].x, itemPos[i].y);
			}
			else {
				itemBody[i]->body->SetActive(false);
			}
		}

	}

	//Lake dungeon
	app->render->DrawTexture(Tp, 3520, 415);
	//Draw enemies lake
	//app->render->DrawTexture(Slime, 3384, -36);
	//app->render->DrawTexture(Slime, 3608, 28);
	//app->render->DrawTexture(Slime, 3608, -420);
	//app->render->DrawTexture(Slime, 3160, -388);
	//app->render->DrawTexture(Slime, 3192, -228);
	//app->render->DrawTexture(Slime, 2936, -836);
	//app->render->DrawTexture(Slime, 2904, -1156);
	//app->render->DrawTexture(Slime, 3224, -1156);
	//app->render->DrawTexture(Slime, 3832, -1188);
	//app->render->DrawTexture(Slime, 3672, -1028);
	//app->render->DrawTexture(Slime, 3800, -740);

	//app->render->DrawTexture(Lily, 3592, -265);
	//app->render->DrawTexture(Lily, 3752, -873);
	//app->render->DrawTexture(Lily, 3400, -873);
	//app->render->DrawTexture(Lily, 3208, -1033);
	//app->render->DrawTexture(Lily, 2984, -1033);

	if (BW == false) {
		app->render->DrawTexture(Naiadon, 3400, -1876);
	}
	if (BF == false) {
		app->render->DrawTexture(Driadon, 1206, 2311);
	}
	if (BC == false) {
		app->render->DrawTexture(Gasha, -1402 + 5, -462 + 10);
	}
	if (BW == true) {
		Naiadon1->body->SetActive(false);
	}
	if (BF == true) {
		Driadon1->body->SetActive(false);
	}
	if (BC == true) {
		Gasha1->body->SetActive(false);
	}

	//Puzzle Lake
	Puzzle_Lake();

	//Forest Dungeon
	//Forest puzzle	
	if (P1Active == false) {
		app->render->DrawTexture(Pilar1N, 736-16, 4304);
	}
	else {
		app->render->DrawTexture(Pilar1A, 736 - 16, 4304);
	}
	if (P2Active == false) {
		app->render->DrawTexture(Pilar2N, 2272 - 16, 4080);
	}
	else {
		app->render->DrawTexture(Pilar2A, 2272 - 16, 4080);
	}
	if (P3Active == false) {
		app->render->DrawTexture(Pilar3N, 2272 - 16, 5456);
	}
	else {
		app->render->DrawTexture(Pilar3A, 2272 - 16, 5456);
	}
	if (P1Active == true && P2Active == true && P3Active == true) {
		FdoorActive = true;
	}
	if (FdoorActive == false) {
		app->render->DrawTexture(ForestDoor, 1216, 3376);
	}
	else {
		Fdoor->body->SetActive(false);
	}

	//Cave Blocks	
	if (B1Block == false) {
		app->render->DrawTexture(Blocking, -1264, 1952-64);
	}
	else {
		Block1->body->SetActive(false);
	}
	if (B2Block == false) {
		app->render->DrawTexture(Blocking, -1424, 1376-64);
	}
	else {
		Block2->body->SetActive(false);
	}
	if (B3Block == false) {
		app->render->DrawTexture(Blocking, -1360, 160-64);
	}
	else {
		Block3->body->SetActive(false);
	}

	//Cave blocks all pressed
	if (app->scene->player->position.y > 2149) {
		Reload = true;
		block1Fx = false;
		block2Fx = false;
		block3Fx = false;
		RestartCave();
	}
	else {
		Reload = false;
	}

	if (TPressed1 == true && TPressed2 == true && TPressed3 == true && TPressed4 == true && TPressed5 == true && TPressed6 == true && TPressed7 == true && TPressed8 == true && TPressed9 == true) {
		B1Block = true;
		if (block1Fx == false) {
			app->audio->PlayFxWithVolume(puzzleFX, 0, app->audio->fxvolume);
			block1Fx = true;
		}
	}
	if (TPressed10 == true && TPressed11 == true && TPressed12 == true && TPressed13 == true && TPressed14 == true && TPressed15 == true && TPressed16 == true && TPressed17 == true && TPressed18 == true && TPressed19 == true && TPressed20 == true && TPressed21 == true && TPressed22 == true && TPressed23 == true && TPressed24 == true && TPressed25 == true && TPressed26 == true && TPressed27 == true) {
		B2Block = true;
		if (block2Fx == false) {
			app->audio->PlayFxWithVolume(puzzleFX, 0, app->audio->fxvolume);
			block2Fx = true;
		}
	}
	if (TPressed28 == true && TPressed29 == true && TPressed30 == true && TPressed31 == true && TPressed32 == true && TPressed33 == true && TPressed34 == true && TPressed35 == true && TPressed36 == true && TPressed37 == true && TPressed38 == true && TPressed39 == true && TPressed40 == true && TPressed41 == true && TPressed42 == true && TPressed43 == true && TPressed44 == true && TPressed45 == true && TPressed46 == true && TPressed47 == true && TPressed48 == true && TPressed49 == true && TPressed50 == true && TPressed51 == true && TPressed52 == true && TPressed53 == true && TPressed54 == true && TPressed55 == true && TPressed56 == true && TPressed57 == true && TPressed58 == true && TPressed59 == true && TPressed60 == true && TPressed61 == true && TPressed62 == true && TPressed63 == true && TPressed64 == true && TPressed65 == true && TPressed66 == true && TPressed67 == true && TPressed68 == true && TPressed69 == true && TPressed70 == true && TPressed71 == true) {
		B3Block = true;
		if (block3Fx == false) {
			app->audio->PlayFxWithVolume(puzzleFX, 0, app->audio->fxvolume);
			block3Fx = true;
		}
	}

	//Pathfinding
	destination = app->map->WorldToMap(app->scene->player->position.x, app->scene->player->position.y+4);
	AuxPlayer = destination;
	AuxPlayer.x = destination.x+1;
	origin = app->map->WorldToMap(origin.x, origin.y);
	AuxEnemy = origin;
	if (checked == false) {
		Checker = AuxPlayer;
		Checker2 = AuxEnemy;
		checked = true;
	}
	destination = app->map->MapToWorld(destination.x+1, destination.y);
	if (Checker != AuxPlayer || Checker2 != AuxEnemy) {
		app->map->ResetPath();
		app->map->DestinationFound = false;
		checked = false;
	}

	//Player Health
	if (app->scene->player->lifeAux != 0) {
		app->scene->player->lifeAux++;
		if (app->scene->player->lifeAux >= 150) {
			app->scene->player->lifeAux = 0;
		}
	}

	//fading
	if (fade == true) {
		fading += 10;
		if (fading >= 255) {
			fading == 255;
			if (app->scene->player->lose == true) {
				app->Instance = app->scene->player->future_instance;
				app->scene->player->tpHouse = true;
				app->scene->player->lose = false;
				fade = false;
			}
			else {
				app->Instance = app->scene->player->future_instance;
				app->scene->player->tp = true;
				fade = false;
			}
			app->audio->Audio_Instance(app->Instance);
		}
	}
	if (fading > 0 && fade == false) {
		fading -= 10;
		if (fading <= 0) {
			fading = 0;
		}
	}
	app->render->DrawRectangle({ -3000,-3000,10000,10000 }, 0, 0, 0, fading);

	//Npc draw�E
	//Town
	if (app->Instance == 0) {
		app->render->DrawTexture(NPC3, 1110, 790, &N3T);
		//app->render->DrawTexture(LAPIS, 1110, 790, &LAPIS1);
	}
	//Tavern
	if (app->Instance == 1) {
		app->render->DrawTexture(NPC1, 148, -280, &N1T);
	}
	//Blacksmith
	if (app->Instance == 2) {
		app->render->DrawTexture(NPC2, 224, -710, &N2T);
	}

	if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP || app->input->GetKey(SDL_SCANCODE_SPACE)==KeyState::KEY_DOWN || app->input->controllers.A != 0 && !A_pressed)
	{
		switch (option)
		{
		case Scene::SELECTED::SAVEGAME:
			app->SaveGameRequest();
			app->audio->PlayFxWithVolume(select, 0, app->audio->fxvolume);
			break;
		case Scene::SELECTED::OPTIONS:
			app->audio->PlayFxWithVolume(select, 0, app->audio->fxvolume);
			options = true;
			break;
		case Scene::SELECTED::MAINMENU:
			app->audio->PlayFxWithVolume(play, 0, app->audio->fxvolume);
			isPaused = false;
			app->mainmenu->fading = 255;
			app->mainmenu->fading2 = 0;
			app->mainmenu->fadeIn = true;
			app->mainmenu->fadeOut = false;
			app->scene->player->active = false;
			app->entityManager->active = false;
			app->physics->active = false;
			app->scene->CanPlayerMove = false;
			app->mainmenu->active = true;
			app->scene->active = false;
			app->audio->PlayMusic("Assets/Soundtrack/Music/Rocky-Tundra-OST-Version.ogg");
			break;
		case Scene::SELECTED::EXIT:
			app->audio->PlayFxWithVolume(play, 0, app->audio->fxvolume);
			return false;
			break;
		case Scene::SELECTED::NONE:
			break;
		default:
			break;
		}
	}

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		if (!app->combat->InCombat) {
			isPaused = !isPaused;
			app->inventory->inventoryOn = false;
			app->audio->PlayFxWithVolume(select, 0, app->audio->fxvolume);
		}

	if (app->input->controllers.Start != 0 && !Start_pressed)
	{
		isPaused = !isPaused;
		Start_pressed = true;
		app->audio->PlayFxWithVolume(select, 0, app->audio->fxvolume);
	}
	else if (app->input->controllers.Start == 0)
	{
		Start_pressed = false;
	}

	if (isPaused)
	{
		Pause();
		if (options)
			app->mainmenu->Options(player->position.x-290, player->position.y - 180);
	}

	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		ret = false;

	// TODO 3: Make the inventoryOn bool true when pressing a key
	if (app->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN)
	{
		app->scene->player->Qmenu = false;
		if (!app->combat->InCombat) {
			app->inventory->inventoryOn = !app->inventory->inventoryOn;
			isPaused = false;
			app->audio->PlayFxWithVolume(select, 0, app->audio->fxvolume);
		}
	}
	if (app->input->controllers.Y != 0 && !Y_pressed)
	{
		if (!app->combat->InCombat) {
			app->inventory->inventoryOn = !app->inventory->inventoryOn;
			isPaused = false;
			app->audio->PlayFxWithVolume(select, 0, app->audio->fxvolume);
		}
	}
	// TODO 3: Some interface for the inventory
	if (app->inventory->inventoryOn)
	{
		CanPlayerMove = false;

		app->render->DrawRectangle({ app->scene->player->position.x + (-640 * app->ScalingMultiplier), app->scene->player->position.y + (-360 * app->ScalingMultiplier),1280 * app->ScalingMultiplier,720 * app->ScalingMultiplier }, 0, 0, 0, 80);

		//app->render->DrawRectangle(app->inventory->rect, 0, 0, 0, 50);
		SDL_Rect rect = { 0, 0, 640, 360 };
		app->render->DrawTexture(invTex, app->scene->player->position.x - 420, app->scene->player->position.y - 180, &rect);
		rect = { 0, 0, 20, 20 };
		app->render->DrawTexture(invArrowTex, app->scene->player->position.x - 260, app->scene->player->position.y - 44, &rect);
		if (app->inventory->nrOfHpPot > 0) {
			//char amount = static_cast<char>(app->inventory->nrOfHpPot);
			string amountStr = std::to_string(app->inventory->nrOfHpPot);
			const char* amount = amountStr.c_str();
			app->render->DrawText(60, 120, WF, "Healing potion", 16);
			app->render->DrawText(300, 120, WF, amount, 16);
		}
		if (app->inventory->nrOfMpPot > 0) {
			string amountStr = std::to_string(app->inventory->nrOfMpPot);
			const char* amount = amountStr.c_str();
			app->render->DrawText(60, 150, WF, "Mana potion", 16);
			app->render->DrawText(300, 150, WF, amount, 16);
		}
		if (app->inventory->nrOfAtkElx > 0) {
			string amountStr = std::to_string(app->inventory->nrOfAtkElx);
			const char* amount = amountStr.c_str();
			app->render->DrawText(60, 180, WF, "Attack elixir", 16);
			app->render->DrawText(300, 180, WF, amount, 16);
		}
		if (app->inventory->nrOfDefElx > 0) {
			string amountStr = std::to_string(app->inventory->nrOfDefElx);
			const char* amount = amountStr.c_str();
			app->render->DrawText(60, 210, WF, "Defense elixir", 16);
			app->render->DrawText(300, 210, WF, amount, 16);
		}
		if (app->inventory->necklace > 0) {
			string amountStr = std::to_string(app->inventory->necklace);
			const char* amount = amountStr.c_str();
			app->render->DrawText(60, 240, WF, "Old necklace", 16);
			app->render->DrawText(300, 240, WF, amount, 16);
		}
		if (app->inventory->manuscript > 0) {
			string amountStr = std::to_string(app->inventory->manuscript);
			const char* amount = amountStr.c_str();
			app->render->DrawText(60, 270, WF, "Manuscript", 16);
			app->render->DrawText(300, 270, WF, amount, 16);
		}
		if (app->inventory->book > 0) {
			string amountStr = std::to_string(app->inventory->book);
			const char* amount = amountStr.c_str();
			app->render->DrawText(60, 300, WF, "Zenith's history book", 16);
			app->render->DrawText(300, 300, WF, amount, 16);
		}

		// Show team stats
		app->render->DrawRectangle({ app->scene->player->position.x + 70, app->scene->player->position.y + (-360 * app->ScalingMultiplier),1280 * app->ScalingMultiplier,720 * app->ScalingMultiplier }, 0, 0, 0, 150);
		app->combat->ShowStatsInventory();
	}
	else
		CanPlayerMove = true;

	return true;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(trophyTex);
	app->tex->UnLoad(BGtexture);
	return true;
}
 // Pause Menu
bool Scene::Pause()
{
	if (!options)
	{
		// Oscurecer todo el fondo
		app->render->DrawRectangle({ player->position.x + (-640 * app->ScalingMultiplier), player->position.y + (-360 * app->ScalingMultiplier),1280 * app->ScalingMultiplier,720 * app->ScalingMultiplier }, 0, 0, 0, 80);
		// Fondo negro
		app->render->DrawRectangle({ player->position.x + (-16 * app->ScalingMultiplier), player->position.y + (-48 * app->ScalingMultiplier),64 * app->ScalingMultiplier,96 * app->ScalingMultiplier }, 0, 0, 0, 180);

		app->render->DrawText(640 / 2 - 32, 26 + 45 * 2, GF, "Save game", 16);
		app->render->DrawText(640 / 2 - 24, 26 + 65 * 2, GF, "Options", 16);
		app->render->DrawText(640 / 2 - 32, 26 + 85 * 2, GF, "Main menu", 16);
		app->render->DrawText(640 / 2 - 16, 26 + 105 * 2, GF, "Exit", 16);

		// Buttons and gamepad
		if (option == SELECTED::NONE) {
			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) {
				option = SELECTED::SAVEGAME;
				PlaySelectFx();
			}
		}
		else if (option == SELECTED::SAVEGAME) {
			app->render->DrawText(640 / 2 - 32, 26 + 45 * 2, WF, "Save game", 16);
			if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) {
				option = SELECTED::OPTIONS;
				PlaySelectFx();
			}
		}
		else if (option == SELECTED::OPTIONS) {
			app->render->DrawText(640 / 2 - 24, 26 + 65 * 2, WF, "Options", 16);
			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {
				option = SELECTED::SAVEGAME;
				PlaySelectFx();
			}
			if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) {
				option = SELECTED::MAINMENU;
				PlaySelectFx();
			}
		}
		else if (option == SELECTED::MAINMENU) {
			app->render->DrawText(640 / 2 - 32, 26 + 85 * 2, WF, "Main menu", 16);
			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {
				option = SELECTED::OPTIONS;
				PlaySelectFx();
			}
			if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) {
				option = SELECTED::EXIT;
				PlaySelectFx();
			}
		}
		else if (option == SELECTED::EXIT) {
			app->render->DrawText(640 / 2 - 16, 26 + 105 * 2, WF, "Exit", 16);
			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {
				option = SELECTED::MAINMENU;
				PlaySelectFx();
			}
		}
		if (option == SELECTED::NONE) {
			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) {
				option = SELECTED::SAVEGAME;
				PlaySelectFx();

			}
		}
		if (option == SELECTED::NONE) {
			if (app->input->controllers.DPADU > 0 || app->input->controllers.DPADD > 0) {
				moveCounter++;
				if (moveCounter >= MOVE_DELAY) {
					option = SELECTED::SAVEGAME;
					PlaySelectFx();
					moveCounter = 0;
				}

			}
		}
		else if (option == SELECTED::SAVEGAME) {
			app->render->DrawText(640 / 2 - 32, 26 + 45 * 2, WF, "Save game", 16);
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
			app->render->DrawText(640 / 2 - 24, 26 + 65 * 2, WF, "Options", 16);
			if (app->input->controllers.DPADU > 0) {
				moveCounter++;
				if (moveCounter >= MOVE_DELAY) {
				option = SELECTED::SAVEGAME;
				PlaySelectFx();
				moveCounter = 0;
				}
			}
			if (app->input->controllers.DPADD > 0) {
				moveCounter++;
				if (moveCounter >= MOVE_DELAY) {
				option = SELECTED::MAINMENU;
				PlaySelectFx();
				moveCounter = 0;
				}
			}
		}
		else if (option == SELECTED::MAINMENU) {
			app->render->DrawText(640 / 2 - 32, 26 + 85 * 2, WF, "Main menu", 16);
			if (app->input->controllers.DPADU > 0) {
				moveCounter++;
				if (moveCounter >= MOVE_DELAY) {
				option = SELECTED::OPTIONS;
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
			app->render->DrawText(640 / 2 - 16, 26 + 105 * 2, WF, "Exit", 16);
			if (app->input->controllers.DPADU > 0) {
				moveCounter++;
				if (moveCounter >= MOVE_DELAY) {
				option = SELECTED::MAINMENU;
				PlaySelectFx();
				moveCounter = 0;
				}
			}
		}
		if (option == SELECTED::NONE) {
			if (app->input->controllers.DPADU > 0 || app->input->controllers.DPADD > 0) {
				moveCounter++;
				if (moveCounter >= MOVE_DELAY) {
				option = SELECTED::SAVEGAME;
				PlaySelectFx();
				moveCounter = 0;
				}
			}
		}
	}
	return true;
}

void Scene::PlaySelectFx()
{
	if (alreadyChangeFX == false) {
		app->audio->PlayFxWithVolume(change, 0, app->audio->fxvolume);
		alreadyChangeFX = true;
	}
}

void Scene::RestartCave()
{
	if (Reload == false) {
		app->scene->fade = true;
		app->scene->player->Teleport_Point(6, app->scene->player->tp11);
	}

	TPressed1 = false;
	TPressed2 = false;
	TPressed3 = true;
	TPressed4 = false;
	TPressed5 = false;
	TPressed6 = false;
	TPressed7 = true;
	TPressed8 = false;
	TPressed9 = false;
	TPressed10 = false;
	TPressed11 = false;
	TPressed12 = false;
	TPressed13 = false;
	TPressed14 = false;
	TPressed15 = false;
	TPressed16 = false;
	TPressed17 = true;
	TPressed18 = false;
	TPressed19 = false;
	TPressed20 = false;
	TPressed21 = false;
	TPressed22 = false;
	TPressed23 = false;
	TPressed24 = false;
	TPressed25 = true;
	TPressed26 = false;
	TPressed27 = false;
	TPressed28 = false;
	TPressed29 = false;
	TPressed30 = false;
	TPressed31 = false;
	TPressed32 = false;
	TPressed33 = false;
	TPressed34 = false;
	TPressed35 = false;
	TPressed36 = false;
	TPressed37 = false;
	TPressed38 = false;
	TPressed39 = false;
	TPressed40 = false;
	TPressed41 = false;
	TPressed42 = false;
	TPressed43 = false;
	TPressed44 = true;
	TPressed45 = false;
	TPressed46 = false;
	TPressed47 = false;
	TPressed48 = true;
	TPressed49 = false;
	TPressed50 = false;
	TPressed51 = false;
	TPressed52 = false;
	TPressed53 = false;
	TPressed54 = false;
	TPressed55 = false;
	TPressed56 = false;
	TPressed57 = false;
	TPressed58 = false;
	TPressed59 = false;
	TPressed60 = false;
	TPressed61 = true;
	TPressed62 = false;
	TPressed63 = false;
	TPressed64 = false;
	TPressed65 = false;
	TPressed66 = false;
	TPressed67 = false;
	TPressed68 = false;
	TPressed69 = true;
	TPressed70 = false;
	TPressed71 = false;

	TBroken1 = false;
	TBroken2 = false;
	TBroken3 = true;
	TBroken4 = false;
	TBroken5 = false;
	TBroken6 = false;
	TBroken7 = true;
	TBroken8 = false;
	TBroken9 = false;
	TBroken10 = false;
	TBroken11 = false;
	TBroken12 = false;
	TBroken13 = false;
	TBroken14 = false;
	TBroken15 = false;
	TBroken16 = false;
	TBroken17 = true;
	TBroken18 = false;
	TBroken19 = false;
	TBroken20 = false;
	TBroken21 = false;
	TBroken22 = false;
	TBroken23 = false;
	TBroken24 = false;
	TBroken25 = true;
	TBroken26 = false;
	TBroken27 = false;
	TBroken28 = false;
	TBroken29 = false;
	TBroken30 = false;
	TBroken31 = false;
	TBroken32 = false;
	TBroken33 = false;
	TBroken34 = false;
	TBroken35 = false;
	TBroken36 = false;
	TBroken37 = false;
	TBroken38 = false;
	TBroken39 = false;
	TBroken40 = false;
	TBroken41 = false;
	TBroken42 = false;
	TBroken43 = false;
	TBroken44 = true;
	TBroken45 = false;
	TBroken46 = false;
	TBroken47 = false;
	TBroken48 = true;
	TBroken49 = false;
	TBroken50 = false;
	TBroken51 = false;
	TBroken52 = false;
	TBroken53 = false;
	TBroken54 = false;
	TBroken55 = false;
	TBroken56 = false;
	TBroken57 = false;
	TBroken58 = false;
	TBroken59 = false;
	TBroken60 = false;
	TBroken61 = true;
	TBroken62 = false;
	TBroken63 = false;
	TBroken64 = false;
	TBroken65 = false;
	TBroken66 = false;
	TBroken67 = false;
	TBroken68 = false;
	TBroken69 = true;
	TBroken70 = false;
	TBroken71 = false;

	B1Block = false;
	B2Block = false;
	B3Block = false;
	Block1->body->SetActive(true);
	Block2->body->SetActive(true);
	Block2->body->SetActive(true);
}


bool Scene::LoadState(pugi::xml_node& data) {

	//Lake Puzzle
	rock_1[0] = data.child("LakePuzzle").attribute("Rock_1_x").as_int();
	rock_1[1] = data.child("LakePuzzle").attribute("Rock_1_y").as_int();
	rock_2[0] = data.child("LakePuzzle").attribute("Rock_2_x").as_int();
	rock_2[1] = data.child("LakePuzzle").attribute("Rock_2_y").as_int();
	rock_3[0] = data.child("LakePuzzle").attribute("Rock_3_x").as_int();
	rock_3[1] = data.child("LakePuzzle").attribute("Rock_3_y").as_int();
	rock_4[0] = data.child("LakePuzzle").attribute("Rock_4_x").as_int();
	rock_4[1] = data.child("LakePuzzle").attribute("Rock_4_y").as_int();
	rock_5[0] = data.child("LakePuzzle").attribute("Rock_5_x").as_int();
	rock_5[1] = data.child("LakePuzzle").attribute("Rock_5_y").as_int();
	rock_6[0] = data.child("LakePuzzle").attribute("Rock_6_x").as_int();
	rock_6[1] = data.child("LakePuzzle").attribute("Rock_6_y").as_int();
	stage = data.child("LakePuzzle").attribute("Stage").as_int();
	Puzzle_Lake();

	//Forest Puzzle
	P1Active = data.child("ForestPuzzle").attribute("Tower_1").as_bool();
	P2Active = data.child("ForestPuzzle").attribute("Tower_2").as_bool();
	P3Active = data.child("ForestPuzzle").attribute("Tower_3").as_bool();


	return true;
}

bool Scene::SaveState(pugi::xml_node& data) {

	//Lake Puzzle
	pugi::xml_node LakePuzzle = data.append_child("LakePuzzle");
	LakePuzzle.append_attribute("Rock_1_x") = rock_1[0];
	LakePuzzle.append_attribute("Rock_1_y") = rock_1[1];
	LakePuzzle.append_attribute("Rock_2_x") = rock_2[0];
	LakePuzzle.append_attribute("Rock_2_y") = rock_2[1];
	LakePuzzle.append_attribute("Rock_3_x") = rock_3[0];
	LakePuzzle.append_attribute("Rock_3_y") = rock_3[1];
	LakePuzzle.append_attribute("Rock_4_x") = rock_4[0];
	LakePuzzle.append_attribute("Rock_4_y") = rock_4[1];
	LakePuzzle.append_attribute("Rock_5_x") = rock_5[0];
	LakePuzzle.append_attribute("Rock_5_y") = rock_5[1];
	LakePuzzle.append_attribute("Rock_6_x") = rock_6[0];
	LakePuzzle.append_attribute("Rock_6_y") = rock_6[1];
	LakePuzzle.append_attribute("Stage") = stage;

	//Forest
	pugi::xml_node ForestPuzzle = data.append_child("ForestPuzzle");
	data.child("ForestPuzzle").append_attribute("Tower_1") = P1Active;
	data.child("ForestPuzzle").append_attribute("Tower_2") = P2Active;
	data.child("ForestPuzzle").append_attribute("Tower_3") = P3Active;

	return true;
}

void Scene::Puzzle_Lake() {

	//Draw Coliders
	colliderPuzzle1->GetPosition(rock_1[0], rock_1[1]);
	colliderPuzzle2->GetPosition(rock_2[0], rock_2[1]);
	colliderPuzzle3->GetPosition(rock_3[0], rock_3[1]);
	colliderPuzzle4->GetPosition(rock_4[0], rock_4[1]);
	colliderPuzzle5->GetPosition(rock_5[0], rock_5[1]);
	colliderPuzzle6->GetPosition(rock_6[0], rock_6[1]);

	//Draw textures

	if (Pressed1 == true && Pressed2 == true && Pressed3 == true) {
		colliderPuzzle2Block->body->SetActive(false);
	}
	else {
		app->render->DrawTexture(Puzzle2Wall, 3396, -1444 - 32);
		colliderPuzzle2Block->body->SetActive(true);
	}

	if (Pressed1 == true) {
		app->render->DrawTexture(Pressed, 3040, -449);

		if (pressed1Fx == false) {
			app->audio->PlayFxWithVolume(puzzleFX, 0, app->audio->fxvolume);
			pressed1Fx = true;
		}
	}
	if (Pressed1 == false) {
		app->render->DrawTexture(Unpressed, 3040, -449);
		pressed1Fx = false;
	}
	if (Pressed2 == true) {
		app->render->DrawTexture(Pressed, 3008, -705);
		if (pressed2Fx == false) {
			app->audio->PlayFxWithVolume(puzzleFX, 0, app->audio->fxvolume);
			pressed2Fx = true;
		}
	}
	if (Pressed2 == false) {
		app->render->DrawTexture(Unpressed, 3008, -705);
		pressed2Fx = false;
	}
	if (Pressed3 == true) {
		app->render->DrawTexture(Pressed, 4000, -1153);
		if (pressed3Fx == false) {
			app->audio->PlayFxWithVolume(puzzleFX, 0, app->audio->fxvolume);
			pressed3Fx = true;
		}
	}
	if (Pressed3 == false) {
		app->render->DrawTexture(Unpressed, 4000, -1153);
		pressed3Fx = false;
	}

	if ((rock_4[0] >= 3009 && rock_4[0] <= 3071 && rock_4[1] >= -480 && rock_4[1] <= -433) || (rock_5[0] >= 3009 && rock_5[0] <= 3071 && rock_5[1] >= -480 && rock_5[1] <= -433) || (rock_6[0] >= 3009 && rock_6[0] <= 3071 && rock_6[1] >= -480 && rock_6[1] <= -433)) {
		Pressed1 = true;
	}
	else {
		Pressed1 = false;
	}
	if ((rock_4[0] >= 2978 && rock_4[0] <= 3039 && rock_4[1] >= -736 && rock_4[1] <= -689) || (rock_5[0] >= 2978 && rock_5[0] <= 3039 && rock_5[1] >= -736 && rock_5[1] <= -689) || (rock_6[0] >= 2978 && rock_6[0] <= 3039 && rock_6[1] >= -736 && rock_6[1] <= -689)) {
		Pressed2 = true;
	}
	else {
		Pressed2 = false;
	}
	if ((rock_4[0] >= 3969 && rock_4[0] <= 4031 && rock_4[1] >= -1184 && rock_4[1] <= -1137) || (rock_5[0] >= 3969 && rock_5[0] <= 4031 && rock_5[1] >= -1184 && rock_5[1] <= -1137) || (rock_6[0] >= 3969 && rock_6[0] <= 4031 && rock_6[1] >= -1184 && rock_6[1] <= -1137)) {
		Pressed3 = true;
	}
	else {
		Pressed3 = false;
	}

	//Draw stones order
	app->render->DrawTexture(Stone, rock_4[0], rock_4[1]);
	app->render->DrawTexture(Stone, rock_5[0], rock_5[1]);
	app->render->DrawTexture(Stone, rock_6[0], rock_6[1]);

	if (stage == 0) {
		app->render->DrawTexture(Stone, rock_1[0], rock_1[1]);
		app->render->DrawTexture(Stone, rock_2[0], rock_2[1]);
		app->render->DrawTexture(Stone, rock_3[0], rock_3[1]);
	}
	if (stage == 1 && colliderPuzzle1->body->IsActive() == false) {
		app->render->DrawTexture(StoneDeep, rock_1[0], rock_1[1]);
		app->render->DrawTexture(Stone, rock_2[0], rock_2[1]);
		app->render->DrawTexture(Stone, rock_3[0], rock_3[1]);
	}
	if (stage == 1 && colliderPuzzle2->body->IsActive() == false) {
		app->render->DrawTexture(StoneDeep, rock_2[0], rock_2[1]);
		app->render->DrawTexture(Stone, rock_1[0], rock_1[1]);
		app->render->DrawTexture(Stone, rock_3[0], rock_3[1]);
	}
	if (stage == 1 && colliderPuzzle3->body->IsActive() == false) {
		app->render->DrawTexture(StoneDeep, rock_3[0], rock_3[1]);
		app->render->DrawTexture(Stone, rock_1[0], rock_1[1]);
		app->render->DrawTexture(Stone, rock_2[0], rock_2[1]);
	}
	if (stage == 2 && colliderPuzzle1->body->IsActive() == false && colliderPuzzle2->body->IsActive() == false) {
		app->render->DrawTexture(Stone, rock_1[0], rock_1[1]);
		app->render->DrawTexture(Stone, rock_2[0], rock_2[1]);
		app->render->DrawTexture(Stone, rock_3[0], rock_3[1]);
	}
	if (stage == 2 && colliderPuzzle1->body->IsActive() == false && colliderPuzzle3->body->IsActive() == false) {
		app->render->DrawTexture(StoneDeep, rock_3[0], rock_3[1]);
		app->render->DrawTexture(StoneDeep, rock_1[0], rock_1[1]);
		app->render->DrawTexture(Stone, rock_2[0], rock_2[1]);
	}
	if (stage == 2 && colliderPuzzle2->body->IsActive() == false && colliderPuzzle3->body->IsActive() == false) {
		app->render->DrawTexture(StoneDeep, rock_3[0], rock_3[1]);
		app->render->DrawTexture(StoneDeep, rock_2[0], rock_2[1]);
		app->render->DrawTexture(Stone, rock_1[0], rock_1[1]);
	}
	if (stage == 3) {
		app->render->DrawTexture(StoneDeep, rock_1[0], rock_1[1]);
		app->render->DrawTexture(StoneDeep, rock_2[0], rock_2[1]);
		app->render->DrawTexture(StoneDeep, rock_3[0], rock_3[1]);
	}

	//Lake puzzle 1 stage 1
	if (((rock_1[0] <= 3452 && rock_1[0] >= 3415 && rock_1[1] <= 250) || (rock_2[0] <= 3452 && rock_2[0] >= 3415 && rock_2[1] <= 250) || (rock_3[0] <= 3452 && rock_3[0] >= 3415 && rock_3[1] <= 250)) && stage == 0) {
		colliderPuzzle1Block->body->SetActive(false);
	}
	else if (stage == 0) {
		colliderPuzzle1Block->body->SetActive(true);
	}

	if (rock_1[1] < 210 && stage == 0) {
		colliderPuzzle1->body->SetActive(false);
		colliderPuzzle1Block->body->SetTransform({ 107.5,4.5 }, 0);
		stage++;
	}
	if (rock_2[1] < 210 && stage == 0) {
		colliderPuzzle2->body->SetActive(false);
		colliderPuzzle1Block->body->SetTransform({ 107.5,4.5 }, 0);
		stage++;
	}
	if (rock_3[1] < 210 && stage == 0) {
		colliderPuzzle3->body->SetActive(false);
		colliderPuzzle1Block->body->SetTransform({ 107.5,4.5 }, 0);
		stage++;
	}

	//Lake puzzle 1 stage 2

	if (((rock_1[0] <= 3452 && rock_1[0] >= 3415 && rock_1[1] <= 208) || (rock_2[0] <= 3452 && rock_2[0] >= 3415 && rock_2[1] <= 208) || (rock_3[0] <= 3452 && rock_3[0] >= 3415 && rock_3[1] <= 208)) && stage == 1) {
		colliderPuzzle1Block->body->SetActive(false);
	}
	else if (stage == 1) {
		colliderPuzzle1Block->body->SetActive(true);
	}

	if (rock_1[1] < 180 && stage == 1) {
		colliderPuzzle1->body->SetActive(false);
		colliderPuzzle1Block->body->SetTransform({ 107.5,3.5 }, 0);
		stage++;
	}
	if (rock_2[1] < 180 && stage == 1) {
		colliderPuzzle2->body->SetActive(false);
		colliderPuzzle1Block->body->SetTransform({ 107.5,3.5 }, 0);
		stage++;
	}
	if (rock_3[1] < 180 && stage == 1) {
		colliderPuzzle3->body->SetActive(false);
		colliderPuzzle1Block->body->SetTransform({ 107.5,3.5 }, 0);
		stage++;
	}
	//Lake puzzle 1 stage 3

	if (((rock_1[0] <= 3452 && rock_1[0] >= 3415 && rock_1[1] <= 175) || (rock_2[0] <= 3452 && rock_2[0] >= 3415 && rock_2[1] <= 175) || (rock_3[0] <= 3452 && rock_3[0] >= 3415 && rock_3[1] <= 175)) && stage == 2) {
		colliderPuzzle1Block->body->SetActive(false);
	}
	else if (stage == 2) {
		colliderPuzzle1Block->body->SetActive(true);
	}

	if (rock_1[1] < 150 && stage == 2) {
		colliderPuzzle1->body->SetActive(false);
		stage++;
	}
	if (rock_2[1] < 150 && stage == 2) {
		colliderPuzzle2->body->SetActive(false);
		stage++;
	}
	if (rock_3[1] < 150 && stage == 2) {
		colliderPuzzle3->body->SetActive(false);
		stage++;
	}

	return ;
}