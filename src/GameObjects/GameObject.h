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

class Registry;
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
	SpriteComponent(GameObject* owner, std::string sprite, int width = 100, int height = 100, std::uint8_t red = 255, std::uint8_t green = 255, std::uint8_t blue = 255) :
		Component(owner), m_sprite(sprite), m_width(width),m_height(height),m_color(Color{red,green,blue}) {}
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

class BoxCollider2DComponent : public Component
{
public:
	BoxCollider2DComponent(GameObject* owner, int width = 0, int height = 0, glm::vec2 offset = glm::vec2(0)) :
		Component(owner), Width(width), Height(height), Offset(offset) {}

	void Update(float deltaTime) override;
	bool CheckAABBCollision(double aX, double aY, double aW, double aH, double bX, double bY, double bW, double bH);
	int Width;
	int Height;
	glm::vec2 Offset;
private:
};

class GameObject
{
public:
	GameObject(Registry* reg): RefRegistry(reg) { Logger::Log("GameObject initialized"); }
	template <typename TComponent, typename ...TArgs> void AddComponent(TArgs&& ...args);
	template <typename TComponent> TComponent* GetComponent();
	template <typename TComponent> bool HasComponent();

	std::unordered_map<std::type_index, std::shared_ptr<Component>> Components;

	Registry* RefRegistry;
private:
};


class Registry
{
public:
	void Update(float deltaTime);
	const std::vector<std::unique_ptr<GameObject>>& GetAllGameObjects() const;

	std::unique_ptr<GameObject>& CreateGameObject(glm::vec3 position = glm::vec3(0,0,0));
private:
	std::vector<std::unique_ptr<GameObject>> gameObjects;
};

//   GAMEOBJECT  /////////////////////////////////////////////////////////////////////////
template <typename TComponent, typename ...TArgs>
void GameObject::AddComponent(TArgs&& ...args)
{
	// Create the component with the owner and forward the remaining arguments
	std::shared_ptr<TComponent> newComponent = std::make_shared<TComponent>(this, std::forward<TArgs>(args)...);

	// Insert the component into the unordered_map and the vector
	Components.insert(std::make_pair(std::type_index(typeid(TComponent)), newComponent));

	Logger::Log("GameObject::AddComponent: Added component of type " + std::string(typeid(TComponent).name()));

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
