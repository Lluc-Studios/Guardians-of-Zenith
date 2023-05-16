#include "EntityManager.h"
#include "Player.h"
#include "Enemy.h"
#include "Enemy2.h"
#include "Item.h"
#include "Teleport.h"
#include "Saw.h"
#include "App.h"
#include "Textures.h"
#include "Scene.h"
#include "ForestDungeon.h"

#include "Defs.h"
#include "Log.h"

EntityManager::EntityManager() : Module()
{
	name.Create("entitymanager");
}

// Destructor
EntityManager::~EntityManager()
{}

// Called before render is available
bool EntityManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Entity Manager");
	bool ret = true;

	//Iterates over the entities and calls the Awake
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Awake();
	}

	return ret;

}

bool EntityManager::Start() {

	bool ret = true; 

	//Iterates over the entities and calls Start
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Start();
	}

	return ret;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	bool ret = true;
	ListItem<Entity*>* item;
	item = entities.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	entities.Clear();

	return ret;
}

Entity* EntityManager::CreateEntity(EntityType type)
{
	Entity* entity = nullptr; 

	switch (type)
	{

	case EntityType::PLAYER:
		entity = new Player();
		break;

	case EntityType::ENEMY:
		entity = new Enemy();
		break;

	case EntityType::ENEMY2:
		entity = new Enemy2();
		break;

	case EntityType::ITEM:
		entity = new Item();
		break;

	case EntityType::SAW:
		entity = new Saw();
		break;
	case EntityType::TELEPORT:
		entity = new Teleport();
		break;
	default: break;
	}

	// Created entities are added to the list
	AddEntity(entity);

	return entity;
}

void EntityManager::DestroyEntity(Entity* entity)
{
	ListItem<Entity*>* item;

	for (item = entities.start; item != NULL; item = item->next)
	{
		if (item->data == entity) entities.Del(item);
	}
}

void EntityManager::AddEntity(Entity* entity)
{
	if ( entity != nullptr) entities.Add(entity);
}

bool EntityManager::Update(float dt)
{
	bool ret = true;
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Update(dt);
	}

	return ret;
}

bool EntityManager::LoadState(pugi::xml_node& data)
{
	bool ret = true; 
	ListItem<Entity*>* item;
	for (item = entities.start; item != nullptr; item = item->next)
	{
		if (item != nullptr) {
			item->data->LoadState(data);
		}
	}
	
	return ret;
}


bool EntityManager::SaveState(pugi::xml_node& data)
{
	bool ret = true;
	ListItem<Entity*>* item;
	
	for (item = entities.start; item != nullptr; item = item->next)
	{
		if (item != nullptr)
			item->data->SaveState(data);
	}

	return true;
}