#pragma once

#include <memory>
#include <SDL.h>
#include "../GameObjects/GameObject.h"

class EventBus;
const int FPS = 60;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Game {
    private:
        bool isRunning;
        int millisecsPreviousFrame = 0;
        SDL_Rect camera;
        std::unique_ptr<Registry> m_registry;
    public:
        Game();
        ~Game();
        void Initialize();
        void Run();
        void ProcessInput();
        void Update();
        void Destroy();
};
