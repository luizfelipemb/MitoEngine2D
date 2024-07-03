#pragma once

#include <SDL.h>
#include "Event.h"

class KeyPressedEvent : public Event
{
public:
    SDL_Keycode Symbol;
    KeyPressedEvent(SDL_Keycode symbol) : Symbol(symbol) {}
};
