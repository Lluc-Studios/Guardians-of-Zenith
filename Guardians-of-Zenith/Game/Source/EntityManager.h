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
		int hp, chp, atk, def, spe, Eclass, exp;
		const char* name;
		const char* weakness;
		const char* resistance;

		const char* A1name;
		int A1dmg, A1target;

		const char* A2name;
		int A2dmg, A2target;

		const char* A3name;
		int A3dmg, A3target;

		const char* A4name;
		int A4dmg, A4target;

	};
	//Slime frog
	//CombatEnemy slimeFrog1{ 600,600,100,50,9,35,"slimefrog", "lapis","lucca"};
	//CombatEnemy slimeFrog2{ 600,600,100,50,9,35,"slimefrog", "lapis","lucca"};
	//CombatEnemy slimeFrog3{ 600,600,100,50,9,35,"slimefrog", "lapis","lucca"};
	CombatEnemy slimeFrog1{ 600,600,100,50,9,1,35,"slimefrog", "lapis","lucca", "tongueattack", 50, 0, "bubbleburst", 60, 0 };
	CombatEnemy slimeFrog2{ 600,600,100,50,9,1,35,"slimefrog", "lapis","lucca", "tongueattack", 50, 0, "bubbleburst", 60, 0 };
	CombatEnemy slimeFrog3{ 600,600,100,50,9,1,35,"slimefrog", "lapis","lucca", "tongueattack", 50, 0, "bubbleburst", 60, 0 };


	List<Entity*> entities;

	bool debug = false; 

};

#endif // __ENTITYMANAGER_H__
