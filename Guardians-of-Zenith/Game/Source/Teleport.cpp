#include "Teleport.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Animation.h"

Teleport::Teleport() : Entity(EntityType::TELEPORT)
{
	name.Create("Teleport");
}

Teleport::~Teleport() {}

bool Teleport::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	id = parameters.attribute("id").as_int();

	return true;
}

bool Teleport::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

	// L07 DONE 4: Add a physics to an Teleporty - initialize the physics body
	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 8, bodyType::STATIC);

	pbody->ctype = ColliderType::TELEPORT;
	pbody->id = id; 

	spinAnim.PushBack({ 0,0,32,32 });
	spinAnim.PushBack({ 32,0,32,32 });
	spinAnim.PushBack({ 64,0,32,32 });
	spinAnim.PushBack({ 96,0,32,32 });
	spinAnim.PushBack({ 128,0,32,32 });
	spinAnim.PushBack({ 160,0,32,32 });
	spinAnim.PushBack({ 192,0,32,32 });
	spinAnim.PushBack({ 224,0,32,32 });
	spinAnim.PushBack({ 256,0,32,32 });
	spinAnim.speed = 0.3f;
	spinAnim.loop = true;

	currentAnimation = &spinAnim;

	

	return true;
}

bool Teleport::Update()
{
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	currentAnimation->Update();
	
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	app->render->DrawTexture(texture, position.x , position.y , &rect);

	return true;
}

bool Teleport::CleanUp()
{
	return true;
}