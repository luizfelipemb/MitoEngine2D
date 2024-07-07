#pragma once
#include <memory>

#include "Event.h"

class GameObject;

class CollisionEnterEvent : public Event
{
public:
    CollisionEnterEvent(const std::unique_ptr<GameObject>& other)
        : Other(other)
    {
    }
    const std::unique_ptr<GameObject>& Other;
};
