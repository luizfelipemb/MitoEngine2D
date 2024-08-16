#pragma once
#include <sol.hpp>

#include "../GameObjects/GameObject.h"

class LuaScript
{
public:
    void SettingsSetup();
    void LoadLuaBindings();
    void EmitOpenLevel(const std::string& levelName);
    void LoadLevel(std::string name);
    void AddScriptComponent(GameObject* gameObject, const std::string& scriptName);
private:
    sol::state lua;
    const std::string SCRIPTS_PATH = "./assets/scripts/";
    const std::string LEVELS_PATH = "./assets/levels/";
    const std::string SETTINGS_PATH = "./assets/settings/";
};
