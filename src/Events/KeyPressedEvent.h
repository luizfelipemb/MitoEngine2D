#pragma once

#include <SDL.h>
#include "Event.h"

class KeyPressedEvent : public Event
{
public:
    SDL_Keycode symbol;
    KeyPressedEvent(SDL_Keycode symbol) : symbol(symbol) {}
};
