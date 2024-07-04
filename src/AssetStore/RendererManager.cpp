#include "RendererManager.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_sdl2.h"
#include "../imgui/imgui_impl_sdlrenderer2.h"
#include "../Logger/Logger.h"

int RendererManager::m_windowWidth;
int RendererManager::m_windowHeight;
int RendererManager::m_mapWidth;
int RendererManager::m_mapHeight;
SDL_Window* RendererManager::m_window;
SDL_Renderer* RendererManager::Renderer;

RendererManager::~RendererManager()
{
    SDL_DestroyRenderer(Renderer);
    SDL_DestroyWindow(m_window);
}

void RendererManager::Initialize()
{
    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);
    m_windowWidth = displayMode.w / 2;
    m_windowHeight = displayMode.h / 2;
    m_mapWidth = m_windowWidth;
    m_mapHeight = m_windowHeight;
    m_window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        m_windowWidth,
        m_windowHeight,
        NULL
    );
    if (!m_window) {
        Logger::Err("Error creating SDL window.");
        return;
    }
    Renderer = SDL_CreateRenderer(m_window, -1, 0);
    if (!Renderer) {
        Logger::Err("Error creating SDL renderer.");
        return;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForSDLRenderer(m_window, Renderer);
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
    //RenderImage("assets/images/radar.png", 0, 0, 100, 100, 1);
    //RenderText("TESTEEEE", "assets/fonts/arial.ttf", 100, 0, 0);

    ImGui::ShowDemoWindow();
    ImGui::Begin("Log Window");
    ImGui::End();
	
    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());

    SDL_RenderPresent(Renderer);
}
