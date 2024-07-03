#pragma once

#include <map>
#include <optional>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>

struct Color
{
    std::uint8_t Red;
    std::uint8_t Green;
    std::uint8_t Blue;
};
class AssetManager
{
    public:
        AssetManager();
        ~AssetManager();

        static void Initialize();
        static void ClearFrameRender();
        static void DrawFrameRender();
        static void ClearAssets();

        static bool LoadTexture(std::string fileName);
        static void RenderImage(std::string filename, int x, int y, int w, int h, double scale,
            std::optional<Color> color = std::nullopt);
        static void RenderText(const std::string& text, const std::string& fontFile, int fontSize, int x, int y, double scale = 1,
            bool centered = false, std::optional<Color> color = std::nullopt);
        static void DrawRectangle(int x, int y, float width, float height, Color color);

private:
        static std::map<std::string, SDL_Texture*> m_textures;
        static std::map<std::string, TTF_Font*> m_fonts;

        static int m_windowWidth;
        static int m_windowHeight;
        static int m_mapWidth;
        static int m_mapHeight;

        static SDL_Window* m_window;
        static SDL_Renderer* m_renderer;
};
