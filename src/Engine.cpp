#include "Engine.h"
#include "Constants.h"
#include "Asteroid.h"
#include "DebugHUD.h"
#include "Text.h"
#include <iostream>


// Constructor
Engine::Engine(SDL_Renderer* renderer)
    : renderer(renderer),
      player(renderer),
      debugHUD(renderer, &player),
      gameOverFont(Text(renderer, "assets/fonts/jb.ttf", 94)) {}

// Destructor
Engine::~Engine() {
    Explosion::UnloadTexture();
}

void Engine::init() {
    asteroids.clear();

    // 10 is number of asteroids... we'll have to make that round based
    for (int i = 0; i < 10; ++i) {
        asteroids.emplace_back(renderer);
    }

    Explosion::LoadTexture(renderer);

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
    for (auto& explosion : explosions) {
        explosion->update(deltaTime);
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
    for (const auto& explosion : explosions) {
        explosion->draw();
    }

    debugHUD.render();

    if (gameState == GameState::GAMEOVER) {
        gameOverFont.display("GAME OVER", SCREEN_WIDTH / 4.0f, SCREEN_HEIGHT / 3.0f, 255, 0, 0, 255);
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
                // std::cout << "COLLISION!" << std::endl;
                handlePlayerDeath();
                break;
        }
    }
}

void Engine::handlePlayerDeath() {
    if (player.isAlive()) {
        player.setAlive(false);
        // EXPLODE
        explosions.emplace_back(
            std::make_unique<Explosion>(
                renderer,
                player.getPosition(),
                player.getSize().x*3,
                0.1f,
                "assets/sound/spaceship-explosion.wav"
            )
        );
    }
    
    gameState = GameState::GAMEOVER;
}
