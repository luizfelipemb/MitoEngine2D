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
	Component* GetComponent(std::string comp);
	template <typename TComponent, typename ...TArgs> void AddComponent(TArgs&& ...args);
private:
	std::unordered_map<std::type_index, std::shared_ptr<Component>> m_components;
};


class Registry
{
public:
	std::shared_ptr<GameObject>& CreateGameObject(glm::vec3 position = glm::vec3(0,0,0));
	template <typename TComponent, typename ...TArgs> void AddComponent(GameObject entity, TArgs&& ...args);
private:
	std::vector<std::shared_ptr<GameObject>> gameObjects;
};