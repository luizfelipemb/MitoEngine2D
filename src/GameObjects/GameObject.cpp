#include "GameObject.h"

#include <iostream>
#include <ostream>

#include "../AssetStore/AssetManager.h"
#include "../Game/Game.h"
#include "../Logger/Logger.h"


void TransformComponent::Update(float deltaTime)
{
}


void SpriteComponent::Update(float deltaTime)
{
	if(auto transform = owner->GetComponent<TransformComponent>())
		AssetManager::RenderImage(
			sprite, 
			transform->Position.x, 
			transform->Position.y,
			100, 100, 
			transform->Scale);
}


void ControllerComponent::Update(float deltaTime)
{
}

void ControllerComponent::OnKeyPressedEvent(KeyPressedEvent& event)
{
	switch (event.symbol)
	{
		case SDLK_w:
			Logger::Log("ControllerComponent Key Up Pressed");
			break;
		case SDLK_d:
			Logger::Log("ControllerComponent Key Right Pressed");
			break;
		case SDLK_s:
			Logger::Log("ControllerComponent Key Down Pressed");
			break;
		case SDLK_a:
			Logger::Log("ControllerComponent Key Left Pressed");
			break;
	}
}

void RigidBody2DComponent::Update(float deltaTime)
{
	if (auto transform = owner->GetComponent<TransformComponent>())
	{
		transform->Position.x += m_velocity.x * deltaTime;
		transform->Position.y += m_velocity.y * deltaTime;
	}
}


void Registry::Update(float deltaTime)
{
	
		for (auto& x : gameObjects)
		{
			for (auto& y : x.Components)
			{
				if (y.second) // Check if the shared_ptr is not null
				{
					y.second->Update(deltaTime);
				}
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

