#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <iostream>
#include "Engine.h"
#include "Constants.h"

#include "backward.hpp"
namespace backward {
    backward::SignalHandling sh;  // This installs signal handlers
}

// Forward declarations
bool init(SDL_Window*& window, SDL_Renderer*& renderer); // method that starts up all subsystems
void handleEvents(bool& running, const bool* keyboardState, Engine* engine); // SDL events are fired throughout operation, this routes them
void shutdown(SDL_Window* window, SDL_Renderer* renderer); // game over handler

// First code executed
int main(int argc, char* argv[]) {
    std::cout << "Human Aimbot Startup" << std::endl;
    srand(static_cast<unsigned int>(time(nullptr))); // Seed RNG once!
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    if (!init(window, renderer)) return 1; 
    {
        Engine engine = Engine(renderer);
        engine.init();

        Uint64 prevCounter = SDL_GetPerformanceCounter();
        double freq = (double)SDL_GetPerformanceFrequency();
        bool running = true;

        while (running) {
            // Server Tick
            // std::cout << "TICK" << std::endl;
            const bool* keyboardState = SDL_GetKeyboardState(NULL);
            handleEvents(running, keyboardState, &engine);

            Uint64 now = SDL_GetPerformanceCounter();
            float deltaTime = static_cast<float>((now - prevCounter) / freq);
            
            if (!running) break;
            engine.update(deltaTime);
            engine.render();
            SDL_Delay(1);      // This just stops the app from pinning 100% CPU usage
            prevCounter = now;
        }
        engine.shutdown();
    }
    shutdown(window, renderer);
    std::cout << "Human Aimbot Shutdown" << std::endl;
    return 0;
}

bool init(SDL_Window*& window, SDL_Renderer*& renderer) {
    // WINDOW setup
    SDL_Surface* iconSurface = IMG_Load("assets/images/human_aimbot_profile_pic.ico");  
    if (!iconSurface) {
        SDL_Log("IMG_LoadSurface failed: %s", SDL_GetError());
        std::cerr << "[SDL3_IMAGE] Icon Initialization failed\n";
        SDL_DestroySurface(iconSurface);
        return false;
    } 
    std::cout << "Human Aimbot Icon Loaded" << std::endl; 

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMEPAD) < 0) {
        std::cerr << "SDL3 Init failed: " << SDL_GetError() << "\n";
        return false;
    }
    std::cout << "SDL Audio and Video Initialized" << std::endl;

    if (TTF_Init() < 0) {
        std::cerr << "TTF Init failed: " << SDL_GetError() << std::endl;
        return false;
    }
    std::cout << "SDL TTF Initialized" << std::endl;

    window = SDL_CreateWindow("Human Aimbot", SCREEN_WIDTH, SCREEN_HEIGHT, 
      SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED | SDL_WINDOW_OPENGL
    );
    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << "\n";
        return false;
    }
    std::cout << "Window Created" << std::endl; 
    SDL_SetWindowIcon(window, iconSurface);
    SDL_DestroySurface(iconSurface);

    // RENDERER setup
    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << "\n";
        return false;
    }
    std::cout << "Renderer Created" << std::endl; 

    SDL_SetRenderVSync(renderer, true);
    SDL_SetRenderLogicalPresentation(renderer, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    return true;
}

void handleEvents(bool& running, const bool* keyboardState, Engine* engine) {
    SDL_Event event;
    // std::cout << "Checking Events" << std::endl; 
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            std::cout << "Quit Detected" << std::endl;
            running = false;
        } else {
            engine->handleGlobalInput(event, keyboardState); // from Engine.cpp
        }
    }
}

void shutdown(SDL_Window* window, SDL_Renderer* renderer) {
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
