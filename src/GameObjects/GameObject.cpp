#include "GameObject.h"

#include "../AssetStore/AssetManager.h"
#include "../Game/Game.h"
#include "../Logger/Logger.h"


void TransformComponent::Update()
{
}


void SpriteComponent::Update()
{
	if(auto transform = owner->GetComponent<TransformComponent>())
		AssetManager::RenderImage(
			sprite, 
			transform->GetPosition().x, 
			transform->GetPosition().y, 
			100, 100, 
			transform->GetScale());
}


void Registry::Update()
{
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
	GameObject newGO = {};
	gameObjects.emplace_back(newGO);
	return &gameObjects.back();
}

