#pragma once
#include "Module.h"
#include "Globals.h"



class ModuleProgram : public Module
{
public:
	ModuleProgram();
	~ModuleProgram();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();
	char* LoadShaderSource(const char* shader_file_name);
	unsigned CompileShader(unsigned type, const char* source);
	unsigned CreateProgram(unsigned vtx_shader, unsigned frg_shader);

private: 

	unsigned program_id=NULL;
	friend class ModuleRenderExercise;
	friend class Mesh;

};

