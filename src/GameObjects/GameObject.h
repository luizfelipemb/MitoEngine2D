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
	~Component() = default;
protected:
};


class TransformComponent : public Component
{
public:

protected:
	
};


class GameObject
{
public:
	//void AddComponent(Component* comp,std::string name);
	Component* GetComponent(std::string comp);

	template <typename TComponent, typename ...TArgs> void AddComponent(TArgs&& ...args);
private:
	std::unordered_map<std::type_index, std::shared_ptr<Component>> m_components;
};


class Registry
{
public:
	GameObject CreateGameObject(glm::vec3 position = glm::vec3(0,0,0));
	//void InstantiateComponent(Component* comp, std::string name);
	//void AddComponentTo(GameObject* gameobject, std::string componentName);
	//Component* GetComponentOf(GameObject* gameobject, std::string componentName);

	template <typename TComponent, typename ...TArgs> void AddComponent(GameObject entity, TArgs&& ...args);
private:
};

template <typename TComponent, typename ...TArgs>
void GameObject::AddComponent(TArgs&& ...args) {

	TComponent newComponent(std::forward<TArgs>(args)...);

	std::shared_ptr<TComponent> newSystem = std::make_shared<TComponent>(std::forward<TArgs>(args)...);
	m_components.insert(std::make_pair(std::type_index(typeid(TComponent)), newSystem));

	Logger::Log("GameObject::AddComponent called");
}

template <typename TComponent, typename ...TArgs>
void Registry::AddComponent(GameObject entity, TArgs&& ...args) {

	entity.AddComponent<TComponent>(std::forward<TArgs>(args)...);
}

