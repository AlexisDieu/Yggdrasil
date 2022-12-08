#pragma once
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleProgram.h"
#include "ModuleRenderExercise.h"
#include "ModuleEditor.h"
#include "ModuleDebugDraw.h"
#include "ModuleCamera.h"
#include "ModuleTexture.h"
#include "Timer.h"


using namespace std;

Application::Application()
{
	// Order matters: they will Init/start/update in this order
	modules.push_back(window = new ModuleWindow());
	modules.push_back(renderer = new ModuleRender());
	modules.push_back(texture = new ModuleTexture());
	modules.push_back(input = new ModuleInput());
	modules.push_back(camera = new ModuleCamera());
	modules.push_back(renderexercice = new ModuleRenderExercise());
	modules.push_back(program = new ModuleProgram());
	modules.push_back(editor = new ModuleEditor());
	modules.push_back(debugdraw = new ModuleDebugDraw());
}

Application::~Application()
{
	for(list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
    {
        delete *it;
    }
}

bool Application::Init()
{
	bool ret = true;
	Timer time;
	time.Start();
	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init();

	time.Stop();
	ENGINE_LOG(" Time to load Application Init %.2f ms", time.Read());
	return ret;
}

bool Application::Start()
{
	bool ret = true;

	for (list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Start();

	return ret;
}

update_status Application::Update()
{
	//create delta time and start the timer
	dt = (float)timer.Read() / 1000.0f;
	timer.Start();

	update_status ret = UPDATE_CONTINUE;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PreUpdate(dt);

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->Update(dt);

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PostUpdate(dt);

	//Limite the fps
	lastFrame = timer.Read();
	if (1000/fps > 0 && lastFrame < 1000/fps) {
		SDL_Delay(1000/fps - lastFrame);
	}

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for(list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		ret = (*it)->CleanUp();

	return ret;
}

int Application::GetFPS() {
	return fps;
}

void Application::SetFPS(int value) {
	fps = value;
}
