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

Item::Item() : Entity(EntityType::ITEM)
{
	name.Create("item");
}

Item::~Item() {}

bool Item::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	itemName = parameters.attribute("name").as_int();
	texturePath = parameters.attribute("texturepath").as_string();



	return true;
}

bool Item::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	

	// L07 DONE 4: Add a physics to an item - initialize the physics body
	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 8, bodyType::STATIC);
	coinFX = app->audio->LoadFx("Assets/Sounds/CoinSoundEffect.wav");
	pbody->ctype = ColliderType::ITEM; 
	pbody->listener = this;
	if (itemName == "Healing Potion")
		app->inventory->addItem(*this); 
	return true;
}

bool Item::Update()
{
	return true;
}

bool Item::PostUpdate()
{
	
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	app->render->DrawTexture(texture, position.x + 8, position.y + 8);

	if (isPicked) {
		app->audio->PlayFxWithVolume(coinFX, 0, 50);
		active = false; 
		pbody->body->SetActive(false);
		return true; 
	}
		
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
		isPicked = true; 
		break;
	}
}