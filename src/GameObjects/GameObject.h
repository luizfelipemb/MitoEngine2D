#pragma once
#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <vec2.hpp>
#include <vec3.hpp>
#include <vector>
#include "../Logger/Logger.h"
#include "../Events/EventBus.h"
#include "../Events/KeyPressedEvent.h"
#include "../AssetStore/AssetManager.h"
#include "../Events/CollisionStayEvent.h"
#include "../Events/GlobalEventBus.h"

class KeyReleasedEvent;
class Registry;
class KeyPressedEvent;
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
    SpriteComponent(GameObject* owner, std::string sprite, int width = 100, int height = 100, std::uint8_t red = 255,
                    std::uint8_t green = 255, std::uint8_t blue = 255) :
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

class GameObject
{
public:
    GameObject(int id, Registry* reg): RefRegistry(reg), m_id(id)
    {
    }
    int GetId() const;
    
    template <typename TComponent, typename... TArgs>
    void AddComponent(TArgs&&... args);
    template <typename TComponent>
    TComponent* GetComponent();
    template <typename TComponent>
    bool HasComponent();

    std::unordered_map<std::type_index, std::shared_ptr<Component>> Components;

    Registry* RefRegistry;
    EventBus LocalEventBus;

    // Operator overloading for entity objects
    GameObject& operator =(const GameObject& other) = default;
    bool operator ==(const GameObject& other) const { return m_id == other.m_id; }
    bool operator !=(const GameObject& other) const { return m_id != other.m_id; }
    bool operator >(const GameObject& other) const { return m_id > other.m_id; }
    bool operator <(const GameObject& other) const { return m_id < other.m_id; }

    private:
        int m_id;
};


class Registry
{
public:
    void Update(float deltaTime);
    const std::vector<std::unique_ptr<GameObject>>& GetAllGameObjects() const;
    bool CheckAABBCollision(double aX, double aY, double aW, double aH, double bX, double bY, double bW, double bH);
    void CalculateCollisions();
    std::unique_ptr<GameObject>& CreateGameObject(glm::vec3 position = glm::vec3(0, 0, 0));

private:
    int m_numberOfGameObjects;
    std::vector<std::unique_ptr<GameObject>> m_gameObjects;
    std::unordered_multimap<int,int> m_objectsColliding;
};

//   GAMEOBJECT  /////////////////////////////////////////////////////////////////////////
template <typename TComponent, typename... TArgs>
void GameObject::AddComponent(TArgs&&... args)
{
    // Create the component with the owner and forward the remaining arguments
    std::shared_ptr<TComponent> newComponent = std::make_shared<TComponent>(this, std::forward<TArgs>(args)...);

    // Insert the component into the unordered_map and the vector
    Components.insert(std::make_pair(std::type_index(typeid(TComponent)), newComponent));

    Logger::Log(
        "GameObject with id:" + std::to_string(m_id) + "; added component: " + std::string(typeid(TComponent).name()));
}

template <typename TComponent>
TComponent* GameObject::GetComponent()
{
    auto it = Components.find(std::type_index(typeid(TComponent)));
    if (it != Components.end())
    {
        std::shared_ptr<Component> basePtr = it->second;
        std::shared_ptr<TComponent> derivedPtr = std::dynamic_pointer_cast<TComponent>(basePtr);
        if (derivedPtr)
        {
            return derivedPtr.get();
        }
        else
        {
            Logger::Err("Component cast failed");
            return nullptr;
        }
    }
    else
    {
        Logger::Err("Component not found");
        return nullptr;
    }
}

template <typename TComponent>
bool GameObject::HasComponent()
{
    auto it = Components.find(std::type_index(typeid(TComponent)));
    return it != Components.end();
}
