#pragma once
#include <SDL_render.h>
#include <string>

class RendererManager
{
public:
    ~RendererManager();
    static void Initialize();
    static void ClearFrameRender();
    static void DrawFrameRender();
    
    static int WindowWidth;
    static int WindowHeight;
    static std::string WindowName;
    static std::string IconImageLocation;

    static SDL_Window* Window;
    static SDL_Renderer* Renderer;
};
