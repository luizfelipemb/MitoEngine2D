#include "RendererManager.h"

#include <iostream>
#include <ostream>
#include <SDL_image.h>

#include "AssetManager.h"
#include "../Game/Game.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_sdl2.h"
#include "../imgui/imgui_impl_sdlrenderer2.h"
#include "../Logger/Logger.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_sdl2.h"
#include "../imgui/imgui_impl_sdlrenderer2.h"
#include "../Logger/Logger.h"

int WindowSettings::WindowWidth;
int WindowSettings::WindowHeight;
std::string WindowSettings::WindowName;
std::string WindowSettings::IconImageLocation;
SDL_Window* RendererManager::Window;
SDL_Renderer* RendererManager::Renderer;

RendererManager::~RendererManager()
{
    SDL_DestroyRenderer(Renderer);
    SDL_DestroyWindow(Window);
}

void RendererManager::Initialize()
{
    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);
    //WindowWidth = displayMode.w / 2;
    //WindowHeight = displayMode.h / 2;
    Window = SDL_CreateWindow(
        WindowSettings::WindowName.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WindowSettings::WindowWidth,
        WindowSettings::WindowHeight,
        0
    );

    //Load Icon
    SDL_Surface* icon = IMG_Load(WindowSettings::IconImageLocation.c_str());
    if (icon != nullptr)
    {
        SDL_SetWindowIcon(Window, icon);
    }
    else
    {
        std::cerr << "IMG_Load Error: " << IMG_GetError() << '\n';
    }

    if (!Window)
    {
        Logger::Err("Error creating SDL window.");
        return;
    }
    Renderer = SDL_CreateRenderer(Window, -1, 0);
    if (!Renderer)
    {
        Logger::Err("Error creating SDL renderer.");
        return;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForSDLRenderer(Window, Renderer);
    ImGui_ImplSDLRenderer2_Init(Renderer);
}

void RendererManager::ClearFrameRender()
{
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    SDL_SetRenderDrawColor(Renderer, 21, 21, 21, 255);
    SDL_RenderClear(Renderer);
}

void RendererManager::DrawFrameRender()
{
    if (g_DebugMode)
    {
        ImGui::Begin("Log Window");
        for (auto& element : Logger::Messages)
        {
            ImVec4 color;
            switch (element.Type)
            {
            case LOG_INFO:
                color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
                break;
            case LOG_ERROR:
                color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
                break;
            case LOG_LUA:
                color = ImVec4(0.12f, 0.69f, 1.f, 1.f);
                break;
            default:
                color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
                break;
            }
            ImGui::PushStyleColor(ImGuiCol_Text, color);
            ImGui::Text("%s", element.Message.c_str());
            ImGui::PopStyleColor();
            ImGui::SetScrollHereY(1.0f); // Scroll to the bottom
        }
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());

    SDL_RenderPresent(Renderer);
}
