#pragma once
#include <SDL_keycode.h>

class KeyReleasedEvent : public Event
{
    public:
        SDL_Keycode symbol;
        KeyReleasedEvent(SDL_Keycode symbol) : symbol(symbol) {}
};

