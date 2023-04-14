#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Module.h"
#include "Entity.h"
#include "List.h"

class EntityManager : public Module
{
public:

	EntityManager();

	// Destructor
	virtual ~EntityManager();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called after Awake
	bool Start();
	// Called every frame
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	// Additional methods
	Entity* CreateEntity(EntityType type);

	void DestroyEntity(Entity* entity);

	void AddEntity(Entity* entity);

	bool LoadState(pugi::xml_node& data);

	bool SaveState(pugi::xml_node& data);


public:

	// stats
	struct CombatEnemy
	{
		int hp, chp, atk, def, spe, exp;
		const char* weakness;
		const char* resistance;
	};
	//Slime frog
	CombatEnemy slimeFrog1{ 600,600,100,50,9,35, "Lapis","Lucca"};
	CombatEnemy slimeFrog2{ 600,600,100,50,9,35, "Lapis","Lucca" };
	CombatEnemy slimeFrog3{ 600,600,100,50,9,35, "Lapis","Lucca" };


	List<Entity*> entities;

	bool debug = false; 

};

#endif // __ENTITYMANAGER_H__
