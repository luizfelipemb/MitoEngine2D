#include "LuaScript.h"

#include "../Events/OpenLevelEvent.h"
#include "../Logger/Logger.h"
#include "../GameObjects/GameObject.h"

void LuaPrint(const std::string& message) {
    
    Logger::Lua(message);
}
void Destroy(GameObject* go)
{
    Registry::DestroyGameObject(go->GetId());
}
GameObject* CreateGameObject(sol::optional<std::string> name)
{
    return Registry::CreateGameObject(name.value_or("")).get();
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
void AddTextComponent(GameObject* gameObject,
    std::string& text,
    std::string& font,
    sol::optional<int> scale,
    sol::optional<std::uint8_t> red,
    sol::optional<std::uint8_t> green,
    sol::optional<std::uint8_t> blue)
{
    gameObject->AddComponent<TextComponent>(
        text,
        font,
        scale ? std::optional<int>(*scale) : std::nullopt,
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

void LuaScript::AddScriptComponent(GameObject* gameObject,
                                   const std::string& scriptName)
{
    if(!gameObject->HasComponent<ScriptComponent>())
        gameObject->AddComponent<ScriptComponent>();
    
    gameObject->GetComponent<ScriptComponent>()->CreateEnvironmentScript
        (lua,scriptName,SCRIPTS_PATH);
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
    
    WindowSettings::WindowHeight = window["height"].get_or(1280);
    WindowSettings::WindowWidth = window["width"].get_or(720);
    WindowSettings::WindowName = window["name"].get_or_create<std::string>("Game");
    WindowSettings::IconImageLocation = window["icon"].get_or_create<std::string>("./assets/images/icon.png");
}

void LuaScript::LoadLuaBindings()
{
    lua.open_libraries();
    lua["globals"] = lua.create_table();
    lua.new_usertype<WindowSettings>(
        "Window",
        "width",sol::property(&WindowSettings::GetWidth),
        "height", sol::property(&WindowSettings::GetHeight)
        );
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
    lua.new_usertype<SpriteComponent>(
        "sprite_component",
        "width", &SpriteComponent::Width,
        "height", &SpriteComponent::Height
    );
    lua.new_usertype<TextComponent>(
            "text_component",
            "text", &TextComponent::Text,
            "font_size", &TextComponent::FontSize,
            "color", &TextComponent::textColor
        );
    lua.new_usertype<ScriptComponent>(
            "script_component",
            "get_function", &ScriptComponent::GetScriptFunction
        );
    lua.new_usertype<GameObject>(
        "gameobject",
        "get_id", &GameObject::GetId,
        "name", &GameObject::Name,
        "has_tag", &GameObject::HasTag,
        "set_tag", &GameObject::Tag,
        
        "add_component_transform", &AddTransformComponent,
        "add_component_sprite", &AddSpriteComponent,
        "add_component_text", &AddTextComponent,
        "add_component_boxcollider", &AddBoxCollider2DComponent,
        "add_component_rigidbody", &AddRigidBody2DComponent,
        "add_component_script", [this](GameObject* gameObject, const std::string& scriptName) {
            AddScriptComponent(gameObject, scriptName);
        },
        
        "get_component_transform", &GameObject::GetComponent<TransformComponent>,
        "get_component_sprite", &GameObject::GetComponent<SpriteComponent>,
        "get_component_text", &GameObject::GetComponent<TextComponent>,
        "get_component_boxcollider", &GameObject::GetComponent<BoxCollider2DComponent>,
        "get_component_rigidbody", &GameObject::GetComponent<RigidBody2DComponent>,
        "get_component_script", &GameObject::GetComponent<ScriptComponent>
    );
    lua.set_function("create",CreateGameObject);
    lua.set_function("destroy", Destroy);
    lua.set_function("mito_log",LuaPrint);
    lua.set_function("open_level",[this](const std::string& name) {
            this->EmitOpenLevel(name);
    });
}

void LuaScript::EmitOpenLevel(const std::string& levelName)
{
    GlobalEventBus::EmitEvent<OpenLevelEvent>(levelName);
}

void LuaScript::LoadLevel(std::string name)
{
    sol::load_result luaScript = lua.load_file(LEVELS_PATH + name);
    if (!luaScript.valid())
    {
        sol::error err = luaScript;
        std::string errorMessage = err.what();
        Logger::Err("Error loading the lua script: "+name + errorMessage);
        return;
    }
    // Executes the script using the Sol state
    lua.script_file(LEVELS_PATH + name);

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
            sol::optional<sol::table> text = entity["components"]["text"];
            if (text != sol::nullopt)
            {
                newGameObject->AddComponent<TextComponent>(
                    entity["components"]["text"]["text"],
                    entity["components"]["text"]["font"],
                    entity["components"]["text"]["scale"],
                    entity["components"]["text"]["red"].get_or(255),
                    entity["components"]["text"]["green"].get_or(255),
                    entity["components"]["text"]["blue"].get_or(255)
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

                    newGameObject->GetComponent<ScriptComponent>()->CreateEnvironmentScript(lua, scriptName, SCRIPTS_PATH);
                }
            }
        }

        i++;
    }
}

