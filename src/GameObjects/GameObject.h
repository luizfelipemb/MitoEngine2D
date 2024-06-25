#pragma once
#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>
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
	void Update() override;
protected:
	
};


class GameObject
{
public:
	Component* GetComponent(std::string comp);
	template <typename TComponent, typename ...TArgs> void AddComponent(TArgs&& ...args);
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

template <typename TComponent, typename ...TArgs>
void Registry::AddComponent(GameObject entity, TArgs&& ...args) {

	entity.AddComponent<TComponent>(std::forward<TArgs>(args)...);
}

