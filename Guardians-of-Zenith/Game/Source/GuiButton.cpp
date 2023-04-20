#include "GuiButton.h"

GuiButton::GuiButton(uint32 id, SDL_Rect bounds, const char* text, int fontSize) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;
	this->fontSize = fontSize;
}

GuiButton::~GuiButton()
{
}

bool GuiButton::Update(float dt)
{
	if (state != GuiControlState::DISABLED)
	{
		app->input->GetMousePosition(mouseX, mouseY);
		//LOG("Mouse x: %d Mouse y: %d", mouseX, mouseY);
		//LOG("bounds.x: %d bounds.h: %d", bounds.x, bounds.y);

		GuiControlState previousState = state;

		if (mouseX >= bounds.x && mouseX <= bounds.x + bounds.w &&
			mouseY >= bounds.y && mouseY <= bounds.y + bounds.h) {
		
			state = GuiControlState::FOCUSED;
			
			if (previousState != state) 
			{
				//LOG("Change state from %d to %d", previousState, state);
			}

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
			{
				state = GuiControlState::PRESSED;
			}

			// If mouse button pressed -> Generate event!
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
			{
				NotifyObserver();
			}
		}
		else 
		{
			state = GuiControlState::NORMAL;
		}
	}

	return false;
}

bool GuiButton::Draw(Render* render)
{
	// Draw the right button depending on state
	switch (state)
	{

	case GuiControlState::DISABLED:
	{
		render->DrawRectangle({ bounds.x, bounds.y, bounds.w, bounds.h }, 200, 200, 200, 255, true, false);

	} break;

	case GuiControlState::NORMAL:
	{
		render->DrawRectangle({ bounds.x, bounds.y, bounds.w, bounds.h }, 0, 128, 255, 255, true, false);

	}	break;

	case GuiControlState::FOCUSED:
	{
		render->DrawRectangle({ bounds.x, bounds.y, bounds.w, bounds.h }, 255, 0, 255, 255, true, false);

	} break;

	case GuiControlState::PRESSED:
	{
		render->DrawRectangle({ bounds.x, bounds.y, bounds.w, bounds.h }, 0, 255, 0, 255, true, false);

	} break;

	case GuiControlState::SELECTED:
		break;
	}

	int size = fontSize;
	int x = bounds.w / size * 0.5;
	int y = bounds.h - size / 4;

	app->render->DrawText(bounds.x + x, bounds.y, { 255, 255, 255 }, text.GetString(), size);

	return false;
}