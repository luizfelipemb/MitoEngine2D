#pragma once
#include <SDL_render.h>

class RendererManager
{
public:
    ~RendererManager();
    static void Initialize();
    static void ClearFrameRender();
    static void DrawFrameRender();
    
    static int m_windowWidth;
    static int m_windowHeight;
    static int m_mapWidth;
    static int m_mapHeight;

    static SDL_Window* m_window;
    static SDL_Renderer* Renderer;
};
