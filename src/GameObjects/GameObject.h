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

class KeyPressedEvent;
class GameObject;

class Component
{
public:
	Component(GameObject* owner) : owner(owner) {}
	virtual ~Component() = default;
	virtual void Update() = 0;

protected:
	GameObject* owner;
};

class TransformComponent : public Component
{
public:
    TransformComponent(GameObject* owner, 
                       glm::vec2 position = glm::vec2(0, 0), 
                       double scale = 1, 
                       double rotation = 0.0)
        : Component(owner), m_position(position), m_scale(scale), m_rotation(rotation) {}

    void Update() override;
	glm::vec2 GetPosition() const { return m_position; }
	double GetScale() const { return m_scale; }
	double GetRotation() const { return m_rotation; }

private:
    glm::vec2 m_position;
	double m_scale;
    double m_rotation;
};

class SpriteComponent : public Component
{
public:
	SpriteComponent(GameObject* owner, std::string sprite) : Component(owner), sprite(sprite)
	{
		EventBus::SubscribeToEvent<KeyPressedEvent>(this, &SpriteComponent::OnKeyPressedEvent);
	}
	void Update() override;
	void OnKeyPressedEvent(KeyPressedEvent& event) { Logger::Log("Key pressed:" + std::to_string(event.symbol)); }
private:
	std::string sprite;
};


class GameObject
{
public:
	Component* GetComponent(std::string comp);
	template <typename TComponent, typename ...TArgs> void AddComponent(TArgs&& ...args);
	template <typename TComponent> TComponent* GetComponent();
	std::vector<std::shared_ptr<Component>> Components;
private:
	std::unordered_map<std::type_index, std::shared_ptr<Component>> m_components;
};


class Registry
{
public:
	void Update();
	GameObject* CreateGameObject(glm::vec3 position = glm::vec3(0,0,0));
private:
	std::vector<GameObject> gameObjects;
};

//   GAMEOBJECT  /////////////////////////////////////////////////////////////////////////
template <typename TComponent, typename ...TArgs>
void GameObject::AddComponent(TArgs&& ...args)
{
	// Create the component with the owner and forward the remaining arguments
	std::shared_ptr<TComponent> newComponent = std::make_shared<TComponent>(this, std::forward<TArgs>(args)...);

	// Insert the component into the unordered_map and the vector
	m_components.insert(std::make_pair(std::type_index(typeid(TComponent)), newComponent));
	Components.emplace_back(newComponent);

	Logger::Log("GameObject::AddComponent called");

}

template <typename TComponent>
TComponent* GameObject::GetComponent()
{
	auto it = m_components.find(typeid(TComponent));
	if (it != m_components.end())
	{
		return std::static_pointer_cast<TComponent>(it->second).get();
	}
	else
	{
		Logger::Err("Component not found");
		return nullptr;
	}
}

