#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow();

	// Destructor
	virtual ~ModuleWindow();

	// Called before quitting
	bool Init();

	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);

	// Called before quitting
	bool CleanUp();

	void SetFullscreen(bool set);

	void SetBrightness(float brightness);

	bool IsFullscreen();

	float GetBrightness() const;

public:
	//The window we'll be rendering to
	SDL_Window* window = NULL;

	//The surface contained by the window
	SDL_Surface* screen_surface = NULL;

	bool fullscreen = FULLSCREEN;
};

#endif // __ModuleWindow_H__