// Engine.cpp
#include "Engine.h"
#include <SDL3/SDL.h>
#include <iostream>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

static int pixelX = SCREEN_WIDTH / 2;
static int pixelY = SCREEN_HEIGHT / 2;

static const int MOVE_SPEED = 5; // pixels per key press

void handleInput(const SDL_Event& event) {
    if (event.type == SDL_EVENT_KEY_DOWN) {
        switch (event.key.key) {
            case SDLK_W:
            case SDLK_UP:
                std::cout << "KEY EVENT: UP" << std::endl;
                pixelY -= MOVE_SPEED;
                if (pixelY < 0) pixelY = 0;
                break;
            case SDLK_S:
            case SDLK_DOWN:
                std::cout << "KEY EVENT: DOWN" << std::endl;
                pixelY += MOVE_SPEED;
                if (pixelY > SCREEN_HEIGHT - 1) pixelY = SCREEN_HEIGHT - 1;
                break;
            case SDLK_A:
            case SDLK_LEFT:
                std::cout << "KEY EVENT: LEFT" << std::endl;
                pixelX -= MOVE_SPEED;
                if (pixelX < 0) pixelX = 0;
                break;
            case SDLK_D:
            case SDLK_RIGHT:
                std::cout << "KEY EVENT: RIGHT" << std::endl;
                pixelX += MOVE_SPEED;
                if (pixelX > SCREEN_WIDTH - 1) pixelX = SCREEN_WIDTH - 1;
                break;
        }
    }
}

void update() {
    
}

void render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255); // Clear color
    SDL_RenderClear(renderer);

    // Draw a white pixel at the center
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderPoint(renderer, pixelX, pixelY);

    SDL_RenderPresent(renderer);
}