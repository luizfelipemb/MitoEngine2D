#pragma once
#include <SDL_keycode.h>

class KeyReleasedEvent : public Event
{
    public:
        SDL_Keycode Symbol;
        KeyReleasedEvent(SDL_Keycode symbol) : Symbol(symbol) {}
};

