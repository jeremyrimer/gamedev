// Engine.cpp
#include "Engine.h"
#include <SDL3/SDL.h>
#include <iostream>
#include <unordered_map>

// Screen Settings
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Player
static int pixelX = SCREEN_WIDTH / 2;
static int pixelY = SCREEN_HEIGHT / 2;
static int MOVE_SPEED = 2;

// Keyboard Button States
static std::unordered_map<SDL_Keycode, bool> keyStates;

void handleInput(const SDL_Event& event) {
    if (event.type == SDL_EVENT_KEY_DOWN) {
        keyStates[event.key.key] = true;
    } else if (event.type == SDL_EVENT_KEY_UP) {
        keyStates[event.key.key] = false;
    }
}

void update() {
    if (keyStates[SDLK_UP]) {
        pixelY = std::max(pixelY - MOVE_SPEED, 0);
    }
    if (keyStates[SDLK_DOWN]) {
        pixelY = std::min(pixelY + MOVE_SPEED, SCREEN_HEIGHT - 1);
    }
    if (keyStates[SDLK_LEFT]) {
        pixelX = std::max(pixelX - MOVE_SPEED, 0);
    }
    if (keyStates[SDLK_RIGHT]) {
        pixelX = std::min(pixelX + MOVE_SPEED, SCREEN_WIDTH - 1);
    }
}

void render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255); // Clear color
    SDL_RenderClear(renderer);

    // Draw a white pixel at the center
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderPoint(renderer, pixelX, pixelY);

    SDL_RenderPresent(renderer);
}