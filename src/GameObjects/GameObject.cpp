#include "GameObject.h"

#include "../Game/Game.h"
#include "../Logger/Logger.h"


//   GAMEOBJECT  /////////////////////////////////////////////////////////////////////////
template <typename TComponent, typename ...TArgs>
void GameObject::AddComponent(TArgs&& ...args) {

	TComponent newComponent(std::forward<TArgs>(args)...);

	std::shared_ptr<TComponent> newSystem = std::make_shared<TComponent>(std::forward<TArgs>(args)...);
	m_components.insert(std::make_pair(std::type_index(typeid(TComponent)), newSystem));

	Logger::Log("GameObject::AddComponent called");
}

//   REGISTRY  /////////////////////////////////////////////////////////////////////////
std::shared_ptr<GameObject>& Registry::CreateGameObject(glm::vec3 position)
{
	Logger::Log("GameObject Created");
	std::shared_ptr<GameObject> newGO = std::make_shared<GameObject>();
	gameObjects.emplace_back(newGO);
	return gameObjects.back();
}

template <typename TComponent, typename ...TArgs>
void Registry::AddComponent(GameObject entity, TArgs&& ...args) {

	entity.AddComponent<TComponent>(std::forward<TArgs>(args)...);
}

