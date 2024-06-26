#include "./Game.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <glm.hpp>
#include "../GameObjects/GameObject.h"
#include "../Logger/Logger.h"
#include "../AssetStore/AssetManager.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_sdl2.h"
#include "../imgui/imgui_impl_sdlrenderer2.h"

Game::Game(): m_registry(std::make_unique<Registry>())
{
	isRunning = false;
	Logger::Log("Game constructor called!");
}

Game::~Game() {
	Logger::Log("Game destructor called!");
}

void Game::Initialize() {

	AssetManager::Initialize();
	isRunning = true;

	GameObject* player = m_registry->CreateGameObject();
	player->AddComponent<TransformComponent>();
	player->AddComponent<SpriteComponent>("assets/images/radar.png");
}

void Game::ProcessInput() {
	SDL_Event sdlEvent;
	while (SDL_PollEvent(&sdlEvent)) {

		ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
		ImGuiIO& io = ImGui::GetIO();
		int mouseX, mouseY;
		const int buttons = SDL_GetMouseState(&mouseX, &mouseY);
		io.MousePos = ImVec2(mouseX, mouseY);
		io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
		io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);

		// Handle core SDL events (close window, key pressed, etc.)
		switch (sdlEvent.type) {
		case SDL_QUIT:
			isRunning = false;
			break;
		case SDL_KEYDOWN:
			if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
				isRunning = false;
			}
			//eventBus->EmitEvent<KeyPressedEvent>(sdlEvent.key.keysym.sym);
			break;
		}
	}
}

void Game::Update() {
	// If we are too fast, waste some time until we reach the MILLISECS_PER_FRAME
	int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecsPreviousFrame);
	if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME) {
		SDL_Delay(timeToWait);
	}

	// The difference in ticks since the last frame, converted to seconds
	double deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0;

	// Store the "previous" frame time
	millisecsPreviousFrame = SDL_GetTicks();

	m_registry->Update();
}

void Game::Run()
{
	while (isRunning) {
		AssetManager::ClearFrameRender();
		ProcessInput();
		Update();
		AssetManager::DrawFrameRender();
	}
}


void Game::Destroy() {
	SDL_Quit();

	ImGui_ImplSDLRenderer2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}
