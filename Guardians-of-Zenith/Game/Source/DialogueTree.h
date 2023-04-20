#ifndef __DIALOGTREE_H__
#define __DIALOGTREE_H__

#include "GuiManager.h"
#include "Module.h"
#include "GuiButton.h"

#include "List.h"
#include "Log.h"
#include "Point.h"
#include "SString.h"
#include <vector>


#define DIALOGUE_INPUT 2
#define DIALOGUE_SAVE 3
#define DIALOGUE_IF 4

using namespace std;

// Tree -> nodes -> choices

class DialogueChoice
{
public:
	DialogueChoice() {};
	~DialogueChoice() {};

public:
	// DONE: Add choices attributes
	int choiceID;
	SString text;
	int nextNode;
	int eventReturn;
};


class DialogueNode
{
public:
	DialogueNode() {};
	~DialogueNode() {};
	
	void SplitText(SString text, int fontSize_, int max_chars_line_);

	void CleanUp()
	{
		choicesList.clear();
	}

public:
	// DONE: Add nodes attributes 
	SString text;
	int nodeID;
	vector <DialogueChoice*> choicesList;
	int playerAnswer = -1;

	bool trimmed = false;
	vector<SString> texts;
};


class DialogueTree
{
public:
	DialogueTree() {};
	DialogueTree(bool a);
	~DialogueTree() {};

	bool UpdateTree(float dt, Module* mod, iPoint pos);
	bool UpdateChoices(Module* mod, iPoint pos, int fontSize);
	bool EventReturn(Module* mod, iPoint pos);
	void CleanUp();

public:
	// DONE: Create tree structure
	int treeID;
	bool active = false;
	vector <DialogueNode*> nodeList;
	DialogueNode* activeNode;


	List <GuiButton*> listDialogueButtons;

	bool updateOptions;
	int fontSize;
	int max_chars_line;
};

#endif //__DIALOGTREE_H__