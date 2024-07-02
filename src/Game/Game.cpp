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
#include "../Events/KeyPressedEvent.h"
#include "../Events/EventBus.h"
#include "../Events/KeyReleasedEvent.h"


Game::Game():
m_registry(std::make_unique<Registry>())
{
	isRunning = false;
	Logger::Log("Game constructor called!");
}

Game::~Game() {
	Logger::Log("Game destructor called!");
}

void Game::ProcessInput() {
	SDL_Event sdlEvent;
	sdlEvent.key.repeat = 0;
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
			if(sdlEvent.key.repeat == 0)
			{
				EventBus::EmitEvent<KeyPressedEvent>(sdlEvent.key.keysym.sym);
			}
			break;
		case SDL_KEYUP:
			EventBus::EmitEvent<KeyReleasedEvent>(sdlEvent.key.keysym.sym);
			break;
		}
	}
}

void Game::Initialize() {

	AssetManager::Initialize();
	isRunning = true;

	std::unique_ptr<GameObject>& player = m_registry->CreateGameObject();
	player->AddComponent<TransformComponent>(glm::vec2(50, 100));
	player->AddComponent<SpriteComponent>("assets/images/RoundedSquare.png",50,50,0,255,0);
	player->AddComponent<ControllerComponent>();
	player->AddComponent<RigidBody2DComponent>();
	player->AddComponent<BoxCollider2DComponent>(50, 50);

	
	std::unique_ptr<GameObject>& enemy = m_registry->CreateGameObject();
	enemy->AddComponent<TransformComponent>(glm::vec2(200, 50));
	enemy->AddComponent<SpriteComponent>("assets/images/RoundedSquare.png",100, 100, 255, 0, 0);
	enemy->AddComponent<RigidBody2DComponent>(glm::vec2(0, 20));
	enemy->AddComponent<BoxCollider2DComponent>(100, 100);
	
}

void Game::Run()
{
	while (isRunning)
	{
		AssetManager::ClearFrameRender();
		ProcessInput();
		Update();
		AssetManager::DrawFrameRender();
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

	m_registry->Update(deltaTime);
}




void Game::Destroy() {
	SDL_Quit();

	ImGui_ImplSDLRenderer2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}
