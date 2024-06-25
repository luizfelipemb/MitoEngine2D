#include "GameObject.h"

#include "../Game/Game.h"
#include "../Logger/Logger.h"


void TransformComponent::Update()
{
	Logger::Log("TransformComponent::Update");
}

void Registry::Update()
{
	Logger::Log("Registry::Update with gameobjects:" + gameObjects.size());
	for(auto x : gameObjects)
	{
		for (auto y : x.Components)
		{
			y->Update();
		}
	}
}

//   REGISTRY  /////////////////////////////////////////////////////////////////////////
GameObject* Registry::CreateGameObject(glm::vec3 position)
{
	Logger::Log("GameObject Created");
	GameObject newGO = {};
	gameObjects.emplace_back(newGO);
	return &gameObjects.back();
}

