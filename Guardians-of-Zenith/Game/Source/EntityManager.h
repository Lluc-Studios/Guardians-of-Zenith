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
		float hp, chp,es,ces, atk, def, spe, Eclass, exp, boss, asset;
		const char* name;
		const char* weakness;
		const char* resistance;

		const char* A1name;
		float A1dmg, A1target, A1effect;

		const char* A2name;
		float A2dmg, A2target, A2effect;

		const char* A3name;
		float A3dmg, A3target, A3effect;

		const char* A4name;
		float A4dmg, A4target, A4effect;

	};
	//Slime frog
	CombatEnemy slimeFrog{ 600,600,0,0,100,40,9,1,35, 1, 1,"Slime frog", "Lapis","Lucca", "Tongue attack", 50, 0,0, "Bubble burst", 60, 0,0 };
	CombatEnemy waterlilyfish{ 400,400,0,0,120,30,11,3,30, 1, 2,"Lilyfish", "Laurea","Lapis", "Bite", 50, 0,0, "Tail whipping", 45, 0,0 };
	CombatEnemy naiadongoddess{ 2000,2000,400,400,400,20,11,1,300, 2, 3,"Naiadon", "Lapis","Lucca", "Trident strike", 70, 0,0, "Tidalwave", 30, 1,0, "Ice blast", 50, 0,1, "Mystic water", 0, 2,0};
	List<Entity*> entities;

	bool debug = false; 

};

#endif // __ENTITYMANAGER_H__
