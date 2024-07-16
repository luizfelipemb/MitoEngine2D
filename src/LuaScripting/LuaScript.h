#pragma once
#include <sol.hpp>

#include "../GameObjects/GameObject.h"

class LuaScript
{
public:
    void LevelSetupViaLua(std::unique_ptr<Registry>& registry);
private:
    sol::state lua;
};
