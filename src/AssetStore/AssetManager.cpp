#include "./AssetManager.h"

#include <iostream>

#include "../Logger/Logger.h"
#include <SDL_image.h>
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_sdl2.h"
#include "../imgui/imgui_impl_sdlrenderer2.h"

std::map<std::string, SDL_Texture*> AssetManager::m_textures;
std::map<std::string, TTF_Font*> AssetManager::m_fonts;

AssetManager::AssetManager() {
	Logger::Log("AssetStore constructor called!");
}

AssetManager::~AssetManager() {
	ClearAssets();
	Logger::Log("AssetStore destructor called!");

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
}

void AssetManager::ClearAssets()
{
	for (auto texture : m_textures)
	{
		SDL_DestroyTexture(texture.second);
	}
	m_textures.clear();

	for (auto font : m_fonts)
	{
		TTF_CloseFont(font.second);
	}
	m_fonts.clear();
}

bool AssetManager::LoadTexture(SDL_Renderer* renderer,std::string fileName)
{
	if (m_textures.find(fileName) != m_textures.end())
	{
		return false;
	}
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
		m_textures[fileName] = texture;
		return true;
	}

	std::cout << "Could not create texture from surface!!! ('" << fileName.c_str() << "')" << std::endl;

	return false;
}

void AssetManager::RenderImage(SDL_Renderer* renderer, std::string filename, int x, int y, int w, int h, double scale,
	std::optional<Color> color)
{
	SDL_Rect destRect;
	destRect.x = x;
	destRect.y = y;
	destRect.w = w * scale;
	destRect.h = h * scale;
	SDL_RendererFlip flip = SDL_FLIP_NONE;

	if (m_textures.find(filename) == m_textures.end())
	{
		LoadTexture(renderer, filename);
	}

	if (color.has_value())
	{
		SDL_SetTextureColorMod(m_textures[filename], color->Red, color->Green, color->Blue);
	}

	if (SDL_RenderCopyEx(renderer, m_textures[filename], nullptr, &destRect, 0, nullptr, flip) != 0)
	{
		std::cout << SDL_GetError() << std::endl;
	}

	if (color.has_value())
	{
		SDL_SetTextureColorMod(m_textures[filename], 255, 255, 255);
	}
}
void AssetManager::RenderText(SDL_Renderer* renderer, const std::string& text, const std::string& fontFile, int fontSize, int x, int y,
	double scale, bool centered, std::optional<Color> color)
{
	if (m_fonts.find(fontFile) == m_fonts.end())
	{
		// Load font
		TTF_Font* font = TTF_OpenFont(fontFile.c_str(), fontSize);
		if (!font)
		{
			std::cerr << "Error loading font: " << TTF_GetError() << std::endl;
			return;
		}
		else
		{
			m_fonts[fontFile] = font;
		}
	}

	// Render text
	SDL_Color textColor = { 0, 0, 0 }; // Default color (black)
	if (color.has_value())
	{
		textColor = SDL_Color{ color->Red, color->Green, color->Blue };
	}
	SDL_Surface* surface = TTF_RenderText_Blended(m_fonts[fontFile], text.c_str(), textColor);
	if (!surface)
	{
		std::cerr << "Error rendering text: " << TTF_GetError() << std::endl;
		TTF_CloseFont(m_fonts[fontFile]);
		return;
	}

	// Create texture from surface
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (!texture)
	{
		std::cerr << "Error creating texture from surface: " << SDL_GetError() << std::endl;
		SDL_FreeSurface(surface);
		TTF_CloseFont(m_fonts[fontFile]);
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


void AssetManager::DrawRectangle(SDL_Renderer* renderer, int x, int y, float width, float height, Color color)
{
	// Set render color
	SDL_SetRenderDrawColor(renderer, color.Red, color.Green, color.Blue, SDL_ALPHA_OPAQUE);

	// Define rectangle
	SDL_Rect rect = { x, y, static_cast<int>(width), static_cast<int>(height) };

	// Draw rectangle
	SDL_RenderFillRect(renderer, &rect);
}
