#pragma once
#include <memory>
#include <unordered_map>
#include <vec2.hpp>

class GameObject;

class CollisionSystem
{
public:
    bool CheckAABBCollision(double aX, double aY, double aW, double aH, double bX,
                                                double bY, double bW, double bH);
    glm::vec2 GetCollisionDirection(double aX, double aY, double aW, double aH, 
                                    double bX, double bY, double bW, double bH);
    void CalculateCollisions(std::vector<std::shared_ptr<GameObject>>& m_gameObjects);

private:
    std::unordered_multimap<int,int> m_objectsColliding;
};
