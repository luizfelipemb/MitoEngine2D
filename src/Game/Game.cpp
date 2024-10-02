#include <sol.hpp>
#include "./Game.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <glm.hpp>
#include <utility>

#include "../GameObjects/GameObject.h"
#include "../Logger/Logger.h"
#include "../AssetStore/AssetManager.h"
#include "../AssetStore/RendererManager.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_sdl2.h"
#include "../imgui/imgui_impl_sdlrenderer2.h"
#include "../Events/KeyPressedEvent.h"
#include "../Events/EventBus.h"
#include "../Events/KeyReleasedEvent.h"
#include "../Events/OpenLevelEvent.h"

class MouseButtonPressedEvent;

Game::Game():
    m_registry(std::make_unique<Registry>())
{
    Logger::Log("Game constructor called!");
}

Game::~Game()
{
    m_registry->ClearGameObjects();
    Logger::Log("Game destructor called!");
}

void Game::ProcessInput()
{
    SDL_Event sdlEvent;
    sdlEvent.key.repeat = 0;
    while (SDL_PollEvent(&sdlEvent))
    {
        ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
        ImGuiIO& io = ImGui::GetIO();
        int mouseX, mouseY;
        const int buttons = SDL_GetMouseState(&mouseX, &mouseY);
        io.MousePos = ImVec2(mouseX, mouseY);
        io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
        io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);

        // Handle core SDL events (close window, key pressed, etc.)
        switch (sdlEvent.type)
        {
        case SDL_QUIT:
            m_isRunning = false;
            break;
        case SDL_KEYDOWN:
            if (sdlEvent.key.keysym.sym == SDLK_ESCAPE)
            {
                m_isRunning = false;
            }
            if(sdlEvent.key.keysym.sym == SDLK_F3)
            {
                g_DebugMode = !g_DebugMode;
            }
            if (sdlEvent.key.repeat == 0)
            {
                GlobalEventBus::EmitEvent<KeyPressedEvent>(sdlEvent.key.keysym.sym);
            }
            break;
        case SDL_KEYUP:
            GlobalEventBus::EmitEvent<KeyReleasedEvent>(sdlEvent.key.keysym.sym);
            break;
        case SDL_MOUSEBUTTONDOWN:
            GlobalEventBus::EmitEvent<MouseButtonPressedEvent>(sdlEvent.button.button, glm::vec2( sdlEvent.button.x, sdlEvent.button.y));
            break;
        case SDL_MOUSEBUTTONUP:
            //GlobalEventBus::EmitEvent<MouseButtonReleasedEvent>(sdlEvent.button.button, sdlEvent.button.x, sdlEvent.button.y);
            break;
        }
    }
}

void Game::Initialize()
{
    m_luaScript.SettingsSetup();
    m_millisecsPerFrame = 1000 / WindowSettings::FPS;
    AssetManager::Initialize();
    RendererManager::Initialize();
    m_isRunning = true;
    GlobalEventBus::SubscribeToEvent<OpenLevelEvent>(this, &Game::SwitchLevel);
    
    m_luaScript.LoadLuaBindings();
    m_luaScript.LoadLevel("main.lua");
    //auto& x = m_registry->CreateGameObject();
    //x->AddComponent<TransformComponent>(glm::vec2(0,0));
    //x->AddComponent<SpriteComponent>("assets/images/explosion.png",64,64);
    //x->AddComponent<AnimationComponent>(13,20,true);
    m_registry->Start();
   
}

void Game::SwitchLevel(OpenLevelEvent& levelEvent)
{
    Logger::Log("level changed to: "+levelEvent.LevelName);
    m_canChangeLevel = true;
    m_nextLevelName = levelEvent.LevelName;
}

void Game::Run()
{
    while (m_isRunning)
    {
        RendererManager::ClearFrameRender();
        ProcessInput();
        Update();
        RendererManager::DrawFrameRender();
        if(m_canChangeLevel)
        {
            m_canChangeLevel = false;
            m_registry->ClearGameObjects();
            m_registry->ClearTags();
            m_luaScript.LoadLevel(m_nextLevelName);
            m_registry->Start();
        }
    }
}

void Game::Update()
{
    // If we are too fast, waste some time until we reach the MILLISECS_PER_FRAME
    int timeToWait = m_millisecsPerFrame - (SDL_GetTicks() - m_msPreviousFrame);
    if (timeToWait > 0 && timeToWait <= m_millisecsPerFrame)
    {
        SDL_Delay(timeToWait);
    }

    // The difference in ticks since the last frame, converted to seconds
    double deltaTime = (SDL_GetTicks() - m_msPreviousFrame) / 1000.0;
    // Store the "previous" frame time
    m_msPreviousFrame = SDL_GetTicks();

    m_registry->Update(deltaTime);
}

void Game::Destroy()
{
    SDL_Quit();

    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}
