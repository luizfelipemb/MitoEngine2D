#include "RenderSystem.h"

#include "../GameObject.h"

RenderSystem::RenderSystem(const std::vector<std::shared_ptr<GameObject>>& gameObjects) :
    m_gameObjects(gameObjects)
{
    GlobalEventBus::SubscribeToEvent<GameObjectCreatedEvent>
        (this,&RenderSystem::OnObjectCreation);
}

void RenderSystem::OnObjectCreation(GameObjectCreatedEvent& gameObjectCreatedEvent)
{
    Logger::Log("RenderSystem::OnObjectCreation");
    SortByLayer();
}

void RenderSystem::SortByLayer()
{
    layeredGameObjects.clear();
    for (const auto& gameObject : m_gameObjects)
    {
        if (gameObject->HasComponent<SpriteComponent>())
        {
            auto layer = gameObject->GetComponent<SpriteComponent>()->Layer;
            layeredGameObjects[layer].emplace_back(gameObject);
        }
    }
}

void RenderSystem::Update()
{
    //TODO: text render support
    for (const auto& [layer, objects] : layeredGameObjects)
    {
        for (const auto& weakPtr : objects)
        {
            if (auto gameObject = weakPtr.lock())
            {
                if (gameObject->HasComponent<SpriteComponent>())
                {
                    gameObject->GetComponent<SpriteComponent>()->Render();
                }
            }
            else
            {
                Logger::Log("RenderSystem::Update: weakPtr null");
            }
        }
    }
}
