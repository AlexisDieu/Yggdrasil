#include <stdlib.h>
#include "Application.h"
#include "ModuleRender.h"
#include "Globals.h"

#include "SDL/include/SDL.h"
#pragma comment( lib, "SDL/lib/x64/SDL2.lib" )
#pragma comment( lib, "SDL/lib/x64/SDL2main.lib" )

#include "./ImGui/imgui.h"
#include "./ImGui/imgui_impl_sdl.h"
#include "./ImGui/imgui_impl_opengl3.h"
#include "./ImGui/imgui_impl_opengl3_loader.h"
#include <stdio.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif

enum main_states
{
	MAIN_CREATION,  
	MAIN_INIT,
	MAIN_START,
	MAIN_UPDATE,
	MAIN_FINISH,
	MAIN_EXIT
};

Application* App = NULL;

//void _CrtMemCheckpoint(
//	_CrtMemState* state
//);

int main(int argc, char ** argv)
{
	int main_return = EXIT_FAILURE;
	main_states state = MAIN_CREATION;

	while (state != MAIN_EXIT)
	{
		switch (state)
		{
		case MAIN_CREATION:

			ENGINE_LOG("Application Creation --------------");
			App = new Application();
			state = MAIN_INIT;
			break;

		case MAIN_INIT:

			ENGINE_LOG("Application Init --------------");
			if (App->Init() == false)
			{
				ENGINE_LOG("Application Init exits with error -----");
				state = MAIN_EXIT;
			}
			else
			{
				state = MAIN_START;
				ENGINE_LOG("Application Start --------------");
			}

			break;
		case MAIN_START:

			ENGINE_LOG("Application Start --------------");
			if (App->Start() == false)
			{
				ENGINE_LOG("Application Start exits with error -----");
				state = MAIN_EXIT;
			}
			else
			{
				state = MAIN_UPDATE;
				ENGINE_LOG("Application Update --------------");
			}

			break;

		case MAIN_UPDATE:
		{
			int update_return = App->Update();

			if (update_return == UPDATE_ERROR)
			{
				ENGINE_LOG("Application Update exits with error -----");
				state = MAIN_EXIT;
			}

			if (update_return == UPDATE_STOP)
				state = MAIN_FINISH;
		}
			break;

		case MAIN_FINISH:

			ENGINE_LOG("Application CleanUp --------------");
			if (App->CleanUp() == false)
			{
				ENGINE_LOG("Application CleanUp exits with error -----");
			}
			else
				main_return = EXIT_SUCCESS;

			state = MAIN_EXIT;

			break;

		}

	}

	delete App;
	ENGINE_LOG("Bye :)\n");
	return main_return;
}
