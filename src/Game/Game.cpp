#include <sol.hpp>
#include "./Game.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <glm.hpp>

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


Game::Game():
m_registry(std::make_unique<Registry>())
{
	m_isRunning = false;
	Logger::Log("Game constructor called!");
}

Game::~Game()
{
	Logger::Log("Game destructor called!");
}

void Game::ProcessInput()
{
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
			m_isRunning = false;
			break;
		case SDL_KEYDOWN:
			if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
				m_isRunning = false;
			}
			if(sdlEvent.key.repeat == 0)
			{
				GlobalEventBus::EmitEvent<KeyPressedEvent>(sdlEvent.key.keysym.sym);
			}
			break;
		case SDL_KEYUP:
			GlobalEventBus::EmitEvent<KeyReleasedEvent>(sdlEvent.key.keysym.sym);
			break;
		}
	}
}

void Game::Initialize()
{
	AssetManager::Initialize();
	RendererManager::Initialize();
	m_isRunning = true;
	
	LevelSetupViaLua();

	/*
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

	std::unique_ptr<GameObject>& enemy2 = m_registry->CreateGameObject();
	enemy2->AddComponent<TransformComponent>(glm::vec2(300, 50));
	enemy2->AddComponent<SpriteComponent>("assets/images/RoundedSquare.png",100, 100, 255, 0, 0);
	enemy2->AddComponent<BoxCollider2DComponent>(100, 100);
	*/
}

void Game::LevelSetupViaLua()
{
	lua.open_libraries(sol::lib::base, sol::lib::math);
	sol::load_result script = lua.load_file("./assets/scripts/Level1.lua");
	lua.new_usertype<GameObject>(
			"gameobject",
			"get_id", &GameObject::GetId
		);
	//lua.set_function("get_id", &GameObject::GetId );
	
	if (!script.valid()) {
		sol::error err = script;
		std::string errorMessage = err.what();
		Logger::Err("Error loading the lua script: " + errorMessage);
		return;
	}
	// Executes the script using the Sol state
	lua.script_file("./assets/scripts/Level1.lua");

	// Read the big table for the current level
	sol::table level = lua["Level"];
	sol::table entities = level["entities"];
	int i = 0;
	while (true)
	{
		sol::optional<sol::table> hasEntity = entities[i];
		if (hasEntity == sol::nullopt) {
			break;
		}

		sol::table entity = entities[i];
		std::unique_ptr<GameObject>& newGameObject = m_registry->CreateGameObject();
		// Components
		sol::optional<sol::table> hasComponents = entity["components"];
		if (hasComponents != sol::nullopt)
		{
			// Transform
			sol::optional<sol::table> transform = entity["components"]["transform"];
			if (transform != sol::nullopt) {
				newGameObject->AddComponent<TransformComponent>(
					glm::vec2(
						entity["components"]["transform"]["position"]["x"],
						entity["components"]["transform"]["position"]["y"]
					),
					entity["components"]["transform"]["scale"].get_or(1.0),
					entity["components"]["transform"]["rotation"].get_or(0.0)
				);
			}
			// RigidBody
			sol::optional<sol::table> rigidbody = entity["components"]["rigidbody"];
			if (rigidbody != sol::nullopt) {
				newGameObject->AddComponent<RigidBody2DComponent>(
					glm::vec2(
						entity["components"]["rigidbody"]["velocity"]["x"].get_or(0.0),
						entity["components"]["rigidbody"]["velocity"]["y"].get_or(0.0)
					)
				);
			}
			// BoxCollider
			sol::optional<sol::table> collider = entity["components"]["boxcollider"];
			if (collider != sol::nullopt) {
				newGameObject->AddComponent<BoxCollider2DComponent>(
					entity["components"]["boxcollider"]["width"],
					entity["components"]["boxcollider"]["height"],
					glm::vec2(
						entity["components"]["boxcollider"]["offset"]["x"].get_or(0),
						entity["components"]["boxcollider"]["offset"]["y"].get_or(0)
					)
				);
			}
			// Sprite
			sol::optional<sol::table> sprite = entity["components"]["sprite"];
			if (sprite != sol::nullopt) {
				newGameObject->AddComponent<SpriteComponent>(
					entity["components"]["sprite"]["sprite_name"],
					entity["components"]["sprite"]["width"],
					entity["components"]["sprite"]["height"],
					entity["components"]["sprite"]["red"].get_or(255),
					entity["components"]["sprite"]["green"].get_or(255),
					entity["components"]["sprite"]["blue"].get_or(255)
				);
			}
			// Script
			sol::optional<sol::table> script = entity["components"]["on_update_script"];
			if (script != sol::nullopt) {
				sol::function func = entity["components"]["on_update_script"][0];
				newGameObject->AddComponent<ScriptComponent>(lua, func);
			}
		}

		i++;
	}
}

void Game::Run()
{
	while (m_isRunning)
	{
		RendererManager::ClearFrameRender();
		ProcessInput();
		Update();
		RendererManager::DrawFrameRender();
	}
}

void Game::Update()
{
	// If we are too fast, waste some time until we reach the MILLISECS_PER_FRAME
	int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - m_msPreviousFrame);
	if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME) {
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
