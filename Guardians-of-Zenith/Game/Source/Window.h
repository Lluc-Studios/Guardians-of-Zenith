#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "Module.h"
#include "SDL/include/SDL.h"

struct SDL_Window;
struct SDL_Surface;

class Window : public Module
{
public:

	Window();

	// Destructor
	virtual ~Window();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Changae title
	void SetTitle(const char* title);

	// Retrive window size
	void GetWindowSize(uint& width, uint& height) const;

	// Retrieve window scale
	uint GetScale() const;

	bool Fullscreen();

public:
	// The window we'll be rendering to
	SDL_Window* window;

	// The surface contained by the window
	SDL_Surface* screenSurface;

	bool fullscreen;
	bool borderless;
	bool resizable;
	bool fullscreen_window;

	Uint32 flags = SDL_WINDOW_SHOWN;
private:


	SString title;
	uint width;
	uint height;
	uint scale;
};

#endif // __WINDOW_H__