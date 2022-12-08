#pragma once
#include "Module.h"
#include "Globals.h"
#include "ModuleProgram.h"
#include "Geometry/Frustum.h"
#include "Model.h"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleRenderExercise : public Module
{
public:
	ModuleRenderExercise();
	~ModuleRenderExercise();

	bool Init();
	float4x4 getModel();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();
	unsigned CreateTriangleVBO();
	void DestroyVBO();
	void RenderVBO(unsigned vbo);

	void SetNewModel(const char* path);

	Model* GetModel3D();

private:
		float4x4 model;
		unsigned int vbo,ebo;
		unsigned texture;

		Model* model3D = new Model();
		Frustum frustum;

		friend class ModuleInput;

};

