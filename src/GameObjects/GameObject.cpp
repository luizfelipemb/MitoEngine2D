#include "GameObject.h"

#include <iostream>
#include <ostream>

#include "../AssetStore/AssetManager.h"
#include "../AssetStore/RendererManager.h"
#include "../Events/CollisionEnterEvent.h"
#include "../Events/CollisionStayEvent.h"
#include "../Events/KeyReleasedEvent.h"
#include "../Game/Game.h"
#include "../Logger/Logger.h"


class CollisionExitEvent;
class CollisionEvent;

void TransformComponent::Update(float deltaTime)
{
}


void SpriteComponent::Update(float deltaTime)
{
	if(auto transform = m_owner->GetComponent<TransformComponent>())
		AssetManager::RenderImage(
			RendererManager::Renderer,
			m_sprite, 
			transform->Position.x,
			transform->Position.y,
			m_width,
			m_height, 
			transform->Scale,
			m_color);
}


ControllerComponent::ControllerComponent(GameObject* owner) : Component(owner)
{
	m_currentVelocity = glm::vec2(0,0);
	m_previousVelocity = m_currentVelocity;
	GlobalEventBus::SubscribeToEvent<KeyPressedEvent>(this, &ControllerComponent::OnKeyPressedEvent);
	GlobalEventBus::SubscribeToEvent<KeyReleasedEvent>(this, &ControllerComponent::OnKeyReleasedEvent);
	owner->LocalEventBus.SubscribeToEvent<CollisionStayEvent>(this, &ControllerComponent::OnCollisionStay);
	owner->LocalEventBus.SubscribeToEvent<CollisionEnterEvent>(this, &ControllerComponent::OnCollisionEnter);
	owner->LocalEventBus.SubscribeToEvent<CollisionExitEvent>(this, &ControllerComponent::OnCollisionExit);
}

void ControllerComponent::Update(float deltaTime)
{
	if(auto rigidBody = m_owner->GetComponent<RigidBody2DComponent>())
	{
		rigidBody->Velocity += m_currentVelocity - m_previousVelocity;
		Logger::Log(std::to_string(static_cast<int>(m_currentVelocity.x))+" "+
							std::to_string(static_cast<int>(m_currentVelocity.y)));
	}
	m_previousVelocity = m_currentVelocity;
}

void ControllerComponent::OnCollisionStay(CollisionStayEvent& event)
{
	m_colliding = true;
}

void ControllerComponent::OnCollisionEnter(CollisionEnterEvent& event)
{
	m_colliding = true;
	m_currentVelocity = glm::vec2(0,0);
}

void ControllerComponent::OnCollisionExit(CollisionExitEvent& event)
{
	m_colliding = false;
}

void ControllerComponent::OnKeyPressedEvent(KeyPressedEvent& event)
{
	if(auto rigidBody = m_owner->GetComponent<RigidBody2DComponent>())
	{
		switch (event.Symbol)
		{
		case SDLK_w:
			m_currentVelocity.y -= 500;
			break;
		case SDLK_d:
			m_currentVelocity.x += 500;
			break;
		case SDLK_s:
			m_currentVelocity.y += 500;
			break;
		case SDLK_a:
			m_currentVelocity.x -= 500;
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
			m_currentVelocity.y = 0;
			break;
		case SDLK_d:
			m_currentVelocity.x = 0;
			break;
		case SDLK_s:
			m_currentVelocity.y = 0;
			break;
		case SDLK_a:
			m_currentVelocity.x = 0;
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

BoxCollider2DComponent::BoxCollider2DComponent(GameObject* owner, int width, int height, glm::vec2 offset) :
	Component(owner), Width(width), Height(height), Offset(offset)
{
}

void BoxCollider2DComponent::Update(float deltaTime)
{
	
}

int GameObject::GetId() const
{
	return m_id;
}

std::unique_ptr<GameObject>& Registry::CreateGameObject(glm::vec3 position)
{
	m_gameObjects.emplace_back(std::make_unique<GameObject>(m_numberOfGameObjects,this));
	Logger::Log("GameObject with id:" + std::to_string(m_numberOfGameObjects) + " created");
	m_numberOfGameObjects++;
	return m_gameObjects.back();
}

void Registry::Update(float deltaTime)
{
	//Update GameObjects
	for (auto& gameObject : m_gameObjects)
	{
		for (auto& componentPair : gameObject->Components)
		{
			if (componentPair.second)
			{
				componentPair.second->Update(deltaTime);
			}
		}
	}

	CalculateCollisions();
}

const std::vector<std::unique_ptr<GameObject>>& Registry::GetAllGameObjects() const
{
	return m_gameObjects;
}

bool Registry::CheckAABBCollision(double aX, double aY, double aW, double aH, double bX, double bY, double bW,
	double bH)
{
	return (
		aX < bX + bW &&
		aX + aW > bX &&
		aY < bY + bH &&
		aY + aH > bY
		);
}

void Registry::CalculateCollisions()
{
	for (auto i = m_gameObjects.begin(); i != m_gameObjects.end(); ++i)
	{
		const std::unique_ptr<GameObject>& a = *i;

		if (!a->HasComponent<TransformComponent>() || !a->HasComponent<BoxCollider2DComponent>())
			continue;

		TransformComponent* aTransform = a->GetComponent<TransformComponent>();
		BoxCollider2DComponent* aCollider = a->GetComponent<BoxCollider2DComponent>();

		// Loop all the entities that still need to be checked (to the right of i)
		for (auto j = i; j != m_gameObjects.end(); ++j)
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
			
			bool wasColliding = false;
			auto range = m_objectsColliding.equal_range(a->GetId());
			for (auto it = range.first; it != range.second; ++it)
			{
				if (it->second == b->GetId())
				{
					wasColliding = true;
					break;
				}
			}
			if (collisionHappened)
			{
				if(!wasColliding)
				{
					Logger::Log("Entities " + std::to_string(a->GetId()) + " and " + std::to_string(b->GetId()) + " started colliding!");
					a->LocalEventBus.EmitEvent<CollisionEnterEvent>(b);
					b->LocalEventBus.EmitEvent<CollisionEnterEvent>(a);
					a->LocalEventBus.EmitEvent<CollisionStayEvent>(b);
					b->LocalEventBus.EmitEvent<CollisionStayEvent>(a);
					m_objectsColliding.insert(std::make_pair(a->GetId(),b->GetId()));
				}else
				{
					a->LocalEventBus.EmitEvent<CollisionStayEvent>(b);
					b->LocalEventBus.EmitEvent<CollisionStayEvent>(a);
				}
			}
			else
			{
				if(wasColliding)
				{
					Logger::Log("Entities " + std::to_string(a->GetId()) + " and " + std::to_string(b->GetId()) + " exit colliding!");
					a->LocalEventBus.EmitEvent<CollisionExitEvent>(b);
                    b->LocalEventBus.EmitEvent<CollisionExitEvent>(a);
					
					// Find and remove the specific key-value pair
					auto range = m_objectsColliding.equal_range(a->GetId());
					for (auto it = range.first; it != range.second; ++it) {
						if (it->second == b->GetId()) {
							m_objectsColliding.erase(it);
							break;
						}
					}
				}
			}
		}
	}
}


