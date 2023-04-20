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
		float hp, chp, atk, def, spe, Eclass, exp, boss, asset;
		const char* name;
		const char* weakness;
		const char* resistance;

		const char* A1name;
		float A1dmg, A1target;

		const char* A2name;
		float A2dmg, A2target;

		const char* A3name;
		float A3dmg, A3target;

		const char* A4name;
		float A4dmg, A4target;

	};
	//Slime frog
	CombatEnemy slimeFrog{ 600,600,60,50,9,1,35, 1, 1,"Slime frog", "Lapis","Lucca", "Tongue attack", 50, 0, "Bubble burst", 60, 0 };
	CombatEnemy waterlilyfish{ 400,400,120,30,11,3,30, 1, 2,"Lilyfish", "Laurea","Lapis", "bite", 50, 0, "Tail whipping", 45, 0 };
	CombatEnemy naiadongoddess{ 2000,2000,400,20,11,1,300, 2, 3,"Naiadon goddess", "Lapis","Lucca", "Trident strike", 70, 0, "Tidalwave", 30, 1 };
	List<Entity*> entities;

	bool debug = false; 

};

#endif // __ENTITYMANAGER_H__
