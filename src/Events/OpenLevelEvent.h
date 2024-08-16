#pragma once

#include <string>

#include "Event.h"

class OpenLevelEvent : public Event
{
public:
    OpenLevelEvent(std::string levelName) : LevelName(levelName)
    {}
    std::string LevelName;
};
