#pragma once
#include "GameObject.h"
#include "../AssetStore/AssetManager.h"
#include "../AssetStore/RendererManager.h"

#include "../Events/KeyPressedEvent.h"
#include "../Events/CollisionStayEvent.h"
#include "../Events/GlobalEventBus.h"

class GameObject;

class Component
{
public:
    explicit Component(GameObject* owner) : m_owner(owner)
    {
    }

    virtual ~Component() = default;
    virtual void Update(float deltaTime) = 0;

protected:
    GameObject* m_owner;
};

class TransformComponent : public Component
{
public:
    explicit TransformComponent(GameObject* owner,
                                std::optional<glm::vec2> position = std::nullopt,
                                std::optional<double> scale = std::nullopt,
                                std::optional<double> rotation = std::nullopt)
        : Component(owner), 
          Position(position.value_or(glm::vec2{})), 
          Scale(scale.value_or(1.0)), 
          Rotation(rotation.value_or(0.0))
    {
    }

    void Update(float deltaTime) override;

    glm::vec2 Position;
    double Scale;
    double Rotation;
};

class SpriteComponent : public Component
{
public:
    explicit SpriteComponent(GameObject* owner, 
                             std::string sprite,
                             std::optional<int> width = std::nullopt, 
                             std::optional<int> height = std::nullopt,
                             std::optional<std::uint8_t> red = std::nullopt, 
                             std::optional<std::uint8_t> green = std::nullopt, 
                             std::optional<std::uint8_t> blue = std::nullopt);

    void Update(float deltaTime) override;
    int Width;
    int Height;
private:
    std::string m_sprite;
    Color m_color;
};
class TextComponent : public Component
{
public:
    explicit TextComponent(GameObject* owner,
        std::string text,
        std::string font,
        std::optional<int> fontSize = std::nullopt,
        std::optional<std::uint8_t> red = std::nullopt,
        std::optional<std::uint8_t> green = std::nullopt,
        std::optional<std::uint8_t> blue = std::nullopt);

    void Update(float deltaTime) override;
    std::string Text;
    int FontSize;
    Color textColor;
private:
    std::string m_font;
};

class ControllerComponent : public Component
{
public:
    explicit ControllerComponent(GameObject* owner);
    void Update(float deltaTime) override;
    void OnCollisionStay(CollisionStayEvent& event);
    void OnCollisionEnter(CollisionEnterEvent& event);
    void OnCollisionExit(CollisionExitEvent& event);
    void OnKeyPressedEvent(KeyPressedEvent& event);
    void OnKeyReleasedEvent(KeyReleasedEvent& event);

private:
    bool m_colliding = false;
    glm::vec2 m_currentVelocity;
    glm::vec2 m_previousVelocity;
};

class RigidBody2DComponent : public Component
{
public:
    explicit RigidBody2DComponent(GameObject* owner, 
                                  std::optional<glm::vec2> velocity = std::nullopt) 
        : Component(owner), 
          Velocity(velocity.value_or(glm::vec2(0.0, 0.0)))
    {
    }

    void Update(float deltaTime) override;
    glm::vec2 Velocity;
};

class BoxCollider2DComponent : public Component
{
public:
    explicit BoxCollider2DComponent(GameObject* owner, 
                                    std::optional<int> width = std::nullopt, 
                                    std::optional<int> height = std::nullopt, 
                                    std::optional<glm::vec2> offset = std::nullopt);

    void Update(float deltaTime) override;
    int Width;
    int Height;
    glm::vec2 Offset;
};

class ScriptComponent : public Component
{
public:
    ScriptComponent(GameObject* owner)
        : Component(owner)
    {}
    ~ScriptComponent()
    {
        StartFunc.clear();
        UpdateFunc.clear();
        scriptFunctions.clear();
    }
    void OnCollisionStay(CollisionStayEvent& event);
    void OnCollisionEnter(CollisionEnterEvent& event);
    void OnCollisionExit(CollisionExitEvent& event);
    void OnKeyPressedEvent(KeyPressedEvent& event);
    void OnKeyReleasedEvent(KeyReleasedEvent& event);
    void AddScript(sol::environment& luaEnv);
    void CreateEnvironmentScript(sol::state& lua, const std::string name, const std::string scriptsFolder);
    sol::function GetScriptFunction(const std::string& name);
    void Update(float deltaTime) override;
    void CallUpdate(float deltaTime);
    void CallStart();

public:
    std::vector<sol::function> StartFunc;
    std::vector<sol::function> UpdateFunc;
    std::map<std::string, sol::function> scriptFunctions;
};

template<typename... Args>
    void CallLuaFunction(sol::function& func, Args&&... args) {
    if (func == sol::lua_nil)
        return;

    sol::protected_function_result safe_result = func(std::forward<Args>(args)...);
    if (!safe_result.valid()) {
        sol::error err = safe_result;
        std::cout << "Error calling Lua function: " << err.what() << '\n';
    }
}