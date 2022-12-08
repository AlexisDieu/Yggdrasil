#include "ModuleRenderExercise.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleDebugDraw.h"
#include "ModuleCamera.h"
#include "ModuleTexture.h"
#include "Model.h"
#include "SDL.h"
#include "GL/glew.h"

#include "Math/float3x3.h"


ModuleRenderExercise::ModuleRenderExercise()
{
}

ModuleRenderExercise::~ModuleRenderExercise()
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteProgram(App->program->program_id);
}

bool ModuleRenderExercise::Init()
{
	//CreateTriangleVBO();
	//texture = App->texture->LoadTextureFromFile("Test-image-Baboon.bmp");
	//texture = App->texture->LoadTextureFromFile("wall.jpg");

	const char* vertexshadersource = App->program->LoadShaderSource("Shader/Uniforms.glsl");
	unsigned int shader = App->program->CompileShader(GL_VERTEX_SHADER, vertexshadersource);

	const char* vertexFragColor = App->program->LoadShaderSource("Shader/VertexFragColor.glsl");
	unsigned int frag = App->program->CompileShader(GL_FRAGMENT_SHADER, vertexFragColor);

	App->program->CreateProgram(shader,frag);

	//model = float4x4::FromTRS(float3(2.0f, 2.0f, 0.0f),
	//	float4x4::identity,
	//	float3(1.0f));

	model3D->Load("Model/BakerHouse.fbx");
	//model3D->Load("D:/UPC_School/Model/Table.fbx");


	//model = float4x4::FromTRS(float3(2.0f, 0.0f, 0.0f),
	//	float4x4::RotateZ(pi / 4.0f),
	//	float3(2.0f, 1.0f, 0.0f));

	return true;
}

float4x4 ModuleRenderExercise::getModel() {
	return model;
}

update_status ModuleRenderExercise::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleRenderExercise::Update(float dt)
{
	//RenderVBO(vbo);
	model3D->Draw();
	App->debugdraw->Draw(App->camera->GetView(), App->camera->GetProj(), SCREEN_WIDTH, SCREEN_HEIGHT);

	return UPDATE_CONTINUE;
}

update_status ModuleRenderExercise::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleRenderExercise::CleanUp()
{
	glDeleteBuffers(1, &vbo);
	return true;
}

unsigned ModuleRenderExercise::CreateTriangleVBO()
{
	//float vertices[] = {
	//-1.0f, -1.0f, 0.0f,
	// 1.0f, -1.0f, 0.0f,
	// 0.0f,  1.0f, 0.0f
	//};
	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f,   0.0f, 1.0f    // top left 
	};

	unsigned int indices[] = {
		0,2,1, //first triangle
		0,3,2 //seconde
	};

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // set vbo active
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); // set vbo active
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	return vbo;
}

void ModuleRenderExercise::DestroyVBO()
{
	glDeleteBuffers(1, &vbo);
}

void ModuleRenderExercise::RenderVBO(unsigned vbo)
{

	glUseProgram(App->program->program_id);
	glUniformMatrix4fv(0, 1, GL_TRUE, &App->camera->GetProj()[0][0]);
	glUniformMatrix4fv(1, 1, GL_TRUE, &App->camera->GetView()[0][0]);
	glUniformMatrix4fv(2, 1, GL_TRUE, &getModel()[0][0]);

	//gluOrtho2D(0,SCREEN_WIDTH,0,SCREEN_HEIGHT);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(5 * sizeof(float)));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	//glBindVertexArray(vbo);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	//glDrawArrays(GL_TRIANGLE_FAN, 0, 3);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	App->debugdraw->Draw(App->camera->GetView(), App->camera->GetProj(), SCREEN_WIDTH, SCREEN_HEIGHT);
}

void ModuleRenderExercise::SetNewModel(const char* path)
{
	delete model3D;
	model3D = new Model();
	model3D->Load(path);
}

Model* ModuleRenderExercise::GetModel3D()
{
	return model3D;
}