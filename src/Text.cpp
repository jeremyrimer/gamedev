#include <SDL3/SDL.h>
#include <string>
#include "Text.h"
#include <stdexcept>

Text::Text(SDL_Renderer* renderer, const char* fontFilePath, int fontSize) :
  renderer(renderer), fontFilePath(fontFilePath), fontSize(fontSize) {
    init();
}

void Text::init() {
    font = TTF_OpenFont(fontFilePath, fontSize);
    if (!font) {
        SDL_Log("Failed to load font: %s", SDL_GetError());
        throw std::runtime_error("Failed to load Font!");
    }
}

void Text::display(std::string text, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a) const {
    SDL_Color color = {r,g,b,a};

    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), text.size(), color);
    if (!surface) {
        SDL_Log("TTF_RenderText_Solid error: %s", SDL_GetError());
        throw std::runtime_error("Failed to create surface!");;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_Log("SDL_CreateTextureFromSurface error: %s", SDL_GetError());
        SDL_DestroySurface(surface);
        throw std::runtime_error("Failed to create Font Texture");;
    }
    
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

    SDL_FRect dst = { static_cast<float>(x), static_cast<float>(y), static_cast<float>(surface->w), static_cast<float>(surface->h) };
    SDL_RenderTexture(renderer, texture, nullptr, &dst);

    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);
}

Text::~Text() {}