#ifndef GAME_H
#define GAME_H

#include <memory>
#include <SDL.h>

const int FPS = 60;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Game {
    private:
        bool isRunning;
        int millisecsPreviousFrame = 0;
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Rect camera;

    public:
        Game();
        ~Game();
        void Initialize();
        void Run();
        void ProcessInput();
        void Update();
        void Render();
        void Destroy();

        static int windowWidth;
        static int windowHeight;
        static int mapWidth;
        static int mapHeight;
};

#endif
