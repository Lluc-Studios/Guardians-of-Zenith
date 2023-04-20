#include "DialogueTree.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Window.h"

using namespace std;

void DialogueNode::SplitText(SString text_, int fontSize_, int max_chars_line_)
{
	string line = text_.GetString();

	// TODO 6: adapt text to the text box
	if (text_.Length() > max_chars_line_)
	{
		int a, b, startIndex = 0;
		for (int j = 0; j <= line.length() / max_chars_line_; j++)	// <= -> in case of decimal, get the round up number 
		{
			a = max_chars_line_ + startIndex;
			b = line.find_first_of(" ", a);	// find first " " (space) from last trimmed to the end. 

			// If we reached the end of the word or the end of the input.
			string temp;
			temp.append(line, startIndex, b - startIndex);	// string text to append, int index start, int size of text to append
			texts.push_back(temp.c_str());
			startIndex = b;
		}
	}
	else
	{
		texts.push_back(line.c_str());
	}

	trimmed = true;
}


DialogueTree::DialogueTree(bool a)
{
	active = a;
}

bool DialogueTree::UpdateTree(float dt, Module* mod, iPoint pos)
{
	fontSize = 12;
	max_chars_line = fontSize * 6;

	// TODO 6: Substitute player's name in text and choices if needed
	if (!app->input->playerName.empty())
	{
		activeNode->text.Substitute("%x", app->input->playerName.c_str());
	}

	if (!activeNode->trimmed)
	{
		activeNode->SplitText(activeNode->text, fontSize, max_chars_line);
	}

	// TODO 6: Render dialogue in text box
	size_t lines = activeNode->texts.size();
	for (size_t i = 0; i < lines; i++)
	{
		//app->render->TextDraw(activeNode->texts[i].GetString(), pos.x + 100, pos.y + 20 + 50 * i, fontSize, { 255, 255, 255 });
		app->render->DrawText(100, 200, { 255, 255, 255 }, activeNode->texts[i].GetString(), fontSize);
	}

	EventReturn(mod, pos);

	if (!updateOptions)
	{
		updateOptions = UpdateChoices(mod, pos, fontSize);
	}

	return true;
}

bool DialogueTree::UpdateChoices(Module* mod, iPoint pos, int fontSize)
{
	GuiButton* button;

	// TODO 3: Create buttons for choices
	for (int i = 0; i < activeNode->choicesList.size(); i++)
	{
		const char* ch_option = activeNode->choicesList[i]->text.GetString();	// SString to const char*	
		int w = activeNode->choicesList[i]->text.Length() * fontSize * 0.5 + 10;
		int h = fontSize + fontSize / 4;

		uint width, height;
		app->win->GetWindowSize(width, height);
		SDL_Rect bounds = { width - w, pos.y - (h + fontSize) * (i + 1), w, h};

		button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, i, ch_option, bounds, fontSize, mod);
		button->state = GuiControlState::NORMAL;
		listDialogueButtons.Add(button);
	}

	return true;
}

bool DialogueTree::EventReturn(Module* mod, iPoint pos)
{
	for (int i = 0; i < activeNode->choicesList.size(); i++)
	{
		switch (activeNode->choicesList[i]->eventReturn)
		{
		case DIALOGUE_INPUT:

			if (!app->input->getInput)
			{
				// Get player's input
				app->input->getInput = true;
				updateOptions = true;

				if (app->input->nameEntered)
				{
					activeNode->choicesList[i]->text.Substitute("%x", app->input->playerName.c_str());	// Change %x for player's name - (const char* current word, const char* new word)
					updateOptions = UpdateChoices(mod, pos, fontSize);
				}
			}

			uint width, height;
			app->win->GetWindowSize(width, height);
			app->render->DrawText( width / 3, 100,{ 255, 255, 255 }, "Player's name:", fontSize);
			if (!app->input->playerName.empty())
			{
				const char* ch_name = app->input->playerName.c_str();	// SString to const char*	
				app->render->DrawText( height / 3 + fontSize * 7, 100,  { 255, 255, 255 }, ch_name, fontSize );
			}

			break;

		case DIALOGUE_SAVE:
			// see on DialogueSystem::OnGuiMouseClickEvent();
			break;
		case DIALOGUE_IF:


			break;
		default:
			return false;
			break;
		}
	}

	return true;
}

void DialogueTree::CleanUp()
{
	// TODO 5: Clean Up
	for (int j = 0; j < nodeList.size(); j++)
	{
		nodeList[j]->CleanUp();
		delete nodeList[j];
	}
	
	nodeList.clear();
}