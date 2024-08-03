#pragma once
#include <SDL_render.h>

class RendererManager
{
public:
    ~RendererManager();
    static void Initialize();
    static void ClearFrameRender();
    static void DrawFrameRender();
    
    static int WindowWidth;
    static int WindowHeight;

    static SDL_Window* Window;
    static SDL_Renderer* Renderer;
};
