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

	void FinishTurn();

public:

	COMBATMENU option = COMBATMENU::ATTACK;

	SDL_Texture* BG;

private:

	SDL_Texture* Character1;
	SDL_Texture* Character2;
	SDL_Texture* Character3;
	SDL_Texture* Enemy1;
	SDL_Texture* Enemy2;
	SDL_Texture* Enemy3;

	int C1MHP = 1000, C1CHP = 100;
	int C1MMP = 250, C1CMP = 200;

	int SaveInstance = 0, WF, GF;
	bool InCombat = false;

	int CurrentCharacters = 1;

	// 1-3 = ally, 4-6 = enemy
	int Turn[6] = { 1, 4, 5, 2, 6, 3 };
};

#endif // __COMBAT_H__
