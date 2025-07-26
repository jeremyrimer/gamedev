#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <iostream>
#include "Engine.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Forward declarations
bool init(SDL_Window*& window, SDL_Renderer*& renderer);
void handleEvents(bool& running, const bool* keyboardState);
void shutdown(SDL_Window* window, SDL_Renderer* renderer);

int main(int argc, char* argv[]) {
    std::cout << "Starting game...\n" << std::endl;

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    if (!init(window, renderer)) {
        return 1;
    } 

    Uint64 prevCounter = SDL_GetPerformanceCounter();
    double freq = (double)SDL_GetPerformanceFrequency();
    bool running = true;

    while (running) {
        // Server Tick
        Uint64 now = SDL_GetPerformanceCounter();
        float deltaTime = (float)((now - prevCounter) / freq);

        const bool* keyboardState = SDL_GetKeyboardState(NULL);

        handleEvents(running, keyboardState);
        update(deltaTime);  // from Engine.cpp
        render(renderer);   // from Engine.cpp
        SDL_Delay(1);      // 16 = ~60 FPS, but with VSYNC, don't need it strict
        prevCounter = now;
    }

    shutdown(window, renderer);
    return 0;
}

bool init(SDL_Window*& window, SDL_Renderer*& renderer) {
    // WINDOW setup
    SDL_Surface* iconSurface = IMG_Load("assets/human_aimbot_profile_pic.png");  
    if (!iconSurface) {
        SDL_Log("IMG_LoadSurface failed: %s", SDL_GetError());
        std::cerr << "[SDL3_IMAGE] Icon Initialization failed\n";
        SDL_DestroySurface(iconSurface);
        return false;
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL3 Init failed: " << SDL_GetError() << "\n";
        return false;
    }

    window = SDL_CreateWindow("Human Aimbot", SCREEN_WIDTH, SCREEN_HEIGHT, 
      SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED
    );
    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << "\n";
        return false;
    }
    SDL_SetWindowIcon(window, iconSurface);
    SDL_DestroySurface(iconSurface);

    // RENDERER setup
    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << "\n";
        return false;
    }
    SDL_SetRenderVSync(renderer, true);
    SDL_SetRenderLogicalPresentation(renderer, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    initPlayer(renderer);
    return true;
}

void handleEvents(bool& running, const bool* keyboardState) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            running = false;
        } else {
            handleGlobalInput(event, keyboardState);
        }
    }
}

void shutdown(SDL_Window* window, SDL_Renderer* renderer) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
