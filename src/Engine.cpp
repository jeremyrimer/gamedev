#include "Engine.h"
#include "Constants.h"
#include "Asteroid.h"
#include "DebugHUD.h"
#include <iostream>

// Constructor
Engine::Engine(SDL_Renderer* renderer)
    : renderer(renderer),
      player(renderer),
      debugHUD(renderer, &player) {}

// Destructor
Engine::~Engine() {
    // Cleanup if needed
}

void Engine::init() {
    asteroids.clear();

    // 10 is number of asteroids... we'll have to make that round based
    for (int i = 0; i < 10; ++i) {
        asteroids.emplace_back(renderer);
    }

    gameOverFont = TTF_OpenFont("assets/fonts/jb.ttf", 94); // number is font size
    if (!gameOverFont) {
        SDL_Log("Failed to load Engine's gameOverFont: %s", SDL_GetError());
        throw std::runtime_error("Failed to load Engine's gameOverFont Font!");
    }
    std::cout << "Engine's gameOverFont Loaded" << std::endl; 

    gameState = GameState::PLAYING;
}

void Engine::handleGlobalInput(const SDL_Event& event, const bool* keyboardState) {
    if (event.type == SDL_EVENT_KEY_DOWN | SDL_EVENT_KEY_UP) {
        if (event.key.key == SDLK_ESCAPE) {
            std::cout << "ESCAPE PRESSED; QUITTING..." << std::endl;
            SDL_Event quitEvent;
            quitEvent.type = SDL_EVENT_QUIT;
            SDL_PushEvent(&quitEvent);
        }
        else if (gameState == GameState::PLAYING) {
            player.handleInput(keyboardState);
        }
        debugHUD.handleInput(keyboardState);
    }
}

void Engine::update(float deltaTime) {
    // std::cout << "Engine Updating" << std::endl;
    player.update(deltaTime);
    collisionCheck();
    for (auto& asteroid : asteroids) {
        asteroid.update(deltaTime);
    }
    debugHUD.update(deltaTime);
}

void Engine::render() {
    // std::cout << "Engine Rendering" << std::endl;
    // background
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255); // Clear color
    SDL_RenderClear(renderer);

    player.render();

    for (const auto& asteroid : asteroids) {
        asteroid.render();
    }

    debugHUD.render();

    if (gameState == GameState::GAMEOVER) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_FRect rect = { SCREEN_WIDTH / 4.0f, SCREEN_HEIGHT / 3.0f, SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 6.0f };
        SDL_RenderFillRect(renderer, &rect);
        
        std::string gameOver = "GAME OVER";

        SDL_Color color = {0};

        SDL_Surface* surface = TTF_RenderText_Blended(gameOverFont, gameOver.c_str(), gameOver.size(), color);
        
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

        SDL_FRect dst = { SCREEN_WIDTH / 4.0f, SCREEN_HEIGHT / 3.0f, static_cast<float>(surface->w), static_cast<float>(surface->h) };
        SDL_RenderTexture(renderer, texture, nullptr, &dst);

        SDL_DestroyTexture(texture);
        SDL_DestroySurface(surface);
    }

    SDL_RenderPresent(renderer);
}

bool Engine::circleRectangleCollision(const Vector2& circleCenter, float circleRadius,
                                      const Vector2& rectTopLeft, float rectWidth, float rectHeight) {
    float closestX = SDL_clamp(circleCenter.x, rectTopLeft.x, rectTopLeft.x + rectWidth);
    float closestY = SDL_clamp(circleCenter.y, rectTopLeft.y, rectTopLeft.y + rectHeight);

    float dx = circleCenter.x - closestX;
    float dy = circleCenter.y - closestY;

    return (dx * dx + dy * dy) < (circleRadius * circleRadius);
}

void Engine::collisionCheck() {
    // std::cout << "CHECKING COLLISIONS" << std::endl;
    Vector2 playerTopLeft = player.getPosition() - player.getSize() * 0.5f;

    for (const auto& asteroid : asteroids) {
        if (circleRectangleCollision(
            asteroid.getPosition(), asteroid.getRadius(),
            playerTopLeft, player.getSize().x, player.getSize().y)) {
                std::cout << "COLLISION!" << std::endl;
                handlePlayerDeath();
                break;
        }
    }
}

void Engine::handlePlayerDeath() {
    player.setAlive(false);
    // TODO: spawn explosion, play sound, etc.
    gameState = GameState::GAMEOVER;
}
