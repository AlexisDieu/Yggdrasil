#pragma once

#include<list>
#include "Globals.h"
#include "Module.h"
#include "Timer.h"

class ModuleRender;
class ModuleWindow;
class ModuleInput;
class ModuleRenderExercise;
class ModuleProgram;
class ModuleDebugDraw;
class ModuleEditor;
class ModuleCamera;
class ModuleTexture;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	bool Start();
	update_status Update();
	bool CleanUp();

	int GetFPS();
	void SetFPS(int value);

public:
	ModuleRender* renderer = nullptr;
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleRenderExercise* renderexercice = nullptr;
	ModuleProgram* program = nullptr;
	ModuleDebugDraw* debugdraw = nullptr;
	ModuleEditor* editor = nullptr;
	ModuleCamera* camera = nullptr;
	ModuleTexture* texture = nullptr;

private:

	std::list<Module*> modules;
	float dt;
	Timer timer;
	int fps = 60;

	int lastFrame, oldFrame;

};

extern Application* App;
