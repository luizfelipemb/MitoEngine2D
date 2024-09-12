#pragma once
#include "Event.h"
#include "../Logger/Logger.h"

class MouseButtonPressedEvent: public Event
{
public:
    MouseButtonPressedEvent(int button, glm::vec2 pos): Button(button), Position(pos)
    {
        Logger::Log("Clicked button id:" + std::to_string(button) +
            " x:"+std::to_string(Position.x)+" y:"+std::to_string(Position.y));
    }
    int Button;
    glm::vec2 Position;
};
