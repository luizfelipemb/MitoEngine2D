#pragma once

#include <memory>
#include "../GameObjects/GameObject.h"
#include "../LuaScripting/LuaScript.h"

class OpenLevelEvent;
class EventBus;

inline bool g_DebugMode = false;

class Game
{
public:
	Game();
	~Game();
	void Initialize();
	void SwitchLevel(OpenLevelEvent& levelName);
	void Run();
	void ProcessInput();
	void Update();
	void Destroy();
private:
	int m_millisecsPerFrame;
	bool m_isRunning = false;
	int m_msPreviousFrame = 0;
	std::string m_nextLevelName;
	bool m_canChangeLevel = false;
	std::unique_ptr<Registry> m_registry;
	LuaScript m_luaScript;
};
