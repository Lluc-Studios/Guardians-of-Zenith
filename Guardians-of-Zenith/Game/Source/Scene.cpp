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
	

	for (pugi::xml_node sawNode = config.child("saw"); sawNode; sawNode = sawNode.next_sibling("saw"))
	{
		Saw* saw = (Saw*)app->entityManager->CreateEntity(EntityType::SAW);
		saw->parameters = sawNode;
	}

	//L02: DONE 3: Instantiate the player using the entity manager
	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
	player->parameters = config.child("player");

	//Item* trophy =(Item*)app->entityManager->CreateEntity(EntityType::ITEM);
	//trophy->parameters = config.child("trophy");

	for (pugi::xml_node en1Node = config.child("enemy"); en1Node; en1Node = en1Node.next_sibling("enemy")) {
		Enemy* grounded_e = (Enemy*)app->entityManager->CreateEntity(EntityType::ENEMY);
		grounded_e->parameters = en1Node;
	}

	for (pugi::xml_node en2Node = config.child("enemy2"); en2Node; en2Node = en2Node.next_sibling("enemy2")) {
		Enemy2* flying_e = (Enemy2*)app->entityManager->CreateEntity(EntityType::ENEMY2);
		flying_e->parameters = en2Node;
	}

	//for (pugi::xml_node tpNode = config.child("teleport"); tpNode; tpNode = tpNode.next_sibling("teleport"))
	//{
	//	Teleport* tp = (Teleport*)app->entityManager->CreateEntity(EntityType::TELEPORT);
	//	tp->parameters = tpNode;
	//}

	villageMusic = app->audio->LoadFx("Assets/Soundtrack/Music/Guidance Island OST Version.ogg");

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
	invTex = app->tex->Load("Assets/Textures/UI_inventory_640x360.png");
	invArrowTex = app->tex->Load("Assets/Textures/UI_inventory_arrow_20x20.png");

	NPC1 = app->tex->Load("Assets/Entities/NPC/npc_1.png");
	NPC2 = app->tex->Load("Assets/Entities/NPC/npc_2.png");
	NPC3 = app->tex->Load("Assets/Entities/NPC/npc_3.png");
	LAPIS = app->tex->Load("Assets/Entities/Characters/Lapis_Directions.png");
	Stone = app->tex->Load("Assets/Textures/Puzzles/Stone.png");
	Tp = app->tex->Load("Assets/Textures/MonolitoTP.png");
	Unpressed = app->tex->Load("Assets/Textures/Puzzles/Unpressed.png");
	Pressed = app->tex->Load("Assets/Textures/Puzzles/Pressed.png");
	Puzzle2Wall = app->tex->Load("Assets/Textures/Puzzles/Door_water_dungeon.png");
	Slime = app->tex->Load("Assets/Entities/enemies/FrogBasic.png");
	Lily = app->tex->Load("Assets/Entities/enemies/LilyBasic.png");
	Naiadon = app->tex->Load("Assets/Entities/enemies/NaiadonBasic.png");
	DefaultTile = app->tex->Load("Assets/Textures/Puzzles/CrackedGround_Default.png");
	BrokenTile = app->tex->Load("Assets/Textures/Puzzles/CrackedGround_Broken.png");

	//Lake enemies
	
	Slime1 = app->physics->CreateRectangle(3400, -20, 32, 32, STATIC);
	Slime1->ctype = ColliderType::ENEMYSLIME;
	Slime2 = app->physics->CreateRectangle(3624, 44, 32, 32, STATIC);
	Slime2->ctype = ColliderType::ENEMYSLIME;
	Slime3 = app->physics->CreateRectangle(3624, -404, 32, 32, STATIC);
	Slime3->ctype = ColliderType::ENEMYSLIME;
	Slime4 = app->physics->CreateRectangle(3176, -372, 32, 32, STATIC);
	Slime4->ctype = ColliderType::ENEMYSLIME;
	Slime5 = app->physics->CreateRectangle(3208, -212, 32, 32, STATIC);
	Slime5->ctype = ColliderType::ENEMYSLIME;
	Slime6 = app->physics->CreateRectangle(2952, -820, 32, 32, STATIC);
	Slime6->ctype = ColliderType::ENEMYSLIME;
	Slime7 = app->physics->CreateRectangle(2920, -1140, 32, 32, STATIC);
	Slime7->ctype = ColliderType::ENEMYSLIME;
	Slime8 = app->physics->CreateRectangle(3240, -1140, 32, 32, STATIC);
	Slime8->ctype = ColliderType::ENEMYSLIME;
	Slime9 = app->physics->CreateRectangle(3848, -1172, 32, 32, STATIC);
	Slime9->ctype = ColliderType::ENEMYSLIME;
	Slime10 = app->physics->CreateRectangle(3688, -1012, 32, 32, STATIC);
	Slime10->ctype = ColliderType::ENEMYSLIME;
	Slime11 = app->physics->CreateRectangle(3816, -724, 32, 32, STATIC);
	Slime11->ctype = ColliderType::ENEMYSLIME;

	Lily1 = app->physics->CreateRectangle(3624, -244, 64, 43, STATIC);
	Lily1->ctype = ColliderType::ENEMYLILY;
	Lily2 = app->physics->CreateRectangle(3784, -852, 64, 43, STATIC);
	Lily2->ctype = ColliderType::ENEMYLILY;
	Lily3 = app->physics->CreateRectangle(3432, -852, 64, 43, STATIC);
	Lily3->ctype = ColliderType::ENEMYLILY;
	Lily4 = app->physics->CreateRectangle(3240, -1012, 64, 43, STATIC);
	Lily4->ctype = ColliderType::ENEMYLILY;
	Lily5 = app->physics->CreateRectangle(3016, -1012, 64, 43, STATIC);
	Lily5->ctype = ColliderType::ENEMYLILY;

	Naiadon1 = app->physics->CreateRectangle(3459, -1814, 118, 125, STATIC);
	Naiadon1->ctype = ColliderType::ENEMYNAIADON;

	//Lake puzzle

	MonoTP = app->physics->CreateRectangle(3537, 448, 32, 64, STATIC);
	MonoTP->ctype = ColliderType::TOWNMONOLITH;

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

	//if (Music == false) {
	//	app->audio->PlayFx(villageMusic, 10);
	//	Music = true;
	//}

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
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {
		int god = 100000;
		if (app->scene->player->laurea.def == 100000) {
			god = 100;
		}
		app->scene->player->laurea.def = god;
		app->scene->player->lapis.def = god;
		app->scene->player->lucca.def = god;
		app->scene->player->laurea.atk = god;
		app->scene->player->lapis.atk = god;
		app->scene->player->lucca.atk = god;
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

	//Camera on player
	/*app->render->camera.x = -1 * (player->position.x * app->win->GetScale() - app->render->camera.w / 2);
	app->render->camera.y = -1 * (player->position.y * app->win->GetScale() - app->render->camera.h / 2);*/

	// Draw map
	if (app->Instance == 0) {
		app->map->Draw();
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
		app->lakedungeon->Draw();
	}
	//if (app->Instance == 5) {
	//	app->forestdungeon->Draw();
	//  app->audio->PlayMusic("Assets/Soundtrack/Music/Azalea_Forest_OST_Version.ogg");
	//}
	if (app->Instance == 6) {
		app->cavedungeon->Draw();
	}
	if (app->Instance == 7) {
		app->monolith->Draw();
	}

	//Lake dungeon
	colliderPuzzle1->GetPosition(x1, y1);
	colliderPuzzle2->GetPosition(x2, y2);
	colliderPuzzle3->GetPosition(x3, y3);
	colliderPuzzle4->GetPosition(x4, y4);
	colliderPuzzle5->GetPosition(x5, y5);
	colliderPuzzle6->GetPosition(x6, y6);

	//Draw enemies lake

	app->render->DrawTexture(Slime, 3384, -36);
	app->render->DrawTexture(Slime, 3608, 28);
	app->render->DrawTexture(Slime, 3608, -420);
	app->render->DrawTexture(Slime, 3160, -388);
	app->render->DrawTexture(Slime, 3192, -228);
	app->render->DrawTexture(Slime, 2936, -836);
	app->render->DrawTexture(Slime, 2904, -1156);
	app->render->DrawTexture(Slime, 3224, -1156);
	app->render->DrawTexture(Slime, 3832, -1188);
	app->render->DrawTexture(Slime, 3672, -1028);
	app->render->DrawTexture(Slime, 3800, -740);

	app->render->DrawTexture(Lily, 3592, -265);
	app->render->DrawTexture(Lily, 3752, -873);
	app->render->DrawTexture(Lily, 3400, -873);
	app->render->DrawTexture(Lily, 3208, -1033);
	app->render->DrawTexture(Lily, 2984, -1033);

	app->render->DrawTexture(Naiadon, 3400, -1876);


	//Draw textures
	app->render->DrawTexture(Tp, 3520, 415);

	if (Pressed1 == true && Pressed2 == true && Pressed3 == true) {
		colliderPuzzle2Block->body->SetActive(false);
	}
	else {
		app->render->DrawTexture(Puzzle2Wall, 3396, -1444);
		colliderPuzzle2Block->body->SetActive(true);
	}

	if (Pressed1 == true) {
		app->render->DrawTexture(Pressed, 3040, -449);
	}
	if (Pressed1 == false) {
		app->render->DrawTexture(Unpressed, 3040, -449);
	}
	if (Pressed2 == true) {
		app->render->DrawTexture(Pressed, 3008, -705);
	}
	if (Pressed2 == false) {
		app->render->DrawTexture(Unpressed, 3008, -705);
	}
	if (Pressed3 == true) {
		app->render->DrawTexture(Pressed, 4000, -1153);
	}
	if (Pressed3 == false) {
		app->render->DrawTexture(Unpressed, 4000, -1153);
	}

	if ((x4 >=3009 && x4 <= 3071 && y4 >= -480 && y4 <=-433) || (x5 >= 3009 && x5 <= 3071 && y5 >= -480 && y5 <= -433) || (x6 >= 3009 && x6 <= 3071 && y6 >= -480 && y6 <= -433)) {
		Pressed1 = true;
	}
	else {
		Pressed1 = false;
	}
	if ((x4 >= 2978 && x4 <= 3039 && y4 >= -736 && y4 <= -689) || (x5 >= 2978 && x5 <= 3039 && y5 >= -736 && y5 <= -689) || (x6 >= 2978 && x6 <= 3039 && y6 >= -736 && y6 <= -689)) {
		Pressed2 = true;
	}
	else {
		Pressed2 = false;
	}
	if ((x4 >= 3969 && x4 <= 4031 && y4 >= -1184 && y4 <= -1137) || (x5 >= 3969 && x5 <= 4031 && y5 >= -1184 && y5 <= -1137) || (x6 >= 3969 && x6 <= 4031 && y6 >= -1184 && y6 <= -1137)) {
		Pressed3 = true;
	}
	else {
		Pressed3 = false;
	}

	//Draw stones order

	app->render->DrawTexture(Stone, x4, y4);
	app->render->DrawTexture(Stone, x5, y5);
	app->render->DrawTexture(Stone, x6, y6);

	if (stage == 0) {
		app->render->DrawTexture(Stone, x1, y1);
		app->render->DrawTexture(Stone, x2, y2);
		app->render->DrawTexture(Stone, x3, y3);
	}
	if (stage == 1 && colliderPuzzle1->body->IsActive() == false) {
		app->render->DrawTexture(Stone, x1, y1);
		app->render->DrawTexture(Stone, x2, y2);
		app->render->DrawTexture(Stone, x3, y3);
	}
	if (stage == 1 && colliderPuzzle2->body->IsActive() == false) {
		app->render->DrawTexture(Stone, x2, y2);
		app->render->DrawTexture(Stone, x1, y1);
		app->render->DrawTexture(Stone, x3, y3);
	}
	if (stage == 1 && colliderPuzzle3->body->IsActive() == false) {
		app->render->DrawTexture(Stone, x3, y3);
		app->render->DrawTexture(Stone, x1, y1);
		app->render->DrawTexture(Stone, x2, y2);
	}
	if (stage == 2 && colliderPuzzle1->body->IsActive() == false && colliderPuzzle2->body->IsActive() == false) {
		app->render->DrawTexture(Stone, x1, y1);
		app->render->DrawTexture(Stone, x2, y2);
		app->render->DrawTexture(Stone, x3, y3);
	}
	if (stage == 2 && colliderPuzzle1->body->IsActive() == false && colliderPuzzle3->body->IsActive() == false) {
		app->render->DrawTexture(Stone, x3, y3);
		app->render->DrawTexture(Stone, x1, y1);
		app->render->DrawTexture(Stone, x2, y2);
	}
	if (stage == 2 && colliderPuzzle2->body->IsActive() == false && colliderPuzzle3->body->IsActive() == false) {
		app->render->DrawTexture(Stone, x3, y3);
		app->render->DrawTexture(Stone, x2, y2);
		app->render->DrawTexture(Stone, x1, y1);
	}
	if (stage == 3) {
		app->render->DrawTexture(Stone, x1, y1);
		app->render->DrawTexture(Stone, x2, y2);
		app->render->DrawTexture(Stone, x3, y3);
	}

	//Lake puzzle 1 stage 1

	if (((x1 <= 3452 && x1 >= 3415 && y1 <=250) || (x2 <= 3452 && x2 >= 3415 && y2 <= 250) || (x3 <= 3452 && x3 >= 3415 && y3 <= 250)) && stage == 0) {
		colliderPuzzle1Block->body->SetActive(false);
	}
	else if (stage == 0){
		colliderPuzzle1Block->body->SetActive(true);
	}

	if (y1 < 210 && stage == 0) {
		colliderPuzzle1->body->SetActive(false);
		colliderPuzzle1Block->body->SetTransform({ 107.5,4.5 }, 0);
		stage++;
	}
	if (y2 < 210 && stage == 0) {
		colliderPuzzle2->body->SetActive(false);
		colliderPuzzle1Block->body->SetTransform({ 107.5,4.5 }, 0);
		stage++;
	}
	if (y3 < 210 && stage == 0) {
		colliderPuzzle3->body->SetActive(false);
		colliderPuzzle1Block->body->SetTransform({ 107.5,4.5 }, 0);
		stage++;
	}

	//Lake puzzle 1 stage 2

	if (((x1 <= 3452 && x1 >= 3415 && y1 <= 208) || (x2 <= 3452 && x2 >= 3415 && y2 <= 208) || (x3 <= 3452 && x3 >= 3415 && y3 <= 208)) && stage == 1) {
		colliderPuzzle1Block->body->SetActive(false);
	}
	else if (stage == 1) {
		colliderPuzzle1Block->body->SetActive(true);
	}

	if (y1 < 180 && stage == 1) {
		colliderPuzzle1->body->SetActive(false);
		colliderPuzzle1Block->body->SetTransform({ 107.5,3.5 }, 0);
		stage++;
	}
	if (y2 < 180 && stage == 1) {
		colliderPuzzle2->body->SetActive(false);
		colliderPuzzle1Block->body->SetTransform({ 107.5,3.5 }, 0);
		stage++;
	}
	if (y3 < 180 && stage == 1) {
		colliderPuzzle3->body->SetActive(false);
		colliderPuzzle1Block->body->SetTransform({ 107.5,3.5 }, 0);
		stage++;
	}
	//Lake puzzle 1 stage 3

	if (((x1 <= 3452 && x1 >= 3415 && y1 <= 175) || (x2 <= 3452 && x2 >= 3415 && y2 <= 175) || (x3 <= 3452 && x3 >= 3415 && y3 <= 175)) && stage == 2) {
		colliderPuzzle1Block->body->SetActive(false);
	}
	else if (stage == 2) {
		colliderPuzzle1Block->body->SetActive(true);
	}

	if (y1 < 150 && stage == 2) {
		colliderPuzzle1->body->SetActive(false);
		stage++;
	}
	if (y2 < 150 && stage == 2) {
		colliderPuzzle2->body->SetActive(false);
		stage++;
	}
	if (y3 < 150 && stage == 2) {
		colliderPuzzle3->body->SetActive(false);
		stage++;
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
	if (app->scene->player->position.x >= -1300 && app->scene->player->position.x <= -1258 && app->scene->player->position.y >=2005 && app->scene->player->position.y <= 2040) TPressed5 = true;
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

			if (app->scene->player->lose = true) {
				app->Instance = app->scene->player->future_instance;
				app->scene->player->tp = true;
				app->scene->player->lose = false;
				fade = false;
			}
			else {
				app->Instance = app->scene->player->future_instance;
				app->scene->player->tp = true;
				fade = false;
			}
			switch (app->scene->player->future_instance)
			{
			case 0:
				LOG("Music_Town");
				app->audio->PlayMusic("Assets/Soundtrack/Music/Guidance-Island-OST-Version.ogg");
				break;
			case 1:
				LOG("Music_Tavern");
				app->audio->PlayMusic("Assets/Soundtrack/Music/Guardia-Millennial-Fair.ogg");
				break;
			case 2:
				LOG("Music_Blacksmith");
				app->audio->PlayMusic("Assets/Soundtrack/Music/Blacksmith.ogg");
				break;
			case 3:
				LOG("Music_House");
				app->audio->PlayMusic("Assets/Soundtrack/Music/Tranquil-Days.ogg");
				break;
			case 4:
				LOG("Music_LakeDaugeon");
				app->audio->PlayMusic("Assets/Soundtrack/Music/The-Riverlands.ogg");
				break;
			case 5:
				LOG("Music_ForestDaugeon");
				app->audio->PlayMusic("Assets/Soundtrack/Music/Azalea-Forest-OST-Version.ogg");
				break;
			case 6:
				LOG("Music_CaveDaugeon");
				app->audio->PlayMusic("Assets/Soundtrack/Music/Ruined-World.ogg");
				break;
			case 7:
				LOG("Music_Monolith");
				app->audio->PlayMusic("Assets/Soundtrack/Music/soul-loop.ogg");
				break;
			}

		}

	}
	if (fading > 0 && fade == false) {
		fading -= 10;
		if (fading <= 0) {
			fading = 0;
		}
	}
	app->render->DrawRectangle({ -3000,-3000,10000,10000 }, 0, 0, 0, fading);

	//Npc draw
	app->render->DrawTexture(NPC1, 148, -280, &N1T);
	app->render->DrawTexture(NPC2, 224, -710, &N2T);
	app->render->DrawTexture(NPC3, 483, 545, &N3T);
	app->render->DrawTexture(LAPIS, 1110, 790, &LAPIS1);

	if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
	{
		switch (option)
		{
		case Scene::SELECTED::SAVEGAME:
			app->SaveGameRequest();
			break;
		case Scene::SELECTED::OPTIONS:
			options = true;
			break;
		case Scene::SELECTED::MAINMENU:
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
		}

	if (app->input->controllers.Start != 0 && !Start_pressed)
	{
		isPaused = !isPaused;
		Start_pressed = true;
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
		if (!app->combat->InCombat) {
			app->inventory->inventoryOn = !app->inventory->inventoryOn;
			isPaused = false;
		}
	}

	// TODO 3: Some interface for the inventory
	if (app->inventory->inventoryOn)
	{
		app->render->DrawRectangle({ app->scene->player->position.x + (-640 * app->ScalingMultiplier), app->scene->player->position.y + (-360 * app->ScalingMultiplier),1280 * app->ScalingMultiplier,720 * app->ScalingMultiplier }, 0, 0, 0, 80);

		//app->render->DrawRectangle(app->inventory->rect, 0, 0, 0, 50);
		SDL_Rect rect = { 0, 0, 640, 360};
		app->render->DrawTexture(invTex, app->scene->player->position.x-420, app->scene->player->position.y-180, &rect);
		rect = { 0, 0, 20, 20 }; 
		app->render->DrawTexture(invArrowTex, app->scene->player->position.x - 260, app->scene->player->position.y-44, &rect);
		if (app->inventory->nrOfHpPot > 0) {
			//char amount = static_cast<char>(app->inventory->nrOfHpPot);
			string amountStr = std::to_string(app->inventory->nrOfHpPot);
			const char* amount = amountStr.c_str();
			app->render->DrawText(60, 140, WF, "Healing potion", 16);
			app->render->DrawText(300, 140, WF, amount, 16);
		}
		if (app->inventory->nrOfMpPot > 0) {
			string amountStr = std::to_string(app->inventory->nrOfMpPot);
			const char* amount = amountStr.c_str(); 
			app->render->DrawText(60, 170, WF, "Mana potion", 16);
			app->render->DrawText(300, 170, WF, amount, 16);
		}
		if (app->inventory->nrOfAtkElx > 0) {
			string amountStr = std::to_string(app->inventory->nrOfAtkElx);
			const char* amount = amountStr.c_str(); 
			app->render->DrawText(60, 200, WF, "Attack elixir", 16);
			app->render->DrawText(300, 200, WF, amount, 16);
		}
		if (app->inventory->nrOfDefElx > 0) {
			string amountStr = std::to_string(app->inventory->nrOfDefElx);
			const char* amount = amountStr.c_str(); 
			app->render->DrawText(60, 230, WF, "Deffense elixir", 16);
			app->render->DrawText(300, 230, WF, amount, 16);
		}
		//	TODO 4: Show the items' sprites in the inventory
		for (int i = 0; i < app->inventory->nrOfItems; i++)
		{
			app->render->DrawTexture(app->inventory->texture, player->position.x + 32 * i, player->position.y);
		}
	}
	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(trophyTex);
	app->tex->UnLoad(BGtexture);
	return true;
}

bool Scene::LoadState(pugi::xml_node& data) {
	return true; 
}

bool Scene::SaveState(pugi::xml_node& data) {
	return true; 
}

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

		// Marcar cuando el raton esta encima
		int x, y;
		x = app->input->GetMousePositionX();
		y = app->input->GetMousePositionY();

		if (x >= 640 / 2 - 32 && x <= 664 && y >= 26 + 45 * 2 && y <= 26 + 55 * 2) {
			app->render->DrawText(640 / 2 - 32, 26 + 45 * 2, WF, "Save game", 16);
			option = SELECTED::SAVEGAME;
		}
		else if (x >= 640 / 2 - 24 && x <= 664 && y >= 26 + 65 * 2 && y <= 26 + 75 * 2) {
			app->render->DrawText(640 / 2 - 24, 26 + 65 * 2, WF, "Options", 16);
			option = SELECTED::OPTIONS;
		}
		else if (x >= 640 / 2 - 32 && x <= 664 && y >= 26 + 85 * 2 && y <= 26 + 95 * 2) {
			app->render->DrawText(640 / 2 - 32, 26 + 85 * 2, WF, "Main menu", 16);
			option = SELECTED::MAINMENU;
		}
		else if (x >= 640 / 2 - 16 && x <= 664 && y >= 26 + 105 * 2 && y <= 26 + 115 * 2) {
			app->render->DrawText(640 / 2 - 16, 26 + 105 * 2, WF, "Exit", 16);
			option = SELECTED::EXIT;
		}
	}
	return true;
}

void Scene::RestartCave()
{
	app->scene->fade = true;
	app->scene->player->Teleport_Point(0, app->scene->player->tp12);

	TPressed1 = false;
	TPressed2 = false;
	TPressed3 = false;
	TPressed4 = false;
	TPressed5 = false;
	TPressed6 = false;
	TPressed7 = false;
	TPressed8 = false;
	TPressed9 = false;
	TPressed10 = false;
	TPressed11 = false;
	TPressed12 = false;
	TPressed13 = false;
	TPressed14 = false;
	TPressed15 = false;
	TPressed16 = false;
	TPressed17 = false;
	TPressed18 = false;
	TPressed19 = false;
	TPressed20 = false;
	TPressed21 = false;
	TPressed22 = false;
	TPressed23 = false;
	TPressed24 = false;
	TPressed25 = false;
	TPressed26 = false;
	TPressed27 = false;

	TBroken1 = false;
	TBroken2 = false;
	TBroken3 = false;
	TBroken4 = false;
	TBroken5 = false;
	TBroken6 = false;
	TBroken7 = false;
	TBroken8 = false;
	TBroken9 = false;
	TBroken10 = false;
	TBroken11 = false;
	TBroken12 = false;
	TBroken13 = false;
	TBroken14 = false;
	TBroken15 = false;
	TBroken16 = false;
	TBroken17 = false;
	TBroken18 = false;
	TBroken19 = false;
	TBroken20 = false;
	TBroken21 = false;
	TBroken22 = false;
	TBroken23 = false;
	TBroken24 = false;
	TBroken25 = false;
	TBroken26 = false;
	TBroken27 = false;
}

