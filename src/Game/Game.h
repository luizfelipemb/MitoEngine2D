#pragma once

#include <memory>
#include "../GameObjects/GameObject.h"

class EventBus;
constexpr int FPS = 60;
constexpr int MILLISECS_PER_FRAME = 1000 / FPS;

class Game
{
public:
	Game();
	~Game();
	void Initialize();
	void LevelSetupViaLua();
	void Run();
	void ProcessInput();
	void Update();
	void Destroy();

private:
	bool m_isRunning;
	int m_msPreviousFrame = 0;
	std::unique_ptr<Registry> m_registry;
	sol::state lua;
};
