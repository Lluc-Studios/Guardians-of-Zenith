#ifndef __APP_H__
#define __APP_H__

#include "Module.h"
#include "List.h"
#include "Timer.h"
#include "PerfTimer.h"

#include "PugiXml/src/pugixml.hpp"

#define CONFIG_FILENAME		"config.xml"
#define SAVE_STATE_FILENAME "save_game.xml"

// Modules
class Window;
class Input;
class Render;
class Textures;
class Audio;
class Scene;
class EntityManager;
class Map;
class Tavern;
class Blacksmith;
class House;
class LakeDungeon;
class ForestDungeon;
class CaveDungeon;
class Monolith;
class Intro;
class Menu;
class ModuleFonts;
class Physics;
class DeathMenu;
class MainMenu;
class PathFinding; 
class Combat;
class DialogueSystem;
class GuiManager;
class Inventory;

class App
{
public:

	// Constructor
	App(int argc, char* args[]);

	// Destructor
	virtual ~App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	// L03: DONE 1: Create methods to control that the real Load and Save happens at the end of the frame
	void LoadGameRequest();
	void SaveGameRequest() ;
	bool LoadFromFile();
	bool SaveToFile() ;

private:

	// Load config file
	bool LoadConfig();

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

public:

	// Modules
	Window* win;
	Input* input;
	Render* render;
	Textures* tex;
	Audio* audio;
	Scene* scene;
	EntityManager* entityManager;
	Map* map;
	Tavern* tavern;
	Blacksmith* blacksmith;
	House* house;
	LakeDungeon* lakedungeon;
	ForestDungeon* forestdungeon;
	CaveDungeon* cavedungeon;
	Monolith* monolith;
	Intro* intro;
	Menu* menu;
	ModuleFonts* font;
	Physics* physics;
	DeathMenu* deathmenu;
	MainMenu* mainmenu;
	PathFinding* pathfinding; 
	Combat* combat;
	GuiManager* guiManager;
	DialogueSystem* dialogueSystem;
	Inventory* inventory;


	//This int determines which instance is loaded, 0 = Town, 1 = Tavern, 2 = Blacksmith, 3 = House, 4 = Lake Dungeon, 5 = Forest Dungeon, 6 = Cave Dungeon , 7 = Monoliths
	int Instance = 0;
	//In case scale is changed, this number shall be modified to match it
	int ScalingMultiplier = 2;
	

private:

	int argc;
	char** args;
	SString title;
	SString organization;

	List<Module*> modules;

	// L01: DONE 2: Create new variables from pugui namespace:
	// xml_document to store the config file and
	// xml_node(s) to read specific branches of the xml
	pugi::xml_document configFile;
	pugi::xml_node configNode;

	uint frames;
	float dt;

	// L03: DONE 1: Create control variables to control that the real Load and Save happens at the end of the frame
    bool saveGameRequested;
	bool loadGameRequested;

	Timer timer;
	PerfTimer ptimer;

	Timer startupTime;
	Timer frameTime;
	Timer lastSecFrameTime;

	uint64 frameCount = 0;
	uint32 framesPerSecond = 0;
	uint32 lastSecFrameCount = 0;

	float averageFps = 0.0f;
	float secondsSinceStartup = 0.0f;

	uint32 maxFrameDuration = 0;
};

extern App* app;

#endif	// __APP_H__