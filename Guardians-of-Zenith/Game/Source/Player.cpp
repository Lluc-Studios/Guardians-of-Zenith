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

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");

}

Player::~Player() {

}

bool Player::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	width = parameters.attribute("width").as_int();
	height = parameters.attribute("height").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	Laurea::stats laurea{ 35,10,20,25,10,2,1,4,3,2,3 };
	Lapis::stats lapis{ 25,20,25,18,12,4,1,3,4,3,2 };
	Lucca::stats lucca{ 20,23,30,12,15,3,1,3,3,3,2 };

	//LOG("stats Laurea: %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d", laurea.hp, laurea.mp, laurea.atk, laurea.def, laurea.spe, laurea.limit, laurea.lvl, laurea.hpGrowth, laurea.mpGrowth, laurea.atkGrowth, laurea.defGrowth);

	return true;
}

bool Player::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	//pbody = app->physics->CreateRectangle(position.x + width/2, position.y + height/2, width-55, height-2, bodyType::DYNAMIC);
	pbody = app->physics->CreateRectangle(position.x, position.y, 18, 29, bodyType::DYNAMIC);
	//pbody = app->physics->CreateCircle(position.x + width / 2, position.y + height / 2,14/2, bodyType::DYNAMIC);
	pbody->body->SetFixedRotation(true);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	alive = true; 


	attackHitbox = app->physics->CreateRectangleSensor(0, 0, 25, 15, bodyType::STATIC);
	attackHitbox->ctype = ColliderType::PLAYER_ATTACK;

	//Sounds
	Step1 = app->audio->LoadFx("Assets/Sounds/Player/FootGravel1.wav");
	Step2 = app->audio->LoadFx("Assets/Sounds/Player/FootGravel2.wav");
	StepMetalic1 = app->audio->LoadFx("Assets/Sounds/Player/StepMetal1.wav");
	StepMetalic2 = app->audio->LoadFx("Assets/Sounds/Player/StepMetal2.wav");
	DeathSound = app->audio->LoadFx("Assets/Sounds/Player/Fire2.wav");
	Swing = app->audio->LoadFx("Assets/Sounds/Player/Swing.wav");
	DamageFx = app->audio->LoadFx("Assets/Sounds/Enemy/Dead2.wav");
	tpFX = app->audio->LoadFx("Assets/Sounds/TeleportSound.wav");

	//Textures
	LFHH = app->tex->Load("Assets/Textures/FULL.png");
	LFH = app->tex->Load("Assets/Textures/HALF.png");
	LFL = app->tex->Load("Assets/Textures/LOW.png");
	LFE = app->tex->Load("Assets/Textures/EMPTY.png");

	char lookupTable[] = { "abcdefghijklmnopqrstuvwxyz0123456789" };
	WF = app->font->Load("Assets/Fonts/FontWhiteDef.png", lookupTable, 1);
	YF = app->font->Load("Assets/Fonts/FontYellowDef.png", lookupTable, 1);



	////Animations
	//playerIdleR.PushBack({ 0 * width,0 * height,width,height });
	//playerIdleL.PushBack({ 0 * width,1 * height,width,height });

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

	//currentAnim = &playerIdleR;

	return true;
}



bool Player::Update()
{
	
	//LOG("Attack Cooldown: %i",attackCD);

	if (auxBool == true) {
		auxCounter++;
		if (auxCounter == 50) {
			auxCounter = 0;
			auxBool = false;
		}
	}

	debugKeys();

	if (playerState != State::ATTACKING)
		Move();

	TotalTime = SDL_GetTicks();

	if ((app->input->GetMouseButtonDown(1) && attackCD <= 0) || (attackFrames > 0 && playerState == State::ATTACKING)) {
		playerState = State::ATTACKING;
		app->audio->PlayFxWithVolume(Swing, 0, 25);
		Attack(attackFrames);
		
		attackFrames--;
	}
	else {
		playerState = State::IDLE;
		attackFrames = 10;
	}
	attackCD--;
	

	//Death
	if (alive != true) {
		Death(); 
	}
	
	//SDL_Rect rect = currentAnim->GetCurrentFrame();
	//currentAnim->Update();
	SDL_Rect rectDown = { 8,2,19,28 };
	SDL_Rect rectUp = { 39,2,18,28 };
	SDL_Rect rectLeft = { 72,2,21,29 };
	SDL_Rect rectRight = { 100,2,21,29 };

	position.x = METERS_TO_PIXELS((pbody->body->GetTransform().p.x) - width / 2);
	position.y = METERS_TO_PIXELS((pbody->body->GetTransform().p.y) - height / 2);

	if (tp1) {
		position.x = 185;
		position.y = -175;
		pbody->body->SetTransform({ PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y) }, 0);
		tp1 = false; 
	}
	if (tp2) {
		position.x = 1015;
		position.y = 190;
		pbody->body->SetTransform({ PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y) }, 0);
		tp2 = false;
	}

	app->render->camera.x = ((-1 * (position.x * app->win->GetScale() - app->render->camera.w / 2))-60)-70;
	app->render->camera.y = -1 * (position.y * app->win->GetScale() - app->render->camera.h / 2);

	//Player draw
	if (facing == DIRECTION::RIGHT) {
		app->render->DrawTexture(texture, position.x + 21, position.y - 7, &rectRight);
	}
	if (facing == DIRECTION::LEFT) {
		app->render->DrawTexture(texture, position.x + 21, position.y - 7, &rectLeft);
	}
	if (facing == DIRECTION::DOWN) {
		app->render->DrawTexture(texture, position.x + 21, position.y - 7, &rectDown);
	}
	if (facing == DIRECTION::UP) {
		app->render->DrawTexture(texture, position.x + 21, position.y - 7, &rectUp);
	}

	//Player Health Damage
	if ((lifeAux >= 1 && lifeAux <= 40) || (lifeAux >= 60 && lifeAux <= 100) || (lifeAux >= 120 && lifeAux <= 150)) {
		app->render->DrawRectangle({ position.x + 139, position.y + 69,32,18 }, 255, 255, 255);
		app->render->DrawRectangle({ position.x + 171, position.y + 75,2,6 }, 255, 255, 255);
	}

	//Player Health Draw
	if (life >= 3) {
		app->render->DrawTexture(LFHH, position.x + 140, position.y + 70);
	}
	if (life == 2) {
		app->render->DrawTexture(LFH, position.x + 140, position.y + 70);
	}
	if (life == 1) {
		app->render->DrawTexture(LFL, position.x + 140, position.y + 70);
	}
	if (life == 0) {
		app->render->DrawTexture(LFE, position.x + 140, position.y + 70);
	}

	//Score Print
	char ConvertAux[12];
	sprintf_s(ConvertAux, "%d", score);
	app->font->BlitText(x.x+1, x.y+170, WF, "score");
	app->font->BlitText(x.x + 34, x.y + 170, WF, ConvertAux);

	//Timer
	if (TotalTime - Time >= 1000) {
		Time = TotalTime;
		timer++;
	}
	sprintf_s(ConvertAux, "%d", timer);
	app->font->BlitText(x.x + 140, x.y+1, YF, "time");
	app->font->BlitText(x.x + 167, x.y + 1, YF, ConvertAux);

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
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		break;
	case ColliderType::FLOOR:
		LOG("Collision FLOOR");
		break;
	case ColliderType::SPIKES:
		LOG("Collision SPIKES");
		if (invincible == false) {
			if (life != 1 && lifeAux == 0) {
				app->audio->PlayFxWithVolume(DamageFx, 0, 50);
				life--;
				lifeAux++;
			}
			else if (life == 1 && lifeAux == 0) {
				life = 0;
				alive = false;
			}
		}
		break;
	case ColliderType::WALL:
		LOG("Collision WALL");

		break; 
	case ColliderType::TELEPORT:
		if (physB->id == 1) {
			tp1 = true; 
		}
		else if (physB->id == 2) {
			tp2 = true; 
		}
		app->audio->PlayFxWithVolume(tpFX, 0, 50);
		LOG("COLLISION TP: %i, position: %i, %i", physB->id);
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	case ColliderType::ENEMY:
		LOG("Collision Enemy");
		if (invincible == false) {
			if (life != 1 && lifeAux == 0) {
				app->audio->PlayFxWithVolume(DamageFx, 0, 50);
				life--;
				lifeAux++;
			}
			else if (life == 1 && lifeAux == 0) {
				life = 0;
				alive = false;
			}
		}
	case ColliderType::TAVERN:
		LOG("Collision Tavern");
		if (auxBool == false) {
			app->Instance = 1;
			app->tavern->Load();
			tp1 = true;
			auxBool = true;
		}

	case ColliderType::TOWN:
		LOG("Collision Town");
		if (auxBool == false) {
			app->Instance = 0;
			tp2 = true;
			auxBool = true;
		}

	}

	
}


bool Player::IsAlive() {
	return alive; 
}

bool Player::LoadState(pugi::xml_node& data) {

	position.x = data.child("player_stats").attribute("position_x").as_int();
	position.y = data.child("player_stats").attribute("position_y").as_int();
	playerState = (State)data.child("player_stats").attribute("state").as_int();
	pbody->body->SetTransform({ PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y) }, 0);

	return true;
}

bool Player::SaveState(pugi::xml_node& data) {
	pugi::xml_node player_stats = data.append_child("player_stats");
	data.child("player_stats").append_attribute("position_x") = position.x;
	data.child("player_stats").append_attribute("position_y") = position.y;
	data.child("player_stats").append_attribute("state") =	(int)playerState;

	return true; 
}

void Player::Move() {

	float speed = 3;
	vel = b2Vec2(0, pbody->body->GetLinearVelocity().y);
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_IDLE && app->input->GetKey(SDL_SCANCODE_S) == KEY_IDLE && app->scene->CanPlayerMove == true) {
		vel = b2Vec2(0, 0);
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && app->scene->CanPlayerMove == true) {
		vel = b2Vec2(-speed, 0);
		facing = DIRECTION::LEFT;
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && app->scene->CanPlayerMove == true) {
		vel = b2Vec2(speed, 0);
		facing = DIRECTION::RIGHT;
	}
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && app->scene->CanPlayerMove == true) {
		vel = b2Vec2(0, -speed);
		facing = DIRECTION::UP;
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && app->scene->CanPlayerMove == true) {
		vel = b2Vec2(0, speed);
		facing = DIRECTION::DOWN;
	}

	//Fx
	if (vel.x != 0 && vel.y == 0) {
		aux++;
		if (aux == 0) {
			app->audio->PlayFxWithVolume(Step1, 0, 25);
		}
		if (aux == 10) {
			app->audio->PlayFxWithVolume(Step2, 0, 25);
			aux = -10;
		}
	}

	//if (facing == DIRECTION::RIGHT && vel.x == 0) {
	//	currentAnim = &playerIdleR;
	//}
	//if (facing == DIRECTION::LEFT && vel.x == 0) {
	//	currentAnim = &playerIdleL;
	//}
	//if (facing == DIRECTION::RIGHT && vel.x != 0) {
	//	currentAnim = &playerRunR;
	//}
	//if (facing == DIRECTION::LEFT && vel.x != 0) {
	//	currentAnim = &playerRunL;
	//}


	//Set the velocity of the pbody of the player
	pbody->body->SetLinearVelocity(vel);
}

void Player::Attack(int frames) {

	

	if (frames > 1) {
		
		if (facing == DIRECTION::RIGHT) {
			attackHitbox->body->SetTransform({ PIXEL_TO_METERS((position.x+50)),  PIXEL_TO_METERS((position.y+8)) }, 0);
			//app->render->DrawRectangle({ position.x+21, position.y, 50, 20 }, 255, 0, 0, 200);
			//currentAnim = &playerAttackR;
		}
		if (facing == DIRECTION::LEFT) {
			attackHitbox->body->SetTransform({ PIXEL_TO_METERS((position.x +15)), PIXEL_TO_METERS((position.y+8)) }, 0);
			//app->render->DrawRectangle({ position.x-39, position.y, 50, 20 }, 255, 0, 0, 200);
			//currentAnim = &playerAttackL;
		}
	}
	else {

		attackHitbox->body->SetTransform({0, 0}, 0);
		attackCD = 40;
	}
	
	

	
}


void Player::debugKeys() {

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {
		if (invincible == false) {
			invincible = true;
		}
		else if (invincible == true) {
			invincible = false;
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {
		position.x = parameters.attribute("x").as_int();
		position.y = parameters.attribute("y").as_int();
		pbody->body->SetTransform({ PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y) }, 0);
	}

	if (app->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN) {
		alive = false;
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
