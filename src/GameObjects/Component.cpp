#include "Component.h"


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
		//Logger::Log(std::to_string(static_cast<int>(m_currentVelocity.x))+" "+
		//					std::to_string(static_cast<int>(m_currentVelocity.y)));
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

void ScriptComponent::AddScript(sol::state& lua)
{
	if(lua["start"] != sol::lua_nil)
	{
		StartFunc.push_back(lua["start"]);
	}
	if(lua["update"] != sol::lua_nil)
	{
		UpdateFunc.push_back(lua["update"]);
	}
	if(lua["on_key_pressed"] != sol::lua_nil)
	{
		GlobalEventBus::SubscribeToEvent<KeyPressedEvent>(
			[this, &lua](KeyPressedEvent& event)
			{
				sol::function luaEventFunc = lua["on_key_pressed"];
				if (luaEventFunc != sol::lua_nil)
				{
					luaEventFunc(m_owner, std::to_string(event.Symbol));
				}
			});
	}
}

void ScriptComponent::Update(float deltaTime)
{
	
}

void ScriptComponent::CallUpdate()
{
	for (auto& func : UpdateFunc)
	{
		if(func != sol::lua_nil)
			func(m_owner);
	}
}

void ScriptComponent::CallStart()
{
	for (auto& func : StartFunc)
	{
		if(func != sol::lua_nil)
			func(m_owner);
	}
}
