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
#include "Pathfinding.h"
#include "Tavern.h"
#include "Blacksmith.h"
#include "House.h"
#include "LakeDungeon.h"
#include "ForestDungeon.h"
#include "CaveDungeon.h"

#include "Defs.h"
#include "Log.h"

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

	Item* trophy =(Item*)app->entityManager->CreateEntity(EntityType::ITEM);
	trophy->parameters = config.child("trophy");
	trophy->score = 200; 

	for (pugi::xml_node en1Node = config.child("enemy"); en1Node; en1Node = en1Node.next_sibling("enemy")) {
		Enemy* grounded_e = (Enemy*)app->entityManager->CreateEntity(EntityType::ENEMY);
		grounded_e->parameters = en1Node;
	}

	for (pugi::xml_node en2Node = config.child("enemy2"); en2Node; en2Node = en2Node.next_sibling("enemy2")) {
		Enemy2* flying_e = (Enemy2*)app->entityManager->CreateEntity(EntityType::ENEMY2);
		flying_e->parameters = en2Node;
	}

	for (pugi::xml_node tpNode = config.child("teleport"); tpNode; tpNode = tpNode.next_sibling("teleport"))
	{
		Teleport* tp = (Teleport*)app->entityManager->CreateEntity(EntityType::TELEPORT);
		tp->parameters = tpNode;
	}

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	//img = app->tex->Load("Assets/Textures/test.png");
	//app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");
	
	// L03: DONE: Load map
	bool retLoad = app->map->Load();
	app->render->camera.x = 0;
	app->render->camera.y = 0;
	app->tavern->Load();
	app->blacksmith->Load();
	app->house->Load();
	// L04: DONE 7: Set the window title with map/tileset info
	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		app->map->mapData.width,
		app->map->mapData.height,
		app->map->mapData.tileWidth,
		app->map->mapData.tileHeight,
		app->map->mapData.tilesets.Count());
		//app->FPS);

	app->win->SetTitle(title.GetString());

	//Initialize trophy texture
	//coinTex = app->tex->Load("Assets/Textures/goldCoin.png");
	//trophyTex = app->tex->Load("Assets/Textures/trophy.png");
	//BGtexture = app->tex->Load("Assets/Maps/parallax1.png");

	// L12 Create walkability map
	if (retLoad) {
		int w, h;
		uchar* data = NULL;

		bool retWalkMap = app->map->CreateWalkabilityMap(w, h, &data);
		if (retWalkMap) app->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);

	}

	EnemyT = app->tex->Load("Assets/Entities/Enemies/Lake_SlimeFrog.png");
	NPC1 = app->tex->Load("Assets/Entities/NPC/npc_1.png");
	NPC2 = app->tex->Load("Assets/Entities/NPC/npc_2.png");
	NPC3 = app->tex->Load("Assets/Entities/NPC/npc_3.png");

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

	if (Music == false) {
		//app->audio->PlayMusic("Assets/Sounds/candybattleLONG.wav");
		Music = true;
	}
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
		app->audio->PlayMusic("Assets/Soundtrack/Music/Guidance_Island_OST_Version.ogg");
	}
	if (app->Instance == 1) {
		app->tavern->Draw();
		app->audio->PlayMusic("Assets/Soundtrack/Music/Guardia_Millennial_Fair.ogg");
	}
	//DO NOT TOUCH!!!
	if (app->Instance == 2) {
		app->blacksmith->Draw();
		app->audio->PlayMusic("Assets/Soundtrack/Music/Guardia_Millennial_Fair.ogg");
	}
	if (app->Instance == 3) {
		app->house->Draw();
		app->audio->PlayMusic("Assets/Soundtrack/Music/Tranquil_Days.ogg");
	}
	//if (app->Instance == 4) {
	//	app->lakedungeon->Draw();
	//  app->audio->PlayMusic("Assets/Soundtrack/Music/The_Riverlands.ogg");
	//}
	//if (app->Instance == 5) {
	//	app->forestdungeon->Draw();
	//  app->audio->PlayMusic("Assets/Soundtrack/Music/Azalea_Forest_OST_Version.ogg");
	//}
	//if (app->Instance == 6) {
	//	app->cavedungeon->Draw();
	//  app->audio->PlayMusic("Assets/Soundtrack/Music/Ruined_World.ogg");
	//}


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
			if (selected == 1) {
				app->Instance = 1;
				app->scene->player->tp1 = true;
				fade = false;
			}
			if (selected == 2) {
				app->Instance = 0;
				app->scene->player->tp2 = true;
				fade = false;
			}
			if (selected == 3) {
				app->Instance = 2;
				app->scene->player->tp3 = true;
				fade = false;
			}
			if (selected == 4) {
				app->Instance = 0;
				app->scene->player->tp4 = true;
				fade = false;
			}
			if (selected == 5) {
				app->Instance = 3;
				app->scene->player->tp5 = true;
				fade = false;
			}
			if (selected == 6) {
				app->Instance = 0;
				app->scene->player->tp6 = true;
				fade = false;
			}
			if (selected == 7) {
				app->Instance = 4;
				app->scene->player->tp7 = true;
				fade = false;
			}
			if (selected == 8) {
				app->Instance = 0;
				app->scene->player->tp8 = true;
				fade = false;
			}
			if (selected == 9) {
				app->Instance = 5;
				app->scene->player->tp9 = true;
				fade = false;
			}
			if (selected == 10) {
				app->Instance = 0;
				app->scene->player->tp10 = true;
				fade = false;
			}
			if (selected == 11) {
				app->Instance = 6;
				app->scene->player->tp11 = true;
				fade = false;
			}
			if (selected == 12) {
				app->Instance = 0;
				app->scene->player->tp12 = true;
				fade = false;
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

	//Enemy and npc draw
	app->render->DrawTexture(EnemyT, 1105, 224, &ET);
	app->render->DrawTexture(NPC1, 148, -280, &N1T);
	app->render->DrawTexture(NPC2, 224, -710, &N2T);
	app->render->DrawTexture(NPC3, 483, 545, &N3T);

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

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
