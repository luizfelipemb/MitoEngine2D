#pragma once
#include <map>
#include <memory>
#include <vector>

class GameObjectCreatedEvent;
class GameObject;

class RenderSystem
{
public:
    RenderSystem(std::vector<std::shared_ptr<GameObject>>& gameObjects);
    void SortByLayer();
    void Update();
    void OnObjectCreation(GameObjectCreatedEvent& gameObjectCreatedEvent);
private:
    std::vector<std::shared_ptr<GameObject>>& m_gameObjects;
    std::map<int, std::vector<std::weak_ptr<GameObject>>> layeredGameObjects;
};
