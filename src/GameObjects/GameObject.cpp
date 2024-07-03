#include "GameObject.h"

#include <iostream>
#include <ostream>

#include "../AssetStore/AssetManager.h"
#include "../Events/KeyReleasedEvent.h"
#include "../Game/Game.h"
#include "../Logger/Logger.h"


class CollisionEvent;

void TransformComponent::Update(float deltaTime)
{
}


void SpriteComponent::Update(float deltaTime)
{
	if(auto transform = m_owner->GetComponent<TransformComponent>())
		AssetManager::RenderImage(
			m_sprite, 
			transform->Position.x,
			transform->Position.y,
			m_width,
			m_height, 
			transform->Scale,
			m_color);
}


void ControllerComponent::Update(float deltaTime)
{
}

void ControllerComponent::OnKeyPressedEvent(KeyPressedEvent& event)
{
	if( auto rigidBody = m_owner->GetComponent<RigidBody2DComponent>())
	{
		switch (event.Symbol)
		{
		case SDLK_w:
			Logger::Log("ControllerComponent Key Up Pressed");
			rigidBody->Velocity.y -= 500;
			break;
		case SDLK_d:
			Logger::Log("ControllerComponent Key Right Pressed");
			rigidBody->Velocity.x += 500;
			break;
		case SDLK_s:
			Logger::Log("ControllerComponent Key Down Pressed");
			rigidBody->Velocity.y += 500;
			break;
		case SDLK_a:
			Logger::Log("ControllerComponent Key Left Pressed");
			rigidBody->Velocity.x -= 500;
			break;
		}
	}
	
}

void ControllerComponent::OnKeyReleasedEvent(KeyReleasedEvent& event)
{
	if (auto rigidBody = m_owner->GetComponent<RigidBody2DComponent>())
	{
		switch (event.Symbol)
		{
		case SDLK_w:
			Logger::Log("ControllerComponent Key Up Released");
			rigidBody->Velocity.y += 500;
			break;
		case SDLK_d:
			Logger::Log("ControllerComponent Key Right Released");
			rigidBody->Velocity.x -= 500;
			break;
		case SDLK_s:
			Logger::Log("ControllerComponent Key Down Released");
			rigidBody->Velocity.y -= 500;
			break;
		case SDLK_a:
			Logger::Log("ControllerComponent Key Left Released");
			rigidBody->Velocity.x += 500;
			break;
		}
	}
}

void RigidBody2DComponent::Update(float deltaTime)
{
	if (auto transform = m_owner->GetComponent<TransformComponent>())
	{
		transform->Position.x += Velocity.x * deltaTime;
		transform->Position.y += Velocity.y * deltaTime;
	}
}

void BoxCollider2DComponent::Update(float deltaTime)
{
	auto& entities = m_owner->RefRegistry->GetAllGameObjects();
	
	for (auto i = entities.begin(); i != entities.end(); ++i)
	{
		const std::unique_ptr<GameObject>& a = *i;

		if (!a->HasComponent<TransformComponent>() || !a->HasComponent<BoxCollider2DComponent>())
			continue;

		TransformComponent* aTransform = a->GetComponent<TransformComponent>();
		BoxCollider2DComponent* aCollider = a->GetComponent<BoxCollider2DComponent>();

		// Loop all the entities that still need to be checked (to the right of i)
		for (auto j = i; j != entities.end(); ++j)
		{
			const std::unique_ptr<GameObject>& b = *j;

			if (!b->HasComponent<TransformComponent>() || !b->HasComponent<BoxCollider2DComponent>())
				continue;

			// Bypass if we are trying to test the same entity
			if (a == b)
				continue;

			auto bTransform = b->GetComponent<TransformComponent>();
			auto bCollider = b->GetComponent<BoxCollider2DComponent>();

			// Perform the AABB collision check between entities a and b
			bool collisionHappened = CheckAABBCollision(
				aTransform->Position.x + aCollider->Offset.x,
				aTransform->Position.y + aCollider->Offset.y,
				aCollider->Width,
				aCollider->Height,
				bTransform->Position.x + bCollider->Offset.x,
				bTransform->Position.y + bCollider->Offset.y,
				bCollider->Width,
				bCollider->Height
			);

			if (collisionHappened)
			{
				Logger::Log("Entities collided!");
				EventBus::EmitEvent<CollisionEvent>(a, b);
			}
		}
	}
}

bool BoxCollider2DComponent::CheckAABBCollision(double aX, double aY, double aW, double aH, double bX, double bY,
	double bW, double bH)
{
	return (
		aX < bX + bW &&
		aX + aW > bX &&
		aY < bY + bH &&
		aY + aH > bY
		);
}

void Registry::Update(float deltaTime)
{
	for (auto& gameObject : gameObjects)
	{
		for (auto& componentPair : gameObject->Components)
		{
			if (componentPair.second)
			{
				componentPair.second->Update(deltaTime);
			}
		}
	}
}

const std::vector<std::unique_ptr<GameObject>>& Registry::GetAllGameObjects() const
{
	return gameObjects;
}

//   REGISTRY  /////////////////////////////////////////////////////////////////////////
std::unique_ptr<GameObject>& Registry::CreateGameObject(glm::vec3 position)
{
	gameObjects.emplace_back(std::make_unique<GameObject>(this));
	return gameObjects.back();
}

