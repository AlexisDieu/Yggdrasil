#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleRenderExercise.h"
#include "ModuleCamera.h"
#include "ModuleWindow.h"
#include "SDL/include/SDL.h"
#include "./ImGui/imgui.h"
#include "./ImGui/imgui_impl_sdl.h"

ModuleInput::ModuleInput()
{}

// Destructor
ModuleInput::~ModuleInput()
{}

// Called before render is available
bool ModuleInput::Init()
{
    ENGINE_LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
        ENGINE_LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
update_status ModuleInput::Update(float dt)
{
    SDL_Event sdlEvent;

    while (SDL_PollEvent(&sdlEvent) != 0)
    {
        ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
        switch (sdlEvent.type)
        {
            case SDL_QUIT:
                return UPDATE_STOP;
            case SDL_WINDOWEVENT:
                if (sdlEvent.window.event == SDL_WINDOWEVENT_RESIZED || sdlEvent.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                    App->renderer->WindowResized(sdlEvent.window.data1, sdlEvent.window.data2);
                break;
            case SDL_DROPFILE:
                App->renderexercice->SetNewModel(sdlEvent.drop.file);
                break;
        }
    }

    keyboard = SDL_GetKeyboardState(NULL);
    if (keyboard[SDL_SCANCODE_ESCAPE]) {
        return UPDATE_STOP;
    }

    float cameraSpeed = App->camera->getCameraSpeed();
    float rotationspeed = App->camera->getRotationSpeed();
    if (keyboard[SDL_SCANCODE_LSHIFT]) 
    {
        cameraSpeed *=  2;
        rotationspeed *= 2;
    }

    //translate camera
    float3 Pos = float3::zero;
    if (keyboard[SDL_SCANCODE_D]) {
        Pos.x += cameraSpeed;
    }
    if (keyboard[SDL_SCANCODE_A]) {
        Pos.x -= cameraSpeed;
    }
    if (keyboard[SDL_SCANCODE_Q]) {
        Pos.y += cameraSpeed;
    }
    if (keyboard[SDL_SCANCODE_E]) {
        Pos.y -= cameraSpeed;
    }
    if (keyboard[SDL_SCANCODE_S]) {
        Pos.z += cameraSpeed;
    }
    if (keyboard[SDL_SCANCODE_W]) {
        Pos.z -= cameraSpeed;
    }
    App->camera->Translate(Pos,dt);

    int x,y;
    Uint32 mouse;

    SDL_PumpEvents();
    //Rotation camera with keyboard
    float3 rad = float3::zero;
    if (keyboard[SDL_SCANCODE_DOWN]) {
        rad.x -= rotationspeed;
    }
    if (keyboard[SDL_SCANCODE_UP]) {
        rad.x += rotationspeed;
    }
    if (keyboard[SDL_SCANCODE_LEFT]) {
        rad.y += rotationspeed;
    }
    if (keyboard[SDL_SCANCODE_RIGHT]) {
        rad.y -= rotationspeed;
    }
    App->camera->Rotate(rad,dt);

    //rotate the camera with the mouse (need to change the data send because it's limited by each movement and cause a lag effect
    bool Mousefirst = true;
    bool MouseMotion = false;
    float3 MouseRel = float3::zero;

    if (sdlEvent.type == SDL_MOUSEMOTION && sdlEvent.button.button == SDL_BUTTON_LEFT)
    {
        if (sdlEvent.motion.xrel > 0)
        {
            MouseRel.y -= rotationspeed;
        }
        if (sdlEvent.motion.xrel < 0)
        {
            MouseRel.y += rotationspeed;
        }
        if (sdlEvent.motion.yrel > 0)
        {
            MouseRel.x -= rotationspeed;
        }
        if (sdlEvent.motion.yrel < 0)
        {
            MouseRel.x += rotationspeed;
        }
    
        App->camera->Rotate(MouseRel,dt);
    }

    //Allow to Zoom and Dezoom
    float horizontalFOV = App->camera->getHorizontalFOV();
    if (sdlEvent.type == SDL_MOUSEWHEEL) 
    {
        if (sdlEvent.wheel.y > 0)
        {
            if (horizontalFOV > 0.1)
            horizontalFOV -= 0.02;
        }     
        if (sdlEvent.wheel.y < 0)
        {
            if(horizontalFOV<3)
            horizontalFOV += 0.02;
        }
        App->camera->setPerspectiveFOV(horizontalFOV);
    }
    //focus the camera on the model
    if (keyboard[SDL_SCANCODE_F])
        App->camera->Focus(App->renderexercice->GetModel3D()->GetAABB()->CenterPoint());

    //orbit around a model
    float3 MouseOrbit = float3::zero;
    mouse = SDL_GetMouseState(&x, &y);
    if (keyboard[SDL_SCANCODE_LALT])
    {
        if (sdlEvent.motion.xrel > 0)
        {
            MouseOrbit.x -= mouse;
        }
        if (sdlEvent.motion.xrel < 0)
        {
            MouseOrbit.x += mouse;
        }
        if (sdlEvent.motion.yrel > 0)
        {
            MouseOrbit.y -= mouse;
        }
        if (sdlEvent.motion.yrel < 0)
        {
            MouseOrbit.y += mouse;
        }
        //App->camera->Rotate(MouseRel, dt);
        App->camera->Orbit(App->renderexercice->GetModel3D()->GetAABB()->CenterPoint(), MouseOrbit, dt);
    }


    return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
    ENGINE_LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}
