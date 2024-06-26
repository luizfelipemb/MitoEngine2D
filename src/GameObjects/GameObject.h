#pragma once
#include <memory>
#include <stdexcept>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <vec2.hpp>
#include <vec3.hpp>
#include <vector>

#include "../Logger/Logger.h"

class Component
{
public:
	Component() = default;
	virtual ~Component() = default;
	virtual void Update() = 0;
protected:
};


class TransformComponent : public Component
{
public:
	TransformComponent(glm::vec2 position = glm::vec2(0, 0), glm::vec2 scale = glm::vec2(1, 1), double rotation = 0.0);
	void Update() override;

private:
	glm::vec2 position;
	glm::vec2 scale;
	double rotation;
};

class SpriteComponent : public Component
{
public:
	SpriteComponent(std::string sprite);
	void Update() override;
private:
	std::string sprite;
};


class GameObject
{
public:
	Component* GetComponent(std::string comp);
	template <typename TComponent, typename ...TArgs> void AddComponent(TArgs&& ...args);
	template <typename TComponent> TComponent& GetComponent();
	std::vector<std::shared_ptr<Component>> Components;
private:
	std::unordered_map<std::type_index, std::shared_ptr<Component>> m_components;
};


class Registry
{
public:
	void Update();
	GameObject* CreateGameObject(glm::vec3 position = glm::vec3(0,0,0));
	template <typename TComponent, typename ...TArgs> void AddComponent(GameObject entity, TArgs&& ...args);
private:
	std::vector<GameObject> gameObjects;
};

//   GAMEOBJECT  /////////////////////////////////////////////////////////////////////////
template <typename TComponent, typename ...TArgs>
void GameObject::AddComponent(TArgs&& ...args) {

	TComponent newComponent(std::forward<TArgs>(args)...);

	std::shared_ptr<TComponent> newSystem = std::make_shared<TComponent>(std::forward<TArgs>(args)...);
	m_components.insert(std::make_pair(std::type_index(typeid(TComponent)), newSystem));
	Components.emplace_back(newSystem);

	Logger::Log("GameObject::AddComponent called");
}

template <typename TComponent>
TComponent& GameObject::GetComponent()
{
	auto it = m_components.find(typeid(TComponent));
	if (it != m_components.end())
	{
		return *std::static_pointer_cast<TComponent>(it->second);
	}
	else
	{
		Logger::Err("Component not found");
		throw std::runtime_error("Component not found");
	}
}

template <typename TComponent, typename ...TArgs>
void Registry::AddComponent(GameObject entity, TArgs&& ...args) {

	entity.AddComponent<TComponent>(std::forward<TArgs>(args)...);
}

