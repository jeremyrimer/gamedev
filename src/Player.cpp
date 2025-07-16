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
    velocityX = 0.0f;
    acceleration = 1600.0f;   // pixels per second^2
    maxSpeed = 200.0f;       // top movement speed
    drag = 1600.0f;          // how quickly to slow down when no input
}

void Player::handleInput(const bool* keyboardState) {
    movingLeft = keyboardState[SDL_SCANCODE_LEFT];
    movingRight = keyboardState[SDL_SCANCODE_RIGHT];
}

void Player::update(float deltaTime) {
    if (movingLeft && !movingRight) {
        velocityX -= acceleration * deltaTime;
    } else if (movingRight && !movingLeft) {
        velocityX += acceleration * deltaTime;
    } else {
        // Apply drag
        if (velocityX > 0) {
            velocityX -= drag * deltaTime;
            if (velocityX < 0) velocityX = 0;
        } else if (velocityX < 0) {
            velocityX += drag * deltaTime;
            if (velocityX > 0) velocityX = 0;
        }
    }

    // Clamp velocity
    if (velocityX > maxSpeed) velocityX = maxSpeed;
    if (velocityX < -maxSpeed) velocityX = -maxSpeed;

    // Apply movement
    position.x += velocityX * deltaTime;

    // Clamp to screen
    if (position.x < 0) {
        position.x = 0;
        velocityX = 0;
    };
    if (position.x > SCREEN_WIDTH - position.w) {
        position.x = SCREEN_WIDTH - position.w;
        velocityX = 0;
    };
}

void Player::render(SDL_Renderer* renderer) {
    SDL_RenderTexture(renderer, texture, nullptr, &position);
}

SDL_FRect Player::getBounds() const {
    return position;
}
