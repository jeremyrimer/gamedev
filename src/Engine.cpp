#include "Constants.h"
#include "Engine.h"
#include "Player.h"
#include <SDL3/SDL.h>
#include <iostream>
#include <unordered_map>

static Player* player;

static std::unordered_map<SDL_Keycode, bool> keyStates;

void initPlayer(SDL_Renderer* renderer) {
    player = new Player(renderer);
}

void handleGlobalInput(const SDL_Event& event, const bool* keyboardState) {
    if (event.type == SDL_EVENT_KEY_DOWN && event.key.repeat == 0) {
        if (event.key.key == SDLK_ESCAPE) {
            std::cout << "ESCAPE PRESSED; QUITTING..." << std::endl;
            SDL_Event quitEvent;
            quitEvent.type = SDL_EVENT_QUIT;
            SDL_PushEvent(&quitEvent);
        } else {
            std::cout << SDL_GetScancodeName(event.key.scancode) << " PRESSED" << std::endl;
            keyStates[event.key.key] = true;
        }
    } else if (event.type == SDL_EVENT_KEY_UP && event.key.repeat == 0) {
        std::cout << SDL_GetScancodeName(event.key.scancode) << " RELEASED" << std::endl;
        keyStates[event.key.key] = false;
    }
    player->handleInput(keyboardState);
}

void update(float deltaTime) {
    // Safety clamp to avoid giant simulation steps after pauses.
    if (deltaTime > 0.1f) deltaTime = 0.1f;
    player->update(deltaTime);
}

void render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255); // Clear color
    SDL_RenderClear(renderer);

    player->render(renderer);
    SDL_RenderPresent(renderer);
}