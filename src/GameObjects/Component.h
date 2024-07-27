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
                                glm::vec2 position = glm::vec2(0, 0),
                                double scale = 1,
                                double rotation = 0.0)
        : Component(owner), Position(position), Scale(scale), Rotation(rotation)
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
    SpriteComponent(GameObject* owner, std::string sprite,int width = 100, int height = 100,
                    std::uint8_t red = 255, std::uint8_t green = 255, std::uint8_t blue = 255) :
        Component(owner), m_sprite(sprite), m_width(width), m_height(height), m_color(Color{red, green, blue})
    {
    }

    void Update(float deltaTime) override;

private:
    std::string m_sprite;
    int m_width;
    int m_height;
    Color m_color;
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
    bool m_colliding=false;
    glm::vec2 m_currentVelocity;
    glm::vec2 m_previousVelocity;
};

class RigidBody2DComponent : public Component
{
public:
    explicit RigidBody2DComponent(GameObject* owner, glm::vec2 velocity = glm::vec2(0.0, 0.0)) :
        Component(owner), Velocity(velocity)
    {
    }

    void Update(float deltaTime) override;
    glm::vec2 Velocity;

private:
};

class BoxCollider2DComponent : public Component
{
public:
    explicit BoxCollider2DComponent(GameObject* owner, int width = 0, int height = 0, glm::vec2 offset = glm::vec2(0));

    void Update(float deltaTime) override;
    int Width;
    int Height;
    glm::vec2 Offset;

private:
};


class ScriptComponent : public Component
{
public:
    ScriptComponent(GameObject* owner)
        : Component(owner)
    {}
    
    void AddScript(sol::environment& luaEnv);
    void Update(float deltaTime) override;
    void CallUpdate(float deltaTime);
    void CallStart();
    std::vector<sol::function> StartFunc;
    std::vector<sol::function> UpdateFunc;
};