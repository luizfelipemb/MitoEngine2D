#include "RenderSystem.h"

#include "../GameObject.h"

void RenderSystem::Update(std::vector<std::unique_ptr<GameObject>>& m_gameObjects)
{
    for (auto& gameObject : m_gameObjects)
    {
        if(gameObject->HasComponent<SpriteComponent>())
        {
            gameObject->GetComponent<SpriteComponent>()->Render();
        }
        
    }
}
