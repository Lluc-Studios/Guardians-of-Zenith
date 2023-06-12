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

	coinFX = app->audio->LoadFx("Assets/Soundtrack/Fx/Menu/Play.wav");
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
				app->scene->player->laurea.lvl++;
				app->scene->player->lapis.lvl++;
				app->scene->player->lucca.lvl++;
				break;
			case 12:
				app->inventory->manuscript++;
				app->scene->player->laurea.lvl++;
				app->scene->player->lapis.lvl++;
				app->scene->player->lucca.lvl++; 
				break;
			case 13:
				app->inventory->manuscript++;
				app->scene->player->laurea.lvl++;
				app->scene->player->lapis.lvl++;
				app->scene->player->lucca.lvl++;
				break;
			case 14:
				app->inventory->manuscript++;
				app->scene->player->laurea.lvl++;
				app->scene->player->lapis.lvl++;
				app->scene->player->lucca.lvl++;
				break;
			case 15:
				app->inventory->book++;
				app->scene->player->laurea.lvl += 5;
				app->scene->player->lapis.lvl += 5;
				app->scene->player->lucca.lvl += 5;

				break;
			default:
				break;
			}
		}
		break;
	}
	handledCollision = true; // Set flag to indicate collision has been handled to avoid adding the item more than once
}