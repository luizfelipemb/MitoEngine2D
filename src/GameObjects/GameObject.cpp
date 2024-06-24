#include "GameObject.h"

#include "../Logger/Logger.h"


//   GAMEOBJECT  /////////////////////////////////////////////////////////////////////////
//void GameObject::AddComponent(Component* comp, std::string name)

	//Logger::Log("GameObject::AddComponent of name:" + name);
	//m_components[name] = comp;


//Component* GameObject::GetComponent(std::string comp)

	//return m_components[comp];



//   REGISTRY  /////////////////////////////////////////////////////////////////////////
GameObject Registry::CreateGameObject(glm::vec3 position)
{
	Logger::Log("GameObject Created");
	return GameObject{};
}

//void Registry::InstantiateComponent(Component* comp, std::string name)

	//Logger::Log("InstantiateComponent of name: " + name);
	//m_components[name] = comp;


//void Registry::AddComponentTo(GameObject* gameobject, std::string componentName)

	//Logger::Log("Registry::AddComponentTo of name:"+componentName);
	//Component newInstanceOfComponent = *m_components[componentName];
	//gameobject->AddComponent(&newInstanceOfComponent,componentName);


//Component* Registry::GetComponentOf(GameObject* gameobject, std::string componentName)

	//return gameobject->GetComponent(componentName);

