#include "Item.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Player.h"
#include "Inventory.h"
#include "Combat.h"

Item::Item() : Entity(EntityType::ITEM)
{
	name.Create("item");
}

Item::~Item() {}

bool Item::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	id = parameters.attribute("id").as_int();
	itemName = parameters.attribute("name").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	instance = parameters.attribute("instance").as_int();

	return true;
}

bool Item::Start() {

	isPicked = false;
	//initilize textures
	texture = app->tex->Load(texturePath);

	// L07 DONE 4: Add a physics to an item - initialize the physics body
	pbody = app->physics->CreateRectangleSensor(position.x + 16, position.y + 16, 16, 16, bodyType::STATIC);
	pbody->listener = this;

	//coinFX = app->audio->LoadFx("Assets/Soundtrack/Fx/Menu/Play.wav");
	PickupFx = app->audio->LoadFx("Assets/Soundtrack/Fx/Gameplay/PickupFx.wav");
	pbody->ctype = ColliderType::ITEM; 

	app->scene->texturas[id - 1] = texture;
	app->scene->itemPos[id - 1] = position;
	app->scene->itemPicked[id - 1] = isPicked;
	app->scene->itemBody[id - 1] = pbody;
	app->scene->itemInstance[id - 1] = instance;

	return true;
}

bool Item::Update()
{
	return true;
}

bool Item::PostUpdate()
{		
	return true;
}

bool Item::CleanUp()
{
	return true;
}

void Item::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		LOG("Collision ITEM");
		isPicked = true;
		app->audio->PlayFxWithVolume(PickupFx, 0, app->audio->fxvolume);
		app->scene->itemPicked[id-1] = isPicked;
		physA->body->DestroyFixture(physA->body->GetFixtureList());
		if (!handledCollision) {
			//app->inventory->addItem(*this);
			switch (id)
			{
			case 1:
				app->inventory->nrOfHpPot++;
				break;
			case 2:
				app->inventory->nrOfHpPot++;
				break;
			case 3:
				app->inventory->nrOfHpPot++;
				break;
			case 4:
				app->inventory->nrOfMpPot++;
				break;
			case 5:
				app->inventory->nrOfMpPot++;
				break;
			case 6:
				app->inventory->nrOfAtkElx++;
				break;
			case 7:
				app->inventory->nrOfAtkElx++;
				break;
			case 8:
				app->inventory->nrOfDefElx++;
				break;
			case 9:
				app->inventory->nrOfDefElx++;
				break;
			case 10:
				app->inventory->necklace++;
				break;
			case 11:
				app->inventory->manuscript++;
				app->scene->player->LevelToMax();
				break;
			case 12:
				app->inventory->manuscript++;
				app->scene->player->LevelToMax();
				break;
			case 13:
				app->inventory->manuscript++;
				app->scene->player->LevelToMax();
				break;
			case 14:
				app->inventory->manuscript++;
				app->scene->player->LevelToMax();
				break;
			case 15:
				app->inventory->book++;
				for (int i = 0; i < 5; i++)
				{
					//Characters level up
					if (app->scene->player->laurea.lvl <= 20) {
						app->scene->player->laurea.lvl++;
						app->scene->player->laurea.EXPneeded += (app->scene->player->laurea.EXPneeded * 0.1);
						app->scene->player->laurea.exp = 0;
						app->scene->player->laurea.hp += (app->scene->player->laurea.HPG * app->scene->player->laurea.HPC);
						app->scene->player->laurea.chp += (app->scene->player->laurea.HPG * app->scene->player->laurea.HPC);
						app->scene->player->laurea.mp += (app->scene->player->laurea.MPG * app->scene->player->laurea.MPC);
						app->scene->player->laurea.cmp += (app->scene->player->laurea.MPG * app->scene->player->laurea.MPC);
						app->scene->player->laurea.atk += (app->scene->player->laurea.ATKG * app->scene->player->laurea.ATKC);
						app->scene->player->laurea.def += (app->scene->player->laurea.DEFG * app->scene->player->laurea.DEFC);
					}
					if (app->scene->player->lapis.lvl <= 20) {
						app->scene->player->lapis.lvl++;
						app->scene->player->lapis.EXPneeded += (app->scene->player->lapis.EXPneeded * 0.1);
						app->scene->player->lapis.exp = 0;
						app->scene->player->lapis.hp += (app->scene->player->lapis.HPG * app->scene->player->lapis.HPC);
						app->scene->player->lapis.chp += (app->scene->player->lapis.HPG * app->scene->player->lapis.HPC);
						app->scene->player->lapis.mp += (app->scene->player->lapis.MPG * app->scene->player->lapis.MPC);
						app->scene->player->lapis.cmp += (app->scene->player->lapis.MPG * app->scene->player->lapis.MPC);
						app->scene->player->lapis.atk += (app->scene->player->lapis.ATKG * app->scene->player->lapis.ATKC);
						app->scene->player->lapis.def += (app->scene->player->lapis.DEFG * app->scene->player->lapis.DEFC);
					}
					if (app->scene->player->lucca.lvl <= 20) {
						app->scene->player->lucca.lvl++;
						app->scene->player->lucca.EXPneeded += (app->scene->player->lucca.EXPneeded * 0.1);
						app->scene->player->lucca.exp = 0;
						app->scene->player->lucca.hp += (app->scene->player->lucca.HPG * app->scene->player->lucca.HPC);
						app->scene->player->lucca.chp += (app->scene->player->lucca.HPG * app->scene->player->lucca.HPC);
						app->scene->player->lucca.mp += (app->scene->player->lucca.MPG * app->scene->player->lucca.MPC);
						app->scene->player->lucca.cmp += (app->scene->player->lucca.MPG * app->scene->player->lucca.MPC);
						app->scene->player->lucca.atk += (app->scene->player->lucca.ATKG * app->scene->player->lucca.ATKC);
						app->scene->player->lucca.def += (app->scene->player->lucca.DEFG * app->scene->player->lucca.DEFC);
					}
				}
				break;
			default:
				break;
			}
		}
		break;
	}
	handledCollision = true; // Set flag to indicate collision has been handled to avoid adding the item more than once
}