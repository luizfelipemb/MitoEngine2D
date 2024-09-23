#include "LuaScript.h"

#include "../Audio/AudioManager.h"
#include "../Events/OpenLevelEvent.h"
#include "../Logger/Logger.h"
#include "../GameObjects/GameObject.h"

void LuaPrint(const std::string& message)
{
    Logger::Lua(message);
}

void Destroy(GameObject* go)
{
    if(go != nullptr)
    {
        Registry::DestroyGameObject(go->GetId());
    }else
    {
        Logger::Err("Tried to destroy NULL object");
    }
}

GameObject* CreateGameObject(sol::optional<std::string> name)
{
    return Registry::CreateGameObject(name.value_or("")).get();
}
GameObject* FindGameObjectByTag(std::string tag)
{
    return Registry::GetGameObjectByTag(tag);
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
                        sol::optional<int> layer,
                        sol::optional<Color> color)
{
    gameObject->AddComponent<SpriteComponent>(
        sprite,
        width ? std::optional<int>(*width) : std::nullopt,
        height ? std::optional<int>(*height) : std::nullopt,
        layer ? std::optional<int>(*layer) : std::nullopt,
        color ? std::optional<Color>(*color) : std::nullopt);
}

void AddTextComponent(GameObject* gameObject,
                      std::string& text,
                      std::string& font,
                      sol::optional<int> layer,
                      sol::optional<int> scale,
                      sol::optional<std::uint8_t> red,
                      sol::optional<std::uint8_t> green,
                      sol::optional<std::uint8_t> blue)
{
    gameObject->AddComponent<TextComponent>(
        text,
        font,
        layer ? std::optional<int>(*layer) : std::nullopt,
        scale ? std::optional<int>(*scale) : std::nullopt,
        red ? std::optional<std::uint8_t>(*red) : std::nullopt,
        green ? std::optional<std::uint8_t>(*green) : std::nullopt,
        blue ? std::optional<std::uint8_t>(*blue) : std::nullopt);
}

void AddRigidBody2DComponent(GameObject* gameObject,
                             sol::optional<glm::vec2> velocity,
                             sol::optional<glm::vec2> acceleration)
{
    gameObject->AddComponent<RigidBody2DComponent>(
        velocity ? std::optional<glm::vec2>(*velocity) : std::nullopt,
        acceleration ? std::optional<glm::vec2>(*acceleration) : std::nullopt);
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
void AddClickableComponent(GameObject* gameObject,
                               sol::optional<int> width,
                               sol::optional<int> height,
                               sol::optional<glm::vec2> offset)
{
    gameObject->AddComponent<ClickableComponent>(
        width ? std::optional<int>(*width) : std::nullopt,
        height ? std::optional<int>(*height) : std::nullopt,
        offset ? std::optional<glm::vec2>(*offset) : std::nullopt);
}

void LuaScript::AddScriptComponent(GameObject* gameObject,
                                   const std::string& scriptName)
{
    if (!gameObject->HasComponent<ScriptComponent>())
        gameObject->AddComponent<ScriptComponent>();

    gameObject->GetComponent<ScriptComponent>()->CreateEnvironmentScript
        (lua, scriptName, SCRIPTS_PATH);
}

void LuaScript::SettingsSetup()
{
    Logger::Log("SettingsSetup");
    lua.open_libraries();
    const std::string& settingsFile = SETTINGS_PATH + "settings.lua";
    sol::load_result luaScript = lua.load_file(settingsFile);
    if (!luaScript.valid())
    {
        sol::error err = luaScript;
        std::string errorMessage = err.what();
        Logger::Err("Error loading the lua script: " + errorMessage);
        return;
    }
    lua.script_file(settingsFile);
    sol::table settings = lua["Settings"];
    sol::table window = settings["window"];

    WindowSettings::FPS = window["fps"].get_or(60);
    WindowSettings::WindowHeight = window["height"].get_or(1280);
    WindowSettings::WindowWidth = window["width"].get_or(720);
    WindowSettings::WindowName = window["name"].get_or_create<std::string>("Game");
    WindowSettings::IconImageLocation = window["icon"].get_or_create<std::string>("./assets/images/icon.png");

    sol::optional<sol::table> sound = settings["sound"];
    if (sound != sol::nullopt)
    {
        if(sound.value()["music"].valid())
        {
            AudioManager::SetMusicVolume(sound.value()["music"]);
        }
        if(sound.value()["sound"].valid())
        {
            AudioManager::SetSoundVolume(sound.value()["sound"]);
        }
    }
}

void LuaScript::LoadLuaBindings()
{
    lua.open_libraries();
    lua["globals"] = lua.create_table();
    lua.new_usertype<WindowSettings>(
        "Window",
        "width", sol::property(&WindowSettings::GetWidth),
        "height", sol::property(&WindowSettings::GetHeight)
    );
    LoadLuaKeys();
    lua.new_usertype<Color>(
        "color",
        sol::constructors<Color(), Color(std::uint8_t, std::uint8_t, std::uint8_t)>(),
        "r", &Color::Red,
        "g", &Color::Green,
        "b", &Color::Blue
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
        "velocity", &RigidBody2DComponent::Velocity,
        "acceleration", &RigidBody2DComponent::Acceleration
    );
    lua.new_usertype<SpriteComponent>(
        "sprite_component",
        "width", &SpriteComponent::Width,
        "height", &SpriteComponent::Height,
        "color", &SpriteComponent::color
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
        "add_component_clickable", &AddClickableComponent,
        "add_component_rigidbody", &AddRigidBody2DComponent,
        "add_component_script", [this](GameObject* gameObject, const std::string& scriptName)
        {
            AddScriptComponent(gameObject, scriptName);
        },

        "get_component_transform", &GameObject::GetComponent<TransformComponent>,
        "get_component_sprite", &GameObject::GetComponent<SpriteComponent>,
        "get_component_text", &GameObject::GetComponent<TextComponent>,
        "get_component_boxcollider", &GameObject::GetComponent<BoxCollider2DComponent>,
        "get_component_clickable", &GameObject::GetComponent<ClickableComponent>,
        "get_component_rigidbody", &GameObject::GetComponent<RigidBody2DComponent>,
        "get_component_script", &GameObject::GetComponent<ScriptComponent>
    );
    //audios
    lua.set_function("load_sound", AudioManager::LoadSound);
    lua.set_function("play_sound", AudioManager::PlaySound);
    lua.set_function("stop_sound", AudioManager::StopSound);
    lua.set_function("load_music", AudioManager::LoadMusic);
    lua.set_function("play_music", AudioManager::PlayMusic);
    lua.set_function("stop_music", AudioManager::StopMusic);
    lua.set_function("set_music_volume", AudioManager::SetMusicVolume);
    lua.set_function("set_sound_volume", AudioManager::SetSoundVolume);
    lua.set_function("get_music_volume", AudioManager::GetMusicVolume);
    lua.set_function("get_sound_volume", AudioManager::GetGlobalSoundVolume);
    
    lua.set_function("create", CreateGameObject);
    lua.set_function("destroy", Destroy);
    lua.set_function("find_by_tag",FindGameObjectByTag);
    lua.set_function("mito_log", LuaPrint);
    lua.set_function("open_level", [this](const std::string& name)
    {
        this->EmitOpenLevel(name);
    });
    lua.set_function("spawn_prefab", [this](const std::string& name)
    {
        return this->SpawnPrefab(name);
    });
}

void LuaScript::LoadLuaKeys()
{
    lua.new_enum("mousecode",
        "right", SDL_BUTTON_RIGHT,
        "left", SDL_BUTTON_LEFT,
        "middle", SDL_BUTTON_MIDDLE
    );
    
    lua.new_enum("keycode",
        // Letters
        "a", SDLK_a,
        "b", SDLK_b,
        "c", SDLK_c,
        "d", SDLK_d,
        "e", SDLK_e,
        "f", SDLK_f,
        "g", SDLK_g,
        "h", SDLK_h,
        "i", SDLK_i,
        "j", SDLK_j,
        "k", SDLK_k,
        "l", SDLK_l,
        "m", SDLK_m,
        "n", SDLK_n,
        "o", SDLK_o,
        "p", SDLK_p,
        "q", SDLK_q,
        "r", SDLK_r,
        "s", SDLK_s,
        "t", SDLK_t,
        "u", SDLK_u,
        "v", SDLK_v,
        "w", SDLK_w,
        "x", SDLK_x,
        "y", SDLK_y,
        "z", SDLK_z,

        // Numbers
        "0", SDLK_0,
        "1", SDLK_1,
        "2", SDLK_2,
        "3", SDLK_3,
        "4", SDLK_4,
        "5", SDLK_5,
        "6", SDLK_6,
        "7", SDLK_7,
        "8", SDLK_8,
        "9", SDLK_9,

        // Function keys
        "f1", SDLK_F1,
        "f2", SDLK_F2,
        "f3", SDLK_F3,
        "f4", SDLK_F4,
        "f5", SDLK_F5,
        "f6", SDLK_F6,
        "f7", SDLK_F7,
        "f8", SDLK_F8,
        "f9", SDLK_F9,
        "f10", SDLK_F10,
        "f11", SDLK_F11,
        "f12", SDLK_F12,

        // Special keys
        "escape", SDLK_ESCAPE,
        "return", SDLK_RETURN,
        "space", SDLK_SPACE,
        "backspace", SDLK_BACKSPACE,
        "tab", SDLK_TAB,
        "capslock", SDLK_CAPSLOCK,
        "left_shift", SDLK_LSHIFT,
        "right_shift", SDLK_RSHIFT,
        "left_ctrl", SDLK_LCTRL,
        "right_ctrl", SDLK_RCTRL,
        "left_alt", SDLK_LALT,
        "right_alt", SDLK_RALT,
        "left_gui", SDLK_LGUI,
        "right_gui", SDLK_RGUI,

        // Arrow keys
        "up", SDLK_UP,
        "down", SDLK_DOWN,
        "left", SDLK_LEFT,
        "right", SDLK_RIGHT,

        // Keypad keys
        "kp_0", SDLK_KP_0,
        "kp_1", SDLK_KP_1,
        "kp_2", SDLK_KP_2,
        "kp_3", SDLK_KP_3,
        "kp_4", SDLK_KP_4,
        "kp_5", SDLK_KP_5,
        "kp_6", SDLK_KP_6,
        "kp_7", SDLK_KP_7,
        "kp_8", SDLK_KP_8,
        "kp_9", SDLK_KP_9,
        "kp_enter", SDLK_KP_ENTER,
        "kp_plus", SDLK_KP_PLUS,
        "kp_minus", SDLK_KP_MINUS,
        "kp_multiply", SDLK_KP_MULTIPLY,
        "kp_divide", SDLK_KP_DIVIDE
    );
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
        Logger::Err("Error loading the lua script: " + name + errorMessage);
        return;
    }
    // Executes the script using the Sol state
    lua.script_file(LEVELS_PATH + name);

    // Read the big table for the current level
    sol::optional<sol::table> level = lua["Level"];
    if(level == sol::nullopt)
    {
        return;
    }
    sol::table entities = level.value()["entities"];
    int i = 1; // Lua table start at 1
    while (true)
    {
        sol::optional<sol::table> hasEntity = entities[i];
        if (hasEntity == sol::nullopt)
        {
            break;
        }

        sol::table entity = entities[i];
        SpawnGameObject(entity);
        i++;
    }
}

GameObject* LuaScript::SpawnGameObject(sol::table entity)
{
    std::string name;
    if (entity["name"] != sol::nil)
    {
        name.assign(entity["name"]);
    }

    // Create the game object using the global function
    GameObject* newGameObject = CreateGameObject(name);

    // Tags
    sol::optional<sol::table> tags = entity["tags"];
    if (tags != sol::nullopt)
    {
        for (auto& tag : tags.value())
        {
            newGameObject->Tag(tag.second.as<std::string>());
        }
    }

    // Components
    sol::optional<sol::table> hasComponents = entity["components"];
    if (hasComponents != sol::nullopt)
    {
        // Transform
        sol::optional<sol::table> transform = entity["components"]["transform"];
        if (transform != sol::nullopt)
        {
            AddTransformComponent(
                newGameObject,
                glm::vec2(
                    entity["components"]["transform"]["position"]["x"],
                    entity["components"]["transform"]["position"]["y"]
                ),
                entity["components"]["transform"]["scale"],
                entity["components"]["transform"]["rotation"]
            );
        }

        // RigidBody
        sol::optional<sol::table> rigidbody = entity["components"]["rigidbody"];
        if (rigidbody != sol::nullopt)
        {
            AddRigidBody2DComponent(
                newGameObject,
                glm::vec2(
                    entity["components"]["rigidbody"]["velocity"]["x"].get_or(0.0),
                    entity["components"]["rigidbody"]["velocity"]["y"].get_or(0.0)
                ),
                glm::vec2(
                    entity["components"]["rigidbody"]["acceleration"]["x"].get_or(0.0),
                    entity["components"]["rigidbody"]["acceleration"]["y"].get_or(0.0)
                )
            );
        }
        
        // Sprite
        sol::optional<sol::table> sprite = entity["components"]["sprite"];
        if (sprite != sol::nullopt)
        {
            AddSpriteComponent(
                newGameObject,
                entity["components"]["sprite"]["sprite_name"],
                entity["components"]["sprite"]["width"],
                entity["components"]["sprite"]["height"],
                entity["components"]["sprite"]["layer"],
                entity["components"]["sprite"]["color"]
            );
        }

        // BoxCollider
        sol::optional<sol::table> collider = entity["components"]["boxcollider"];
        if (collider != sol::nullopt)
        {
            AddBoxCollider2DComponent(
                newGameObject,
                entity["components"]["boxcollider"]["width"],
                entity["components"]["boxcollider"]["height"],
                glm::vec2(
                    entity["components"]["boxcollider"]["offset"]["x"].get_or(0),
                    entity["components"]["boxcollider"]["offset"]["y"].get_or(0)
                )
            );
        }
        
        // ClickableComponent
        sol::optional<sol::table> clickable = entity["components"]["clickable"];
        if (clickable != sol::nullopt)
        {
            AddClickableComponent(
                newGameObject,
                entity["components"]["clickable"]["width"],
                entity["components"]["clickable"]["height"],
                glm::vec2(
                    entity["components"]["clickable"]["offset"]["x"].get_or(0),
                    entity["components"]["clickable"]["offset"]["y"].get_or(0)
                )
            );
        }


        // Text
        sol::optional<sol::table> text = entity["components"]["text"];
        if (text != sol::nullopt)
        {
            std::string textStr = entity["components"]["text"]["text"];
            std::string fontStr = entity["components"]["text"]["font"];

            AddTextComponent(
                newGameObject,
                textStr,
                fontStr,
                entity["components"]["text"]["layer"],
                entity["components"]["text"]["scale"],
                entity["components"]["text"]["red"].get_or(255),
                entity["components"]["text"]["green"].get_or(255),
                entity["components"]["text"]["blue"].get_or(255)
            );
        }

        // Scripts
        sol::optional<sol::table> scriptsTable = entity["components"]["scripts"];
        if (scriptsTable != sol::nullopt)
        {
            for (auto& scriptEntry : scriptsTable.value())
            {
                std::string scriptName = scriptEntry.second.as<std::string>();

                AddScriptComponent(newGameObject, scriptName);
            }
        }
    }
    GlobalEventBus::EmitEvent<GameObjectCreatedEvent>();
    return newGameObject;
}

GameObject* LuaScript::SpawnPrefab(std::string name)
{
    sol::load_result luaScript = lua.load_file("./assets/prefabs/" + name);
    if (!luaScript.valid())
    {
        sol::error err = luaScript;
        std::string errorMessage = err.what();
        Logger::Err("Error loading the lua script: " + name + errorMessage);
        return nullptr;
    }
    // Executes the script using the Sol state
    lua.script_file("./assets/prefabs/" + name);
    sol::table prefab = lua["Prefab"];
    return SpawnGameObject(prefab);
}
