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

    for (int i = 0; i < 10; ++i) {
        asteroids.emplace_back(renderer, player.getPosition());
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
    // std::cout << "Player Updating" << std::endl;
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
    // std::cout << "After Player Render" << std::endl;

    for (const auto& asteroid : asteroids) {
        asteroid.render();
    }
    // std::cout << "After asteroids render" << std::endl;
    for (const auto& explosion : explosions) {
        explosion->draw();
    }
    // std::cout << "After explosions render" << std::endl;
    for (auto it = explosions.begin(); it != explosions.end(); ) {
        if ((*it)->isFinished()) {
            it = explosions.erase(it);  // erase returns the next iterator
        } else {
            ++it;
        }
    }
    // std::cout << "After explosions removal" << std::endl;

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
    if (!player.isAlive()) return;

    Vector2 playerTopLeft = player.getPosition() - player.getSize() * 0.5f;

    std::vector<Asteroid> newPieces;
    std::vector<size_t> toRemove;

    for (size_t i = 0; i < asteroids.size(); ++i) {
        if (circleRectangleCollision(
            asteroids[i].getPosition(), asteroids[i].getRadius(),
            playerTopLeft, player.getSize().x, player.getSize().y)) {
            handlePlayerDeath();

            // Add split pieces to newPieces
            newPieces = asteroids[i].split(renderer);
            toRemove.push_back(i);

            break; // stop after one collision
        }
    }

    // Remove asteroids in reverse order so indices stay valid
    for (auto it = toRemove.rbegin(); it != toRemove.rend(); ++it) {
        // std::cout << "=== Erasing blown asteroid" << std::endl;
        asteroids.erase(asteroids.begin() + *it);
    }

    // Append new pieces
    if (!newPieces.empty()) {
        // std::cout << "=== Adding " << newPieces.size() << " asteroids" << std::endl;
        asteroids.insert(asteroids.end(), newPieces.begin(), newPieces.end());
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
                "assets/sound/spaceship-explosion.wav",
                0.3f
            )
        );
    }
    
    gameState = GameState::GAMEOVER;
}
