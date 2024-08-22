#pragma once
#include <memory>
#include <vector>

class GameObject;

class RenderSystem
{
public:
    void Update(std::vector<std::unique_ptr<GameObject>>& m_gameObjects);

    /*TODO: system rendering

    layer int ---> vector ref gameobjects
    start ---> organize by layer
    on object add --> add by layer
    on object destroy ---> remove by layer
    */
};
