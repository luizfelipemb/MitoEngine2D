#pragma once
#include <sol.hpp>

#include "../GameObjects/GameObject.h"

class LuaScript
{
public:
    void SettingsSetup();
    void LevelSetupViaLua();
    void AddScriptComponent(GameObject* gameObject, const std::string& scriptName);
private:
    sol::state lua;
    const std::string SCRIPTS_PATH = "./assets/scripts/";
    const std::string LEVELS_PATH = "./assets/levels/";
    const std::string SETTINGS_PATH = "./assets/settings/";
    const std::string INITIAL_LEVEL_NAME = "main.lua";
};
