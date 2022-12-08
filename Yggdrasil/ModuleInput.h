#pragma once
#include "Module.h"
#include "Globals.h"

typedef unsigned __int8 Uint8;

class ModuleInput : public Module
{
public:
	
	ModuleInput();
	~ModuleInput();

	bool Init();
	update_status Update(float dt);
	void setCameraSpeed(float speed);
	float getCameraSpeed();
	bool CleanUp();

private:
	const Uint8 *keyboard = NULL;
};