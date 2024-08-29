#pragma once
#include <map>
#include <memory>
#include <vector>

class GameObjectCreatedEvent;
class GameObject;

class RenderSystem
{
public:
    RenderSystem(const std::vector<std::shared_ptr<GameObject>>& gameObjects);
    void SortByLayer();
    void Update();
    void OnObjectCreation(GameObjectCreatedEvent& gameObjectCreatedEvent);
private:
    const std::vector<std::shared_ptr<GameObject>>& m_gameObjects;
    std::map<int, std::vector<std::weak_ptr<GameObject>>> layeredGameObjects;
    
    /*TODO:
    on start ---> SortByLayer
    on object add --> add by layer func
    on object destroy ---> remove by layer func
    on object change layer ---> change layer func
    */
};
