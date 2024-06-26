#pragma once

#include <map>
#include <optional>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>

struct Color
{
    std::uint8_t red;
    std::uint8_t green;
    std::uint8_t blue;
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
        static std::map<std::string, SDL_Texture*> textures;
        static std::map<std::string, TTF_Font*> fonts;

        static int windowWidth;
        static int windowHeight;
        static int mapWidth;
        static int mapHeight;

        static SDL_Window* window;
        static SDL_Renderer* renderer;
};
