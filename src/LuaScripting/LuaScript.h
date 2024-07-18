#pragma once
#include <sol.hpp>

#include "../GameObjects/GameObject.h"

class LuaScript
{
public:
    void LevelSetupViaLua(std::unique_ptr<Registry>& registry);
private:
    sol::state lua;
    const std::string SCRIPTS_PATH = "./assets/scripts/";
    const std::string LEVELS_PATH = "./assets/levels/";
    const std::string INITIAL_LEVEL_NAME = "main.lua";
};
