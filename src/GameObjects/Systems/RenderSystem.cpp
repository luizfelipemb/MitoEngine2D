#include "RenderSystem.h"

#include "../GameObject.h"

void RenderSystem::SortByLayer(std::vector<std::shared_ptr<GameObject>>& m_gameObjects)
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

void RenderSystem::Update(std::vector<std::shared_ptr<GameObject>>& m_gameObjects)
{
    SortByLayer(m_gameObjects);

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
        }
    }
}
