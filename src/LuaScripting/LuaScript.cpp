#include "LuaScript.h"
#include "../Logger/Logger.h"
#include "../GameObjects/GameObject.h"

void LuaPrint(const std::string& message) {
    
    Logger::Lua(message);
}
void Destroy(int id)
{
    Registry::DestroyGameObject(id);
}
GameObject* CreateGameObject(std::string name)
{
    return Registry::CreateGameObject(name).get();
}

void AddTransformComponent(GameObject* gameObject,
    sol::optional<glm::vec2> position,
    sol::optional<double> scale,
    sol::optional<double> rotation)
{
    gameObject->AddComponent<TransformComponent>(
        position ? std::optional<glm::vec2>(*position) : std::nullopt,
        scale ? std::optional<double>(*scale) : std::nullopt,
        rotation ? std::optional<double>(*rotation) : std::nullopt);
}
void AddSpriteComponent(GameObject* gameObject,
    const std::string& sprite,
    sol::optional<int> width,
    sol::optional<int> height,
    sol::optional<std::uint8_t> red,
    sol::optional<std::uint8_t> green,
    sol::optional<std::uint8_t> blue)
{
    gameObject->AddComponent<SpriteComponent>(
        sprite,
        width ? std::optional<int>(*width) : std::nullopt,
        height ? std::optional<int>(*height) : std::nullopt,
        red ? std::optional<std::uint8_t>(*red) : std::nullopt,
        green ? std::optional<std::uint8_t>(*green) : std::nullopt,
        blue ? std::optional<std::uint8_t>(*blue) : std::nullopt);
}

void AddRigidBody2DComponent(GameObject* gameObject,
    sol::optional<glm::vec2> velocity)
{
    gameObject->AddComponent<RigidBody2DComponent>(
        velocity ? std::optional<glm::vec2>(*velocity) : std::nullopt);
}

void AddBoxCollider2DComponent(GameObject* gameObject,
    sol::optional<int> width,
    sol::optional<int> height,
    sol::optional<glm::vec2> offset)
{
    gameObject->AddComponent<BoxCollider2DComponent>(
        width ? std::optional<int>(*width) : std::nullopt,
        height ? std::optional<int>(*height) : std::nullopt,
        offset ? std::optional<glm::vec2>(*offset) : std::nullopt);
}

void LuaScript::SettingsSetup()
{
    Logger::Log("SettingsSetup");
    lua.open_libraries(sol::lib::base, sol::lib::math);
    sol::load_result luaScript = lua.load_file(SETTINGS_PATH+"settings.lua");
    if (!luaScript.valid())
    {
        sol::error err = luaScript;
        std::string errorMessage = err.what();
        Logger::Err("Error loading the lua script: " + errorMessage);
        return;
    }
    lua.script_file(SETTINGS_PATH+"settings.lua");
    sol::table settings = lua["Settings"];
    sol::table window = settings["window"];
    int windowHeight = window["height"];
    int windowWidth = window["width"];
    RendererManager::WindowHeight = windowHeight;
    RendererManager::WindowWidth = windowWidth;
    Logger::Log("Window Height:"+std::to_string(windowHeight));
}

void LuaScript::LevelSetupViaLua()
{
    lua.open_libraries(sol::lib::base, sol::lib::math);
    lua.new_usertype<glm::vec2>(
            "vec2",
            sol::constructors<glm::vec2(), glm::vec2(float, float)>(),
            "x", &glm::vec2::x,
            "y", &glm::vec2::y
        );
    lua.new_usertype<TransformComponent>(
            "transform_component",
            "position", &TransformComponent::Position,
            "scale", &TransformComponent::Scale,
            "rotation", &TransformComponent::Rotation
        );
    lua.new_usertype<RigidBody2DComponent>(
            "rigidbody_component",
            "velocity", &RigidBody2DComponent::Velocity
        );
    lua.new_usertype<ScriptComponent>(
            "script_component",
            "get_function", &ScriptComponent::GetScriptFunction
        );
    lua.new_usertype<GameObject>(
        "gameobject",
        "get_id", &GameObject::GetId,
        "get_name", &GameObject::GetName,
        
        "add_component_transform", &AddTransformComponent,
        "add_component_sprite", &AddSpriteComponent,
        "add_component_boxcollider", &AddBoxCollider2DComponent,
        "add_component_rigidbody", &AddRigidBody2DComponent,
        
        "get_component_transform", &GameObject::GetComponent<TransformComponent>,
        "get_component_sprite", &GameObject::GetComponent<SpriteComponent>,
        "get_component_boxcollider", &GameObject::GetComponent<BoxCollider2DComponent>,
        "get_component_rigidbody", &GameObject::GetComponent<RigidBody2DComponent>,
        "get_component_script", &GameObject::GetComponent<ScriptComponent>
    );
    
    lua.set_function("create",CreateGameObject);
    lua.set_function("destroy", Destroy);
    lua.set_function("mito_log",LuaPrint);

    sol::load_result luaScript = lua.load_file(LEVELS_PATH+INITIAL_LEVEL_NAME);
    if (!luaScript.valid())
    {
        sol::error err = luaScript;
        std::string errorMessage = err.what();
        Logger::Err("Error loading the lua script: " + errorMessage);
        return;
    }
    // Executes the script using the Sol state
    lua.script_file(LEVELS_PATH+INITIAL_LEVEL_NAME);

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
        std::unique_ptr<GameObject>& newGameObject = Registry::CreateGameObject(name);
        
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
                newGameObject->AddComponent<ScriptComponent>();
                for (auto& scriptEntry : scriptsTable.value())
                {
                    std::string scriptName = scriptEntry.second.as<std::string>();

                    // Create a new environment table
                    //TODO: load all scripts previously and add to a map and assign here according
                    sol::environment scriptEnv(lua, sol::create, lua.globals());
                    
                    sol::load_result script = lua.load_file(SCRIPTS_PATH + scriptName);
                    Logger::Log(scriptName);
                    if (!script.valid())
                    {
                        sol::error err = script;
                        std::string errorMessage = err.what();
                        Logger::Err("Error loading the script: " + errorMessage);
                    } else
                    {
                        lua.script_file(SCRIPTS_PATH + scriptName, scriptEnv);
                        newGameObject->GetComponent<ScriptComponent>()->AddScript(scriptEnv);
                    }
                }
            }
        }

        i++;
    }
}
