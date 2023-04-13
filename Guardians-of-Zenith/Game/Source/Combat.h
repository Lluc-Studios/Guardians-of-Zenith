#ifndef __COMBAT_H__
#define __COMBAT_H__

#include "Module.h"
#include "Player.h"
#include "Item.h"


class Combat : public Module
{
public:

	enum class COMBATMENU {
		ATTACK = 1,
		DEFEND,
		INVENTORY,
		ESCAPE
	};

	Combat();

	// Destructor
	virtual ~Combat();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void StartCombat();

	void ExitCombat();

public:
	COMBATMENU option = COMBATMENU::ATTACK;

	SDL_Texture* BG;

private:
	int SaveInstance = 0, WF, GF;
	bool InCombat = false;

	int CurrentCharacters = 1;
};

#endif // __COMBAT_H__
