// Player.cpp
#include "Constants.h"
#include "Player.h"
#include "Engine.h"
#include <SDL3_image/SDL_image.h>
#include <unordered_map>
#include <iostream>

int startX;
int startY;

Player::Player(SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load("assets/player-ship.png");
    if (!surface) {
        SDL_Log("Failed to load player texture: %s", SDL_GetError());
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_Log("Failed to create player texture: %s", SDL_GetError());
    }
    SDL_DestroySurface(surface);
    float textureWidth;
    startX = SCREEN_WIDTH / 2;
    startY = SCREEN_HEIGHT - 64;

    position = { static_cast<float>(startX), static_cast<float>(startY), 32, 32 };
    speed = 200.0f;
}

void Player::handleInput(const bool* keystates) {
    if (keystates[SDL_SCANCODE_LEFT])  position.x -= speed / 60.0f;
    if (keystates[SDL_SCANCODE_RIGHT]) position.x += speed / 60.0f;

    // Clamp to screen
    if (position.x < 0) position.x = 0;
    if (position.x > SCREEN_WIDTH - position.w) position.x = SCREEN_WIDTH - position.w;
}

void Player::update(float deltaTime) {
    // Currently nothing to update here
}

void Player::render(SDL_Renderer* renderer) {
    SDL_RenderTexture(renderer, texture, nullptr, &position);
}

SDL_FRect Player::getBounds() const {
    return position;
}
