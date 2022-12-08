#include "ModuleEditor.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleRenderExercise.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleCamera.h"
#include "ModuleTexture.h"
#include "./ImGui/imgui.h"
#include "./ImGui/imgui_impl_sdl.h"
#include "./ImGui/imgui_impl_opengl3.h"
#include "GL/glew.h"
#include <assimp/cimport.h>


ModuleEditor::ModuleEditor()
{
}

ModuleEditor::~ModuleEditor()
{
}

bool ModuleEditor::Init()
{
    //const char* glsl_version = "#version 440";


    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiWindowFlags_AlwaysAutoResize;
    io.ConfigFlags |= ImGuiBackendFlags_HasMouseHoveredViewport;

    //io.WantCaptureMouse;
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigWindowsMoveFromTitleBarOnly;

    return true;
}

bool ModuleEditor::Start()
{
    const char* glsl_version = "#version 440";

    ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer->glcontext);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ModuleTexture* text = new ModuleTexture();
    ret = text->LoadTextureFromFile("Baker_house.png", "");


    return true;
}

update_status ModuleEditor::PreUpdate(float dt)
{
    //Creation of frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(App->window->window);
    ImGui::NewFrame();
    return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update(float dt)
{
    float f = App->camera->getCameraSpeed();
    float horizontalFOV = App->camera->getHorizontalFOV();
    float nearPlane = App->camera->getNearPlane();
    float farPlane = App->camera->getFarPlane();
    float ratio = App->camera->getAspectRatio();
    float brightness = App->window->GetBrightness();
    bool fullscreen = App->window->IsFullscreen();
    int ramInMB = SDL_GetSystemRAM()/1000;
    const aiScene* scene = App->renderexercice->GetModel3D()->GetScene();
    //static ImGuiTextBuffer log;
    ImGui::Begin("Menu");


            if (ImGui::CollapsingHeader("Camera"))
            {
                    if (ImGui::Button("Reset Position")) {
                        App->camera->ResetPos();
                    }
                    if (ImGui::Button("Reset Camera")) {
                        App->camera->ResetCamera();
                    }
                    if (ImGui::Button("Reset FOV")) {
                        App->camera->ResetFOV();
                    }
                    if (ImGui::SliderFloat("Change Speed", &f, 1.0f, 5.0f)) {
                        App->camera->setCameraSpeed(f);
                    }
                    if (ImGui::SliderFloat("Change fov", &horizontalFOV, 0.0f, 3.0f)) {
                        App->camera->setPerspectiveFOV(horizontalFOV);
                    }
                    if (ImGui::SliderFloat("Change Aspect ratio", &ratio, 0.0f, 3.0f)) {
                        App->camera->setAspectRatio(ratio);
                    }
                    if (ImGui::SliderFloat("Change Near Plane", &nearPlane, 0.0f, 100.0f)) {
                        App->camera->setNearPlane(nearPlane);
                    }
                    if (ImGui::SliderFloat("Change Far Plane", &farPlane, 0.0f, 200.0f)) {
                        App->camera->setFarPlane(farPlane);
                    } 
            }
             if (ImGui::CollapsingHeader("Window"))
             {
                     if (ImGui::Checkbox("Fullscreen", &fullscreen)) {
                            App->window->SetFullscreen(fullscreen);
                     }
                     if (ImGui::SliderFloat("Brightness", &brightness, 0.0f, 1.0f)) {
                         App->window->SetBrightness(brightness);
                     }
             }
        
            if (ImGui::CollapsingHeader("Mesh"))
            {
                int my_image_width = 0;
                int my_image_height = 0;
                GLuint my_image_texture = 0;
                //bool ret = text->LoadTextureFromFile(text->imagepath, text->fbxTexturepath);
                ImGui::Text("Number of meshes = %d", scene->mNumMeshes);
                ImGui::Text("Number of Textures = %d", scene->mNumTextures);
                ImGui::Text("pointer = %p", my_image_texture);
                ImGui::Text("size = %d x %d", my_image_width, my_image_height);
                ImGui::Image((ImTextureID)ret, ImVec2(512, 512));
            }

            if (ImGui::CollapsingHeader("Hardware"))
            {
                //Graphic cards
                ImGui::TextColored(ImVec4(1.f, 0.1f, 0.1f, 1.00f), "Vendor: % s", glGetString(GL_VENDOR));
                ImGui::TextColored(ImVec4(1.f, 0.1f, 0.1f, 1.00f), "Renderer: %s", glGetString(GL_RENDERER));
                ImGui::TextColored(ImVec4(1.f, 0.1f, 0.1f, 1.00f), "OpenGL version supported %s", glGetString(GL_VERSION));
                ImGui::TextColored(ImVec4(1.f, 0.1f, 0.1f, 1.00f), "GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

            }
            if (ImGui::CollapsingHeader("Time")) {
                if (ImGui::Button("Start"))
                    timer.Start();
                ImGui::SameLine();
                if (ImGui::Button("Stop"))
                    timer.Stop();

                if (ImGui::Button("StartPerf"))
                    perfTimer.Start();
                ImGui::SameLine();
                if (ImGui::Button("StopPerf"))
                    perfTimer.Stop();

                ImGui::TextColored(ImVec4(0.5f, 1.0f, 1.0f, 1.00f), "Time %.2f ", timer.Read());
                ImGui::TextColored(ImVec4(0.5f, 1.0f, 1.0f, 1.00f), "Time performance %.2f ", perfTimer.Read());

                float fps = App->GetFPS();
                ImGui::SliderFloat("Change Far Plane", &fps, 1.0f, 200.0f);
                App->SetFPS(fps);

                float deltaTime = dt * 1000;
                float frame = 1.f / dt;
                fps_History.push_back(frame);
                ms_History.push_back(deltaTime);

                char Framerate[25];
                sprintf_s(Framerate, 25, "Framerate %.1f", fps_History[index]);
                ImGui::PlotHistogram("##framerate", &fps_History[0], 100, 0, Framerate, 0.f, 300.f, ImVec2(310, 160));

                char ms[25];
                sprintf_s(ms, 25, "ms %.1f", ms_History[index]);
                ImGui::PlotHistogram("##framerate", &ms_History[0], 100, 0, ms, 0.f, 30.f, ImVec2(310, 160));

                //limit the vector then delete the first element to let the next one take his place
                if (index < 100 - 1) {
                    ++index;
                }
                else {
                    fps_History.erase(fps_History.begin());
                    ms_History.erase(ms_History.begin());
                }
            }
            if (ImGui::CollapsingHeader("Console")) {
                ImGui::TextWrapped("Enter 'HELP' for help. (Doesn't work but it's for later)");
                for (int i = 0; i < log_History.size(); ++i) {
                    const char* log = log_History[i];
                    ImGui::TextUnformatted(log);
                }
            }


            ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


    return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate(float dt)
{
    //ImGui::Render();
    //ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    //SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
    //SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();

    return UPDATE_CONTINUE;
}

bool ModuleEditor::CleanUp()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(App->renderer->glcontext);
    SDL_DestroyWindow(App->window->window);
    SDL_Quit();
    return true;
}

//New call to do a Engine Log and also put it in the Editor console
void ModuleEditor::AddLog(const char* value)
{
    ENGINE_LOG(value);
    log_History.push_back(_strdup(value));
}