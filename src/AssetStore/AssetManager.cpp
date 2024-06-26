#include "./AssetManager.h"

#include <iostream>

#include "../Logger/Logger.h"
#include <SDL_image.h>
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_sdl2.h"
#include "../imgui/imgui_impl_sdlrenderer2.h"

std::map<std::string, SDL_Texture*> AssetManager::textures;
std::map<std::string, TTF_Font*> AssetManager::fonts;

int AssetManager::windowWidth;
int AssetManager::windowHeight;
int AssetManager::mapWidth;
int AssetManager::mapHeight;
SDL_Window* AssetManager::window;
SDL_Renderer* AssetManager::renderer;

AssetManager::AssetManager() {
    Logger::Log("AssetStore constructor called!");
}

AssetManager::~AssetManager() {
    ClearAssets();
    Logger::Log("AssetStore destructor called!");

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void AssetManager::Initialize()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        Logger::Err("Error initializing SDL.");
        return;
    }
    if (TTF_Init() != 0) {
        Logger::Err("Error initializing SDL TTF.");
        return;
    }
    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);
    windowWidth = displayMode.w / 2;
    windowHeight = displayMode.h / 2;
    mapWidth = windowWidth;
    mapHeight = windowHeight;
    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowWidth,
        windowHeight,
        NULL
    );
    if (!window) {
        Logger::Err("Error creating SDL window.");
        return;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        Logger::Err("Error creating SDL renderer.");
        return;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);
}

void AssetManager::ClearFrameRender()
{
	ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    SDL_RenderClear(renderer);

}

void AssetManager::DrawFrameRender()
{
    
    //RenderImage("assets/images/radar.png", 0, 0, 100, 100, 1);
    RenderText("TESTEEEE", "assets/fonts/arial.ttf", 100, 0, 0);

    ImGui::ShowDemoWindow();
    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());

    SDL_RenderPresent(renderer);
}

void AssetManager::ClearAssets() {
    for (auto texture: textures) {
        SDL_DestroyTexture(texture.second);
    }
    textures.clear();

    for (auto font: fonts) {
        TTF_CloseFont(font.second);
    }
    fonts.clear();
}

bool AssetManager::LoadTexture(std::string fileName)
{
    if (textures.find(fileName) != textures.end())
        return false;

    SDL_Surface* tempSurf = IMG_Load(fileName.c_str());

    if (tempSurf == nullptr)
    {
        std::cout << "could not load image named: '" << fileName.c_str() << "'!!!" << std::endl;
        return false;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tempSurf);

    SDL_FreeSurface(tempSurf);

    if (texture != nullptr)
    {
        std::cout << "Texture loaded successfully... ('" << fileName.c_str() << "')" << std::endl;
        textures[fileName] = texture;
        return true;
    }

    std::cout << "Could not create texture from surface!!! ('" << fileName.c_str() << "')" << std::endl;

    return false;
}

void AssetManager::RenderImage(std::string filename, int x, int y, int w, int h, double scale,
    std::optional<Color> color)
{
    SDL_Rect destRect;
    destRect.x = x;
    destRect.y = y;
    destRect.w = w * scale;
    destRect.h = h * scale;
    SDL_RendererFlip flip = SDL_FLIP_NONE;

    if (textures.find(filename) == textures.end())
        LoadTexture(filename);

    if (color.has_value())
    {
        SDL_SetTextureColorMod(textures[filename], color->red, color->green, color->blue);
    }

    if (SDL_RenderCopyEx(renderer, textures[filename], nullptr, &destRect, 0, nullptr, flip) != 0)
        std::cout << SDL_GetError() << std::endl;

    if (color.has_value())
    {
        SDL_SetTextureColorMod(textures[filename], 255, 255, 255);
    }
}
void AssetManager::RenderText(const std::string& text, const std::string& fontFile, int fontSize, int x, int y,
    double scale, bool centered, std::optional<Color> color)
{
    if (fonts.find(fontFile) == fonts.end())
    {
        // Load font
        TTF_Font* font = TTF_OpenFont(fontFile.c_str(), fontSize);
        if (!font)
        {
            std::cerr << "Error loading font: " << TTF_GetError() << std::endl;
            return;
        }else
        {
            fonts[fontFile] = font;
        }
    }

    // Render text
    SDL_Color textColor = { 0, 0, 0 }; // Default color (black)
    if (color.has_value())
    {
        textColor = SDL_Color{ color->red, color->green, color->blue };
    }
    SDL_Surface* surface = TTF_RenderText_Blended(fonts[fontFile], text.c_str(), textColor);
    if (!surface)
    {
        std::cerr << "Error rendering text: " << TTF_GetError() << std::endl;
        TTF_CloseFont(fonts[fontFile]);
        return;
    }

    // Create texture from surface
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture)
    {
        std::cerr << "Error creating texture from surface: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(surface);
        TTF_CloseFont(fonts[fontFile]);
        return;
    }

    // Calculate text dimensions
    int textWidth = static_cast<int>(surface->w * scale);
    int textHeight = static_cast<int>(surface->h * scale);

    // Adjust position for centered text
    if (centered)
    {
        x -= textWidth / 2;
        y -= textHeight / 2;
    }

    // Destination rectangle
    SDL_Rect destRect = { x, y, textWidth, textHeight };

    // Render text
    if (SDL_RenderCopy(renderer, texture, nullptr, &destRect) != 0)
    {
        std::cerr << "Error rendering text: " << SDL_GetError() << std::endl;
    }

    // Clean up
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}


void AssetManager::DrawRectangle(int x, int y, float width, float height, Color color)
{
    // Set render color
    SDL_SetRenderDrawColor(renderer, color.red, color.green, color.blue, SDL_ALPHA_OPAQUE);

    // Define rectangle
    SDL_Rect rect = { x, y, width, height };

    // Draw rectangle
    SDL_RenderFillRect(renderer, &rect);
}
