#pragma once
#include <SDL_render.h>
#include <string>

struct WindowSettings
{
    inline static int FPS;
    inline static int WindowWidth;
    inline static int WindowHeight;
    inline static std::string WindowName;
    inline static std::string IconImageLocation;
    static int GetWidth() { return WindowWidth; }
    static int GetHeight() { return WindowHeight; }
};

class RendererManager
{
public:
    ~RendererManager();
    static void Initialize();
    static void ClearFrameRender();
    static void DrawFrameRender();

    inline static SDL_Window* Window;
    inline static SDL_Renderer* Renderer;
};
