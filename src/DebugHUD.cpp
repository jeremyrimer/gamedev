#include "DebugHUD.h"
#include "Player.h"
#include <SDL3_image/SDL_image.h>
#include <string>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include <Constants.h>

DebugHUD::DebugHUD(SDL_Renderer* renderer) : renderer(renderer) {
  // CONSTRUCTOR...
  font = TTF_OpenFont("assets/fonts/jb.ttf", 12); // number is font size
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
        playerBounds = player.getPosition();
        playerAngle = player.getAngle();
        playerVelocity = player.getVelocity();
    }
}

void DebugHUD::render() {
    if (!visible) return;

    std::string playerXText = "Position X: " + std::to_string(playerBounds.x); 
    std::string playerYText = "Position Y: " + std::to_string(playerBounds.y); 
    std::string playerAngleText = "Angle: " + std::to_string(playerAngle);
    std::string playerXVelocity = "Velocity X: " + std::to_string(playerVelocity.x); 
    std::string playerYVelocity = "Velocity Y: " + std::to_string(playerVelocity.y);

    renderText(playerXText, 10, SCREEN_HEIGHT - 20);
    renderText(playerYText, 10, SCREEN_HEIGHT - 35);
    renderText(playerAngleText, 10, SCREEN_HEIGHT - 50);
    renderText(playerXVelocity, 10, SCREEN_HEIGHT - 65);
    renderText(playerYVelocity, 10, SCREEN_HEIGHT - 80);
}

void DebugHUD::renderText(const std::string &text, int x, int y) {
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

    SDL_FRect dst = { static_cast<float>(x), static_cast<float>(y), static_cast<float>(surface->w), static_cast<float>(surface->h) };
    SDL_RenderTexture(renderer, texture, nullptr, &dst);

    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);
}