#include "LuaScript.h"
#include "../Logger/Logger.h"
#include "../GameObjects/GameObject.h"

class TransformComponent;

void SetEntityPosition(GameObject* entity, double x, double y)
{
    if (entity->HasComponent<TransformComponent>())
    {
        auto transform = entity->GetComponent<TransformComponent>();
        transform->Position.x = x;
        transform->Position.y = y;
    }
    else
    {
        Logger::Err("Trying to set the position of an entity that has no transform component");
    }
}

void LuaPrint(const std::string& message) {
    
    Logger::Lua(message);
}

void LuaScript::LevelSetupViaLua(std::unique_ptr<Registry>& registry)
{
    lua.open_libraries(sol::lib::base, sol::lib::math);
    lua.new_usertype<GameObject>(
        "gameobject",
        "get_id", &GameObject::GetId,
        "get_name", &GameObject::GetName
    );
    lua.set_function("set_position", SetEntityPosition);
    lua.set_function("mito_log",LuaPrint);

    sol::load_result luaScript = lua.load_file("./assets/scripts/main.lua");
    if (!luaScript.valid())
    {
        sol::error err = luaScript;
        std::string errorMessage = err.what();
        Logger::Err("Error loading the lua script: " + errorMessage);
        return;
    }
    // Executes the script using the Sol state
    lua.script_file("./assets/scripts/main.lua");

    // Read the big table for the current level
    sol::table level = lua["Level"];
    sol::table entities = level["entities"];
    int i = 1; // Lua table start at 1
    while (true)
    {
        sol::optional<sol::table> hasEntity = entities[i];
        if (hasEntity == sol::nullopt)
        {
            break;
        }

        sol::table entity = entities[i];

        std::string name;
        if (entity["name"] != sol::nil)
        {
            name.assign(entity["name"]);
        }
        std::unique_ptr<GameObject>& newGameObject = registry->CreateGameObject(name);
        
        // Components
        sol::optional<sol::table> hasComponents = entity["components"];
        if (hasComponents != sol::nullopt)
        {
            // Transform
            sol::optional<sol::table> transform = entity["components"]["transform"];
            if (transform != sol::nullopt)
            {
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
            if (rigidbody != sol::nullopt)
            {
                newGameObject->AddComponent<RigidBody2DComponent>(
                    glm::vec2(
                        entity["components"]["rigidbody"]["velocity"]["x"].get_or(0.0),
                        entity["components"]["rigidbody"]["velocity"]["y"].get_or(0.0)
                    )
                );
            }
            // BoxCollider
            sol::optional<sol::table> collider = entity["components"]["boxcollider"];
            if (collider != sol::nullopt)
            {
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
            if (sprite != sol::nullopt)
            {
                newGameObject->AddComponent<SpriteComponent>(
                    entity["components"]["sprite"]["sprite_name"],
                    entity["components"]["sprite"]["width"],
                    entity["components"]["sprite"]["height"],
                    entity["components"]["sprite"]["red"].get_or(255),
                    entity["components"]["sprite"]["green"].get_or(255),
                    entity["components"]["sprite"]["blue"].get_or(255)
                );
            }
            // Other Lua script add
            sol::optional<sol::table> scriptsTable = entity["components"]["scripts"];
            if (scriptsTable != sol::nullopt)
            {
                for (auto& scriptEntry : scriptsTable.value())
                {
                    std::string scriptPath = scriptEntry.second.as<std::string>();
                    sol::load_result script = lua.load_file("./assets/scripts/" + scriptPath);
                    Logger::Log(scriptPath);
                    if (!script.valid())
                    {
                        sol::error err = script;
                        std::string errorMessage = err.what();
                        Logger::Err("Error loading the script: " + errorMessage);
                    }
                    else
                    {
                        sol::function func = script;
                        newGameObject->AddComponent<ScriptComponent>(lua, func);
                    }
                }
            }
            // Script
            sol::optional<sol::table> updateScript = entity["components"]["on_update_script"];
            if (updateScript != sol::nullopt)
            {
                sol::function func = entity["components"]["on_update_script"][1];
                newGameObject->AddComponent<ScriptComponent>(lua, func);
            }
        }

        i++;
    }
}
