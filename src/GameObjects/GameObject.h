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
	virtual void Update(float deltaTime) = 0;

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
        : Component(owner), Position(position), Scale(scale), Rotation(rotation) {}

    void Update(float deltaTime) override;

    glm::vec2 Position;
	double Scale;
    double Rotation;
};

class SpriteComponent : public Component
{
public:
	SpriteComponent(GameObject* owner, std::string sprite) : Component(owner), sprite(sprite)
	{
	}
	void Update(float deltaTime) override;
private:
	std::string sprite;
};

class ControllerComponent : public Component
{
public:
	ControllerComponent(GameObject* owner) : Component(owner)
	{
		EventBus::SubscribeToEvent<KeyPressedEvent>(this, &ControllerComponent::OnKeyPressedEvent);
	}
	void Update(float deltaTime) override;
	void OnKeyPressedEvent(KeyPressedEvent& event);
private:
};

class RigidBody2DComponent : public Component
{
public:
	RigidBody2DComponent(GameObject* owner, glm::vec2 velocity = glm::vec2(0.0, 0.0)) :
		Component(owner), m_velocity(velocity) {}

	void Update(float deltaTime) override;
private:
	glm::vec2 m_velocity;
};

class GameObject
{
public:
	Component* GetComponent(std::string comp);
	template <typename TComponent, typename ...TArgs> void AddComponent(TArgs&& ...args);
	template <typename TComponent> TComponent* GetComponent();
	std::unordered_map<std::type_index, std::shared_ptr<Component>> Components;
private:
};


class Registry
{
public:
	void Update(float deltaTime);
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
	Components.insert(std::make_pair(std::type_index(typeid(TComponent)), newComponent));

	Logger::Log("GameObject::AddComponent called");

}

template <typename TComponent>
TComponent* GameObject::GetComponent()
{
	auto it = Components.find(typeid(TComponent));
	if (it != Components.end())
	{
		Logger::Log("Component found");
		return std::static_pointer_cast<TComponent>(it->second).get();
	}
	else
	{
		Logger::Err("Component not found");
		return nullptr;
	}
}

