#pragma once
#include <SDL_render.h>
#include <string>

class WindowSettings
{
public:
    static int WindowWidth;
    static int WindowHeight;
    static std::string WindowName;
    static std::string IconImageLocation;
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

    static bool ShowDebugInfo;
    static SDL_Window* Window;
    static SDL_Renderer* Renderer;
};
