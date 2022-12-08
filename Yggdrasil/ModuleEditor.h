#pragma once
#include "Module.h"
#include "Globals.h"
#include "Timer.h"
#include <vector>
#include "ModuleTexture.h"

class ModuleTexure;

class ModuleEditor : public Module
{
public:
	ModuleEditor();
	~ModuleEditor();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();
	void AddLog(const char* entry);
private:

	Timer timer;
	PerformanceTimer perfTimer;
	std::vector<char*> log_History;
	std::vector<float> fps_History;
	std::vector<float> ms_History;
	int index = 0;

	bool ret;
	ModuleTexture* texture;
};

