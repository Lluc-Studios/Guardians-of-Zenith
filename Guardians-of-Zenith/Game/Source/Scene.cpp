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

	villageMusic = app->audio->LoadFx("Assets/Soundtrack/Music/Guidance Island OST Version.ogg");

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
	app->monolith->Load();
	//app->lakedungeon->Load();
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

	EnemyT = app->tex->Load("Assets/Entities/Enemies/SlimeFrog_Movment.png");
	NPC1 = app->tex->Load("Assets/Entities/NPC/npc_1.png");
	NPC2 = app->tex->Load("Assets/Entities/NPC/npc_2.png");
	NPC3 = app->tex->Load("Assets/Entities/NPC/npc_3.png");
	LAPIS = app->tex->Load("Assets/Entities/Characters/Lapis_Directions.png");

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
		app->audio->PlayFx(villageMusic, 10);
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
		//app->audio->PlayMusic("Assets/Soundtrack/Music/Guidance_Island_OST_Version.ogg");
	}
	if (app->Instance == 1) {
		app->tavern->Draw();
		//app->audio->PlayMusic("Assets/Soundtrack/Music/Guardia_Millennial_Fair.ogg");
	}
	//DO NOT TOUCH!!!
	if (app->Instance == 2) {
		app->blacksmith->Draw();
		//app->audio->PlayMusic("Assets/Soundtrack/Music/Guardia_Millennial_Fair.ogg");
	}
	if (app->Instance == 3) {
		app->house->Draw();
		//app->audio->PlayMusic("Assets/Soundtrack/Music/Tranquil_Days.ogg");
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
	if (app->Instance == 7) {
		app->monolith->Draw();
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
			if (selected == 13) {
				app->Instance = 7;
				app->scene->player->tp13 = true;
				fade = false;
			}
			if (selected == 14) {
				app->Instance = 0;
				app->scene->player->tp14 = true;
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
	app->render->DrawTexture(EnemyT, 1089, 208, &ET);
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
		if(!app->combat->InCombat)
			isPaused = !isPaused;

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
