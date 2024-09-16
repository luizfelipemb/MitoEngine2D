#include "Component.h"
 
 #include "../Audio/AudioManager.h"
 #include "../Game/Game.h"
 
 
 class MouseInteractedEvent;
 
 void TransformComponent::Update(float deltaTime)
 {
    
 }
 
 TextComponent::TextComponent(GameObject* owner,
                             std::string text,
                             std::string font,
                             std::optional<int> layer,
                             std::optional<int> fontSize,
                             std::optional<std::uint8_t> red,
                             std::optional<std::uint8_t> green,
                             std::optional<std::uint8_t> blue)
     : RenderableComponent(owner,layer),
     Text(text),
     m_font(font),
     FontSize(fontSize.value_or(0)),
     textColor(Color{ red.value_or(255), green.value_or(255), blue.value_or(255) })
 {
 }
 
 void TextComponent::Update(float deltaTime)
 {
     
 }
 
 void TextComponent::Render()
 {
     if (auto transform = m_owner->GetComponent<TransformComponent>())
     {        
         AssetManager::RenderText(
             RendererManager::Renderer,
             Text,
             m_font,
             FontSize,
             transform->Position.x,
             transform->Position.y,
             1,
             false,
             textColor);
     }
 }
 
 
 SpriteComponent::SpriteComponent(GameObject* owner, std::string sprite,
                                  std::optional<int> width, std::optional<int> height, std::optional<int> layer,
                                  std::optional<std::uint8_t> red, std::optional<std::uint8_t> green, std::optional<std::uint8_t> blue)
     :   RenderableComponent(owner,layer),
         Width(width.value_or(0)),
         Height(height.value_or(0)),
           m_sprite(sprite),
           m_color(Color{red.value_or(255), green.value_or(255), blue.value_or(255)})
 {
     if(Width==0)
     {
         Width = AssetManager::GetWidthOfSprite(RendererManager::Renderer,sprite);
     }
     if(Height==0)
     {
         Height = AssetManager::GetHeightOfSprite(RendererManager::Renderer,sprite);
     }
 }
 
 void SpriteComponent::Update(float deltaTime)
 {
 }
 
 void SpriteComponent::Render()
 {
     if (auto transform = m_owner->GetComponent<TransformComponent>())
     {
         if (m_sprite == "")
         {
             AssetManager::DrawRectangle(
                 RendererManager::Renderer,
                 transform->Position.x,
                 transform->Position.y,
                 Width,
                 Height,
                 m_color);
         }
         else
         {
             AssetManager::RenderImage(
                                 RendererManager::Renderer,
                                 m_sprite,
                                 transform->Position.x,
                                 transform->Position.y,
                                 Width,
                                 Height,
                                 transform->Scale,
                                 m_color);
         }
     }
 }
 
 
 ControllerComponent::ControllerComponent(GameObject* owner) : Component(owner)
 {
     m_currentVelocity = glm::vec2(0, 0);
     m_previousVelocity = m_currentVelocity;
     GlobalEventBus::SubscribeToEvent<KeyPressedEvent>(this, &ControllerComponent::OnKeyPressedEvent);
     GlobalEventBus::SubscribeToEvent<KeyReleasedEvent>(this, &ControllerComponent::OnKeyReleasedEvent);
     owner->LocalEventBus.SubscribeToEvent<CollisionStayEvent>(this, &ControllerComponent::OnCollisionStay);
     owner->LocalEventBus.SubscribeToEvent<CollisionEnterEvent>(this, &ControllerComponent::OnCollisionEnter);
     owner->LocalEventBus.SubscribeToEvent<CollisionExitEvent>(this, &ControllerComponent::OnCollisionExit);
 }
 
 void ControllerComponent::Update(float deltaTime)
 {
     if (auto rigidBody = m_owner->GetComponent<RigidBody2DComponent>())
     {
         rigidBody->Velocity += m_currentVelocity - m_previousVelocity;
         //Logger::Log(std::to_string(static_cast<int>(m_currentVelocity.x))+" "+
         //					std::to_string(static_cast<int>(m_currentVelocity.y)));
     }
     m_previousVelocity = m_currentVelocity;
 }
 
 void ControllerComponent::OnCollisionStay(CollisionStayEvent& event)
 {
     m_colliding = true;
 }
 
 void ControllerComponent::OnCollisionEnter(CollisionEnterEvent& event)
 {
     m_colliding = true;
     m_currentVelocity = glm::vec2(0, 0);
 }
 
 void ControllerComponent::OnCollisionExit(CollisionExitEvent& event)
 {
     m_colliding = false;
 }
 
 void ControllerComponent::OnKeyPressedEvent(KeyPressedEvent& event)
 {
     if (auto rigidBody = m_owner->GetComponent<RigidBody2DComponent>())
     {
         switch (event.Symbol)
         {
         case SDLK_w:
             m_currentVelocity.y -= 500;
             break;
         case SDLK_d:
             m_currentVelocity.x += 500;
             break;
         case SDLK_s:
             m_currentVelocity.y += 500;
             break;
         case SDLK_a:
             m_currentVelocity.x -= 500;
             break;
         }
     }
 }
 
 void ControllerComponent::OnKeyReleasedEvent(KeyReleasedEvent& event)
 {
     if (auto rigidBody = m_owner->GetComponent<RigidBody2DComponent>())
     {
         switch (event.Symbol)
         {
         case SDLK_w:
             m_currentVelocity.y = 0;
             break;
         case SDLK_d:
             m_currentVelocity.x = 0;
             break;
         case SDLK_s:
             m_currentVelocity.y = 0;
             break;
         case SDLK_a:
             m_currentVelocity.x = 0;
             break;
         }
     }
 }
 
 
 
 void RigidBody2DComponent::Update(float deltaTime)
 {
     if (auto transform = m_owner->GetComponent<TransformComponent>())
     {
         transform->Position.x += Velocity.x * deltaTime;
         transform->Position.y += Velocity.y * deltaTime;
     }
 }
 
 ClickableComponent::ClickableComponent(GameObject* owner, std::optional<int> width, std::optional<int> height,
                                                std::optional<glm::vec2> offset):
     Component(owner), Offset(offset.value_or(glm::vec2(0)))
 {
     Width = width.value_or(0);
     Height= height.value_or(0);
     if(Width == 0 && owner->HasComponent<SpriteComponent>())
     {
         Width = owner->GetComponent<SpriteComponent>()->Width;
     }
     if(Height == 0 && owner->HasComponent<SpriteComponent>())
     {
         Height = owner->GetComponent<SpriteComponent>()->Height;
     }
     GlobalEventBus::SubscribeToEvent<MouseButtonPressedEvent>
     (this,&ClickableComponent::OnMousePressedEvent);
 }
 
 ClickableComponent::~ClickableComponent()
 {
     GlobalEventBus::UnsubscribeFromOwner(this);
 }
 
 void ClickableComponent::OnMousePressedEvent(MouseButtonPressedEvent& event)
 {
     if(auto transform = m_owner->GetComponent<TransformComponent>())
     {
         if(event.Position.x >= transform->Position.x    && event.Position.x <= transform->Position.x + Width
            && event.Position.y >= transform->Position.y && event.Position.y <= transform->Position.y + Height)
         {
             m_owner->LocalEventBus.EmitEvent<MouseInteractedEvent>();
         }
     }
 }
 
 BoxCollider2DComponent::BoxCollider2DComponent(GameObject* owner, std::optional<int> width, std::optional<int> height,
                                                std::optional<glm::vec2> offset):
     Component(owner), Offset(offset.value_or(glm::vec2(0)))
 {
     Width = width.value_or(0);
     Height= height.value_or(0);
     if(Width == 0 && owner->HasComponent<SpriteComponent>())
     {
         Width = owner->GetComponent<SpriteComponent>()->Width;
     }
     if(Height == 0 && owner->HasComponent<SpriteComponent>())
     {
         Height = owner->GetComponent<SpriteComponent>()->Height;
     }
 }
 void BoxCollider2DComponent::Update(float deltaTime)
 {
     
 }
 
AudioComponent::AudioComponent(
    GameObject* owner,
    const sol::optional<std::string> id,
    const sol::optional<std::string> path,
    const sol::optional<bool> autoPlay,
    const sol::optional<bool> isMusic) 
    : Component(owner)
 {
     bool success = init();
     if (autoPlay.value_or(false) && success && id.has_value() && path.has_value())
     {
         std::string Id = id.value();
         std::string Path = path.value();

         if (isMusic.value_or(false))
         {
             if (loadMusic(Id, Path))
             {
                 playMusic(Id);
             }
         }
         else
         {
             if (loadSound(Id, Path))
             {
                 playSound(Id);
             }
         }
     }
 }

bool AudioComponent::init()
{
    return AudioManager::GetInstance().init();
}


bool AudioComponent::loadMusic(const std::string& id, const std::string& path)
{
    return AudioManager::GetInstance().loadMusic(id, path);
}

bool AudioComponent::loadSound(const std::string& id, const std::string& path)
{
    return AudioManager::GetInstance().loadSound(id, path);
}

void AudioComponent::playMusic(const std::string& id, int loops)
{
    AudioManager::GetInstance().playMusic(id, loops);
}

void AudioComponent::playSound(const std::string& id, int loops)
{
    AudioManager::GetInstance().playSound(id, loops);    
}

void AudioComponent::stopMusic()
{
    AudioManager::GetInstance().stopMusic();
}

void AudioComponent::stopSound(int channel = -1)
{
    AudioManager::GetInstance().stopSound(channel);
}

void AudioComponent::cleanUp()
{
    AudioManager::GetInstance().cleanUp();
}

void ScriptComponent::OnCollisionStay(CollisionStayEvent& event)
{
    CallLuaFunction(scriptFunctions["on_collision_stay"], m_owner, event.Other.get());
}

void ScriptComponent::OnCollisionEnter(CollisionEnterEvent& event)
{
    CallLuaFunction(scriptFunctions["on_collision_enter"], m_owner, event.Other.get(),event.Direction);
}

void ScriptComponent::OnCollisionExit(CollisionExitEvent& event)
{
    CallLuaFunction(scriptFunctions["on_collision_exit"], m_owner, event.Other.get());
}

void ScriptComponent::OnKeyPressedEvent(KeyPressedEvent& event)
{
    CallLuaFunction(scriptFunctions["on_key_pressed"], m_owner, event.Symbol);
}

void ScriptComponent::OnKeyReleasedEvent(KeyReleasedEvent& event)
{
    CallLuaFunction(scriptFunctions["on_key_released"], m_owner, event.Symbol);
}

void ScriptComponent::OnMousePressedEvent(MouseButtonPressedEvent& event)
{
    CallLuaFunction(scriptFunctions["on_mouse_pressed"], m_owner, event.Button);
}

void ScriptComponent::OnMouseInteractedEvent(MouseInteractedEvent& event)
{
    CallLuaFunction(scriptFunctions["on_mouse_interacted"], m_owner);
}

void ScriptComponent::AddScript(sol::environment& luaEnv)
{
    if (luaEnv["on_enable"] != sol::lua_nil)
    {
        luaEnv["on_enable"](m_owner);
    }
    if (luaEnv["start"] != sol::lua_nil)
    {
        StartFunc.push_back(luaEnv["start"]);
    }
    if (luaEnv["update"] != sol::lua_nil)
    {
        UpdateFunc.push_back(luaEnv["update"]);
    }
    if (luaEnv["on_key_pressed"] != sol::lua_nil)
    {
        GlobalEventBus::SubscribeToEvent<KeyPressedEvent>
            (this, &ScriptComponent::OnKeyPressedEvent);
    }
    if (luaEnv["on_key_released"] != sol::lua_nil)
    {
        GlobalEventBus::SubscribeToEvent<KeyReleasedEvent>
            (this, &ScriptComponent::OnKeyReleasedEvent);
    }
    if (luaEnv["on_mouse_pressed"] != sol::lua_nil)
    {
        GlobalEventBus::SubscribeToEvent<MouseButtonPressedEvent>
            (this, &ScriptComponent::OnMousePressedEvent);
    }
    if (luaEnv["on_mouse_interacted"] != sol::lua_nil)
    {
        m_owner->LocalEventBus.SubscribeToEvent<MouseInteractedEvent>
            (this, &ScriptComponent::OnMouseInteractedEvent);
    }
    if (luaEnv["on_collision_enter"] != sol::lua_nil)
    {
        m_owner->LocalEventBus.SubscribeToEvent<CollisionEnterEvent>
        (this, &ScriptComponent::OnCollisionEnter);
    }
    if (luaEnv["on_collision_stay"] != sol::lua_nil)
    {
        m_owner->LocalEventBus.SubscribeToEvent<CollisionStayEvent>
        (this, &ScriptComponent::OnCollisionStay);
    }
    if (luaEnv["on_collision_exit"] != sol::lua_nil)
    {
        m_owner->LocalEventBus.SubscribeToEvent<CollisionExitEvent>
        (this, &ScriptComponent::OnCollisionExit);
    }

    // Store functions in a map for retrieval
    for (auto& pair : luaEnv)
    {
        std::string funcName = pair.first.as<std::string>();
        if (luaEnv[funcName].is<sol::function>())
        {
            scriptFunctions[funcName] = luaEnv[funcName];
        }
    }
}

void ScriptComponent::CreateEnvironmentScript(sol::state& lua, const std::string name, const std::string scriptsFolder)
{
    // Create a new environment table
    sol::environment scriptEnv(lua, sol::create, lua.globals());
                    
    sol::load_result script = lua.load_file(scriptsFolder + name);
    Logger::Log(name);
    if (!script.valid())
    {
        sol::error err = script;
        std::string errorMessage = err.what();
        Logger::Err("Error loading the script: " + errorMessage);
    } else
    {
        lua.script_file(scriptsFolder + name, scriptEnv);
        // Share the global table
        scriptEnv["globals"] = lua["globals"];
        AddScript(scriptEnv);
    }
}

sol::function ScriptComponent::GetScriptFunction(const std::string& name)
{
    if (scriptFunctions.find(name) != scriptFunctions.end())
    {
        return scriptFunctions[name];
    }
    return sol::nil;
}


void ScriptComponent::CallUpdate(float deltaTime)
{
    for (auto& func : UpdateFunc)
    {
        CallLuaFunction(func, m_owner,deltaTime);
    }
}

void ScriptComponent::CallStart()
{
    for (auto& func : StartFunc)
    {
        CallLuaFunction(func, m_owner);
    }
}