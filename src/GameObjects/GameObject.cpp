#include "GameObject.h"
#include "../Events/CollisionEnterEvent.h"
#include "../Events/CollisionStayEvent.h"
#include "../Events/GameObjectCreatedEvent.h"
#include "../Logger/Logger.h"

class CollisionExitEvent;
class CollisionEvent;

GameObject::~GameObject()
{
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
    Registry::TagEntity(m_id, tag);
}

bool GameObject::HasTag(const std::string& tag)
{
    Logger::Log("Has Tag id " + std::to_string(m_id) + " and tag:" + tag
        + " result: " + std::to_string(Registry::EntityHasTag(m_id, tag)));
    return Registry::EntityHasTag(m_id, tag);
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
    Logger::Log("Cleared GameObjects, new size:" + std::to_string(m_gameObjects.size()));
}

void Registry::TagEntity(int entityid, const std::string& tag)
{
    m_tagPerGameObjectId[entityid].insert(tag);
    m_gameObjectIdPerTag[tag].insert(entityid);
    
    std::cout << "Entity ID " << entityid << " tagged with '" << tag << "'.\n";
}

bool Registry::EntityHasTag(int entityid, const std::string& tag)
{
    std::cout << "Checking if entity ID " << entityid << " has tag '" << tag << "'...\n";

    // Check if the entity ID exists in m_tagPerGameObjectId
    auto entityIt = m_tagPerGameObjectId.find(entityid);
    if (entityIt == m_tagPerGameObjectId.end())
    {
        std::cout << "Entity ID " << entityid << " not found in m_tagPerGameObjectId.\n";
        return false;
    }

    // Check if the tag exists in the entity's set of tags
    if (entityIt->second.find(tag) == entityIt->second.end())
    {
        std::cout << "Tag '" << tag << "' not associated with entity ID " << entityid << ".\n";
        return false;
    }

    std::cout << "Entity ID " << entityid << " is correctly associated with tag '" << tag << "'.\n";
    return true;
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
    
    //TODO: Layer sort not every frame
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

const std::vector<std::shared_ptr<GameObject>>& Registry::GetAllGameObjects() const
{
    return m_gameObjects;
}