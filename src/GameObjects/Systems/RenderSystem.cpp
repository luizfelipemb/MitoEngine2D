#include "RenderSystem.h"

#include "../GameObject.h"
#include "../../Game/Game.h"

RenderSystem::RenderSystem(std::vector<std::shared_ptr<GameObject>>& gameObjects) :
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
        if (gameObject->HasComponentBase<RenderableComponent>())
        {
            auto layer = gameObject->GetComponentBase<RenderableComponent>()->Layer;
            //TODO: store component instead of gameobject
            layeredGameObjects[layer].emplace_back(gameObject);
        }
    }
}

void RenderSystem::Update()
{
    for (const auto& [layer, objects] : layeredGameObjects)
    {
        for (const auto& weakPtr : objects)
        {
            if (auto gameObject = weakPtr.lock())
            {
                gameObject->GetComponentBase<RenderableComponent>()->Render();
            }
            else
            {
                //Logger::Log("RenderSystem::Update: weakPtr null");
            }
        }
    }
    
    if(g_DebugMode)
    {
        for(const auto& gameobject : m_gameObjects)
            {
                if(gameobject->HasComponent<BoxCollider2DComponent>() && gameobject->HasComponent<TransformComponent>())
                {
                    auto transform = gameobject->GetComponent<TransformComponent>();
                    auto boxco = gameobject->GetComponent<BoxCollider2DComponent>();
                    AssetManager::DrawBorderRectangle(
                            RendererManager::Renderer,
                            transform->Position.x,
                            transform->Position.y,
                            boxco->Width,
                            boxco->Height,
                            {0,0,255});
        
                }
            }
    }
    
}
