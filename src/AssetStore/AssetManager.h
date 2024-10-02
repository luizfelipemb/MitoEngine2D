#pragma once

#include <map>
#include <optional>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include <cstdint>

struct Color
{
    Color() : Red(0), Green(0), Blue(0) {}
    Color(std::uint8_t r, std::uint8_t g, std::uint8_t b) : Red(r), Green(g), Blue(b) {}
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
        static void ClearAssets();

        static bool LoadTexture(SDL_Renderer* renderer, std::string fileName);
        static void RenderImage(SDL_Renderer* renderer, std::string filename, int x, int y, int w, int h, double scale,
            std::optional<Color> color,std::optional<int> sourceX = std::nullopt, std::optional<int> sourceY = std::nullopt);
        static int GetWidthOfSprite(SDL_Renderer* renderer, std::string filename);
        static int GetHeightOfSprite(SDL_Renderer* renderer, std::string filename);
        static void RenderText(SDL_Renderer* renderer, const std::string& text, const std::string& fontFile, int fontSize, int x, int y, double scale = 1,
            bool centered = false, std::optional<Color> color = std::nullopt);
        static void DrawRectangle(SDL_Renderer* renderer, int x, int y, float width, float height, Color color);
        static void DrawBorderRectangle(SDL_Renderer* renderer, int x, int y, float width, float height, Color color);

private:
        static std::map<std::string, SDL_Texture*> m_textures;
        static std::map<std::string, TTF_Font*> m_fonts;
};
