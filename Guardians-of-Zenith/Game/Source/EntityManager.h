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
	CombatEnemy poisonSlimeFrog{ 600,600,0,0,100,40,9,1,35, 1, 4,"Poison frog", "Lapis","Lucca", "Tongue attack", 50, 0,0, "Poison bubble", 60, 0,2 };
	CombatEnemy infernalSlimeFrog{ 600,600,0,0,100,40,9,1,35, 1, 5,"Infernal frog", "Lapis","Lucca", "Tongue attack", 50, 0,0, "Magma bubble", 60, 0,3 };
	CombatEnemy electricSlimeFrog{ 600,600,0,0,100,40,9,1,35, 1, 6,"Electric frog", "Lapis","Lucca", "Tongue attack", 50, 0,0, "Electric bubble", 60, 0,4 };
	CombatEnemy demonicSlimeFrog{ 1000,1000,0,0,120,50,9,1,150, 1, 7,"Demonic frog", "Lapis","Lucca", "Corrupt blood", 50, 0,5, "Demonic fire", 60, 1,3 };
	CombatEnemy waterlilyfish{ 400,400,0,0,120,30,11,3,30, 1, 2,"Lilyfish", "Laurea","Lapis", "Bite", 50, 0,0, "Tail whipping", 45, 0,0 };
	CombatEnemy droughtwaterlilyfish{ 500,500,0,0,150,40,11,3,60, 1, 8,"Drought Lilyfish", "Laurea","Lapis", "Putrid bite", 50, 0,2, "Boiling water", 70, 0,3 };
	CombatEnemy naiadongoddess{ 2000,2000,400,400,400,20,11,1,300, 2, 3,"Naiadon", "Lapis","Lucca", "Trident strike", 70, 0,0, "Tidalwave", 30, 1,0, "Ice blast", 50, 0,1, "Mystic water", 0, 2,0};
	CombatEnemy deadLifeMantis{ 500,500,0,0,140,50,25,2,60, 1, 9,"Dead leaf mantis", "Laurea","Lapis", "Claw cut", 50, 0,0, "Poison bite", 60, 0,2 };
	CombatEnemy mutantPlant{ 800,800,0,0,80,70,8,3,55, 1, 10,"Mutant mushroom", "Lucca","Laurea", "Spore", 50, 1,0, "Vine whip", 60, 0,0 };
	CombatEnemy dryadon{ 2500,2500,500,500,450,45,10,2,550, 2, 11,"Dryadon", "Lucca","Laurea", "Polearm attack", 70, 0,0, "Dark spell", 50, 1,0, "Poison sting", 45, 0,2, "Damage up", 0, 2,0 };
	CombatEnemy pixie{ 700,700,0,0,110,50,13,2,120, 1, 12,"Pixie", "Lucca","Laurea", "Scratch", 70, 0,0, "Magic spell", 35, 0,-1 };
	CombatEnemy spectrum{ 1000,1000,0,0,50,40,12,1,100, 1, 13,"Spectrum", "Lapis","Lucca", "Soul drain", 50, 0,0, "Wildfire", 80, 0,3 };
	CombatEnemy gashadokuro{ 3000,3000,1000,1000,500,50,20,3,800, 2, 14,"Gashadokuro", "Laurea","Lapis", "Bone punch", 60, 0,0, "Bonemerang", 50, 1,0, "Hellfire", 50, 0,3, "Defense up", 0, 2,0 };
	List<Entity*> entities;


	SDL_Texture* ForestPNG_Sadow;

	bool debug = false; 

};

#endif // __ENTITYMANAGER_H__
