#pragma once
#include <memory>
#include "glm.hpp"
#include "Event.h"

class GameObject;

class CollisionEnterEvent : public Event
{
public:
    CollisionEnterEvent(const std::shared_ptr<GameObject>& other,glm::vec2 direction)
        : Other(other), Direction(direction)
    {
    }
    const std::shared_ptr<GameObject>& Other;
    glm::vec2 Direction;
};
