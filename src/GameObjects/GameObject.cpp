#include "GameObject.h"
#include "../Events/CollisionEnterEvent.h"
#include "../Events/CollisionStayEvent.h"
#include "../Events/GameObjectCreatedEvent.h"
#include "../Logger/Logger.h"

class CollisionExitEvent;
class CollisionEvent;

GameObject::~GameObject()
{
    GlobalEventBus::UnsubscribeFromOwner(this);
    if (HasComponent<ScriptComponent>())
    {
        GlobalEventBus::UnsubscribeFromOwner(GetComponent<ScriptComponent>());
    }
}

int GameObject::GetId() const
{
    return m_id;
}

std::string GameObject::GetName() const
{
    return Name;
}

void GameObject::Tag(const std::string& tag)
{
    Logger::Log("Tagged id " + std::to_string(m_id) + " with tag:" + tag);
    Registry::TagGameObject(m_id, tag);
}

bool GameObject::HasTag(const std::string& tag)
{
    Logger::Log("Has Tag id " + std::to_string(m_id) + " and tag:" + tag
        + " result: " + std::to_string(Registry::GameObjectHasTag(m_id, tag)));
    return Registry::GameObjectHasTag(m_id, tag);
}

std::shared_ptr<GameObject>& Registry::CreateGameObject(std::string name)
{
    if (name.empty())
    {
        name = "GameObject";
    }
    m_gameObjects.emplace_back(std::make_unique<GameObject>(m_nextFreeId, name));
    Logger::Log("GameObject with id:" + std::to_string(m_nextFreeId) + " created");
    m_nextFreeId++;
    return m_gameObjects.back();
}

void Registry::DestroyGameObject(int id)
{
    m_idsToDestroy.emplace_back(id);
}

void Registry::ClearGameObjects()
{
    m_gameObjects.clear();
}

void Registry::TagGameObject(int id, const std::string& tag)
{
    m_tagPerGameObjectId[id].insert(tag);
    m_gameObjectIdPerTag[tag].insert(id);
}

bool Registry::GameObjectHasTag(int id, const std::string& tag)
{
    // Check if the entity ID exists in m_tagPerGameObjectId
    auto entityIt = m_tagPerGameObjectId.find(id);
    if (entityIt == m_tagPerGameObjectId.end())
    {
        return false;
    }

    // Check if the tag exists in the entity's set of tags
    if (entityIt->second.find(tag) == entityIt->second.end())
    {
        return false;
    }
    return true;
}

void Registry::ClearTags()
{
    m_gameObjectIdPerTag.clear();
    m_tagPerGameObjectId.clear();
}

GameObject* Registry::GetGameObjectByTag(const std::string& tag)
{
    return GetGameObjectById(*m_gameObjectIdPerTag[tag].begin());
}

void Registry::Start()
{
    std::vector<GameObject*> temp;
    for (auto& entity : m_gameObjects)
    {
        temp.emplace_back(entity.get());
    }

    for (auto entity : temp)
    {
        if (entity->HasComponent<ScriptComponent>())
        {
            auto script = entity->GetComponent<ScriptComponent>();
            script->CallStart();
        }
    }
    
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
    for (auto& entity : m_gameObjects)
    {
        if (entity->HasComponent<ScriptComponent>())
        {
            auto script = entity->GetComponent<ScriptComponent>();
            script->CallUpdate(deltaTime);
        }
    }

    m_collisionSystem.CalculateCollisions(m_gameObjects);
    m_renderSystem.Update();
    
    for (auto& id : m_idsToDestroy)
    {
        Logger::Log("Trying to destroy id:" + std::to_string(id));
        auto it = std::find_if(m_gameObjects.begin(), m_gameObjects.end(),
                               [id](const std::shared_ptr<GameObject>& obj)
                               {
                                   return obj->GetId() == id;
                               });

        if (it != m_gameObjects.end())
        {
            Logger::Log("GameObject with id:" + std::to_string(id) + " destroyed");
            if (it->get()->HasComponent<ScriptComponent>())
            {
                GlobalEventBus::UnsubscribeFromOwner(it->get()->GetComponent<ScriptComponent>());
                it->get()->LocalEventBus.Reset();
            }

            m_gameObjects.erase(it);
        }
        else
        {
            Logger::Log("GameObject with id:" + std::to_string(id) + " not found");
        }
    }
    m_idsToDestroy.clear();
}
GameObject* Registry::GetGameObjectById(int id)
{
    auto it = std::find_if(m_gameObjects.begin(), m_gameObjects.end(),
                               [id](const std::shared_ptr<GameObject>& obj)
                               {
                                   return obj->GetId() == id;
                               });
    if (it != m_gameObjects.end())
    {
        return it->get();
    }
    return nullptr;
}

const std::vector<std::shared_ptr<GameObject>>& Registry::GetAllGameObjects() const
{
    return m_gameObjects;
}

