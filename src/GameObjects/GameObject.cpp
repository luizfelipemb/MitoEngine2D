#include "GameObject.h"

#include "../AssetStore/AssetManager.h"
#include "../Game/Game.h"
#include "../Logger/Logger.h"


TransformComponent::TransformComponent(glm::vec2 position, glm::vec2 scale, double rotation):
	position(position),scale(scale),rotation(rotation)
{
}

void TransformComponent::Update()
{
	Logger::Log("TransformComponent::Update");
}


SpriteComponent::SpriteComponent(std::string sprite) : sprite(sprite)
{

}

void SpriteComponent::Update()
{
	Logger::Log("SpriteComponent::Update with sprite:"+sprite);
	AssetManager::RenderImage(sprite, 0, 0, 100, 100, 1);
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

