#pragma once
#include <sol.hpp>
#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <vec2.hpp>
#include <vec3.hpp>
#include <vector>
#include <tuple>

#include "Component.h"
#include "../Logger/Logger.h"
#include "../Events/EventBus.h"

class Registry;
class Component;
class TransformComponent;

class GameObject
{
public:
    GameObject(int id, std::string name = "GameObject"):
             m_id(id), Name(name) {}
    int GetId() const;
    std::string GetName() const;
    template <typename TComponent, typename... TArgs> void AddComponent(TArgs&&... args);
    template <typename TComponent> TComponent* GetComponent();
    template <typename TComponent> bool HasComponent();
    GameObject& operator =(const GameObject& other) = default;
    bool operator ==(const GameObject& other) const { return m_id == other.m_id; }
    bool operator !=(const GameObject& other) const { return m_id != other.m_id; }
    bool operator >(const GameObject& other) const { return m_id > other.m_id; }
    bool operator <(const GameObject& other) const { return m_id < other.m_id; }
    std::unordered_map<std::type_index, std::shared_ptr<Component>> Components;
    EventBus LocalEventBus;
    std::string Name;

    private:
        int m_id;
};


class Registry
{
public:
    void Start();
    void Update(float deltaTime);
    const std::vector<std::unique_ptr<GameObject>>& GetAllGameObjects() const;
    static std::unique_ptr<GameObject>* GetGameObjectFromId(int id);
    bool CheckAABBCollision(double aX, double aY, double aW, double aH, double bX,
                                                double bY, double bW, double bH);
    glm::vec2 Registry::GetCollisionDirection(double aX, double aY, double aW, double aH, 
                                              double bX, double bY, double bW, double bH);
    void CalculateCollisions();
    static std::unique_ptr<GameObject>& CreateGameObject(std::string name = "GameObject");
    static void DestroyGameObject(int id);
    void ClearGameObjects();
private:
    static std::vector<int> m_idsToDestroy;
    static int m_nextFreeId;
    static std::vector<std::unique_ptr<GameObject>> m_gameObjects;
    std::unordered_multimap<int,int> m_objectsColliding;
};

template <typename TComponent, typename... TArgs>
void GameObject::AddComponent(TArgs&&... args)
{
    if(HasComponent<TComponent>())
    {
        Logger::Err("GameObject with id:" + std::to_string(m_id) +
        "; already have component: " + std::string(typeid(TComponent).name()));
    }
    // Create the component with the owner and forward the remaining arguments
    std::shared_ptr<TComponent> newComponent = std::make_shared<TComponent>(this, std::forward<TArgs>(args)...);

    // Insert the component into the unordered_map and the vector
    Components.insert(std::make_pair(std::type_index(typeid(TComponent)), newComponent));

    Logger::Log("GameObject with id:" + std::to_string(m_id) +
        "; added component: " + std::string(typeid(TComponent).name()));
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
