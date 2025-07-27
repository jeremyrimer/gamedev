#include "DebugHUD.h"
#include "Player.h"
#include <SDL3_image/SDL_image.h>
#include <string>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include <Constants.h>

DebugHUD::DebugHUD(SDL_Renderer* renderer) {
  // CONSTRUCTOR...
  font = TTF_OpenFont("assets/cour.ttf", 12); // number is font size
  if (!font) {
      SDL_Log("Failed to load font: %s", SDL_GetError());
      // handle error, maybe disable debug HUD rendering
  }
}

void DebugHUD::handleInput(const bool* keystates) {
    bool currentF3State = keystates[SDL_SCANCODE_F3];

    // Toggle on key down (not held)
    if (currentF3State && !lastF3State) {
        visible = !visible;
    }

    lastF3State = currentF3State;
}

void DebugHUD::update(float deltaTime, const Player& player) {
    if (visible) {
        playerBounds = player.getBounds();
    }
}

void DebugHUD::render(SDL_Renderer* renderer) {
    if (!visible) return;

    std::string playerXText = "Player X: " + std::to_string(playerBounds.x); 
    std::string playerYText = "Player Y: " + std::to_string(playerBounds.y); 
    std::string playerHText = "Player H: " + std::to_string(playerBounds.h); 
    std::string playerWText = "Player W: " + std::to_string(playerBounds.w);

    renderText(renderer, playerXText, 10, SCREEN_HEIGHT - 20);
    renderText(renderer, playerYText, 10, SCREEN_HEIGHT - 30);
    renderText(renderer, playerHText, 10, SCREEN_HEIGHT - 40);
    renderText(renderer, playerWText, 10, SCREEN_HEIGHT - 50);
}

void DebugHUD::renderText(SDL_Renderer* renderer, const std::string &text, int x, int y) {
    SDL_Color color = {255, 255, 255, 255};

    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), text.size(), color);
    if (!surface) {
        SDL_Log("TTF_RenderText_Blended error: %s", SDL_GetError());
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_Log("SDL_CreateTextureFromSurface error: %s", SDL_GetError());
        SDL_DestroySurface(surface);
        return;
    }
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

    SDL_FRect dst = { (float)x, (float)y, (float)surface->w, (float)surface->h };
    SDL_RenderTexture(renderer, texture, nullptr, &dst);

    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);
}