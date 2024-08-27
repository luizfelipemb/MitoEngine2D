#pragma once
#include <map>
#include <memory>
#include <vector>

class GameObject;

class RenderSystem
{
public:
    void SortByLayer(std::vector<std::shared_ptr<GameObject>>& gameObjects);
    void Update();
private:
    std::map<int, std::vector<std::weak_ptr<GameObject>>> layeredGameObjects;
    
    /*TODO:
    on start ---> SortByLayer
    on object add --> add by layer func
    on object destroy ---> remove by layer func
    on object change layer ---> change layer func
    */
};
