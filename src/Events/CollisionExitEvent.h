#pragma once
#include <memory>
#include "Event.h"
class GameObject;

class CollisionExitEvent : public Event
{
public:
    CollisionExitEvent(const std::shared_ptr<GameObject>& other)
        : Other(other)
    {
    }
    const std::shared_ptr<GameObject>& Other;
};
