#include "Engine.h"
#include "Constants.h"
#include "Asteroid.h"
#include "DebugHUD.h"
#include "Text.h"
#include <iostream>
#include "Audio.h"
#include <SDL3_image/SDL_image.h>


// Constructor
Engine::Engine(SDL_Renderer* renderer)
    : renderer(renderer),
      player(renderer),
      debugHUD(renderer, &player),
      gameOverFont(Text(renderer, "assets/fonts/jb.ttf", 94)),
      loadingFont(Text(renderer, "assets/fonts/jb.ttf", 22)),
      titleFont(Text(renderer, "assets/fonts/jb.ttf", 130)),
      instructionsFont(Text(renderer, "assets/fonts/jb.ttf", 16)),
      scoreFont(Text(renderer, "assets/fonts/jb.ttf", 16)),
      lives(PLAYER_STARTING_LIVES),
      respawnTimer(0.0f),
      round(1),
      bulletSound(Audio("assets/sound/blaster.wav"))  {
        bulletSound.setVolume(0.2f);

        SDL_Surface* avatarSurface = IMG_Load("assets/images/human_aimbot_avatar.png");
        avatarTexture = SDL_CreateTextureFromSurface(renderer, avatarSurface);
        if (!avatarTexture) {
            throw std::runtime_error("Failed to load Human Aimbot Avatar image!");
        }
        SDL_DestroySurface(avatarSurface);
      }

// Destructor
Engine::~Engine() {
    Explosion::UnloadTexture();
    SDL_DestroyTexture(avatarTexture);
}

void Engine::init() {
    Explosion::LoadTexture(renderer);
    gameState = GameState::LOADING;
}

void Engine::initGame() {
    score = 0;
    round = 1;
    lives = PLAYER_STARTING_LIVES;
    respawnTimer = 0.0f;
    player.respawn({SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f}, false);
    gameState = GameState::PLAYING;
    asteroids.clear();
    spawnAsteroidsForRound();
}

void Engine::handleGlobalInput(const SDL_Event& event, const bool* keyboardState) {
    if (event.type == SDL_EVENT_KEY_DOWN | SDL_EVENT_KEY_UP) {
        if (event.key.key == SDLK_ESCAPE) {
            std::cout << "ESCAPE PRESSED; QUITTING..." << std::endl;
            SDL_Event quitEvent;
            quitEvent.type = SDL_EVENT_QUIT;
            SDL_PushEvent(&quitEvent);
        }
        else if (gameState == GameState::LOADING) {
                if (event.key.key == SDLK_SPACE) {  
                initGame();
            }
        }
        else if (gameState == GameState::GAMEOVER) {
                if (event.key.key == SDLK_SPACE) {  
                initGame();
            }
        }
        else if (gameState == GameState::PLAYING) {
            if (event.type == SDL_EVENT_KEY_DOWN && 
                event.key.key == SDLK_SPACE && 
                bullets.size() < 3 &&
                player.isAlive()
            ) {
                firing = true;
            }
            player.handleInput(keyboardState);
        }
        debugHUD.handleInput(keyboardState);
    }
}

void Engine::update(float deltaTime) {
    // std::cout << "Player Updating" << std::endl;
    if (gameState == GameState::PLAYING) {
        player.update(deltaTime);
        if (firing && bullets.size() < 3) {
            fireBullet();
        }
        if (!player.isAlive()) {
            // waiting to respawn
            if (lives >= 0) { 
                respawnTimer -= deltaTime;
                if (respawnTimer <= 0.0f) {
                    player.respawn({SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f}, true);
                }
            }
        } else {
            collisionCheck();
        }
    }

    for (auto it = bullets.begin(); it != bullets.end();) {
        it->update(deltaTime);
        if (!it->isAlive()) {
            it = bullets.erase(it);
        } else {
            ++it;
        }
    }

    for (auto& asteroid : asteroids) {
        asteroid.update(deltaTime);
    }
    for (auto& explosion : explosions) {
        explosion->update(deltaTime);
    }
    for (auto bulletIt = bullets.begin(); bulletIt != bullets.end(); ) {
        bool bulletDestroyed = false;

        for (size_t i = 0; i < asteroids.size(); ++i) {
            if (circleCircleCollision(bulletIt->getPosition(), bulletIt->getRadius(),
                                    asteroids[i].getPosition(), asteroids[i].getRadius())) {

                // Spawn explosion
                explosions.emplace_back(
                    std::make_unique<Explosion>(
                        renderer,
                        asteroids[i].getPosition(),
                        asteroids[i].getRadius() * 2.0f,
                        0.1f,
                        "assets/sound/spaceship-explosion.wav",
                        0.1f
                    )
                );

                switch (asteroids[i].getSize()) {
                    case AsteroidSize::LARGE:  score += LARGE_ASTEROID_SCORE; break;
                    case AsteroidSize::MEDIUM: score += MEDIUM_ASTEROID_SCORE; break;
                    case AsteroidSize::SMALL:  score += SMALL_ASTEROID_SCORE; break;
                }

                // Split asteroid into smaller pieces if needed
                std::vector<Asteroid> newPieces = asteroids[i].split(renderer);
                asteroids.erase(asteroids.begin() + i); // remove hit asteroid
                if (!newPieces.empty()) {
                    asteroids.insert(asteroids.end(), newPieces.begin(), newPieces.end());
                }

                bulletDestroyed = true;
                
                break; // stop checking other asteroids for this bullet
            }
        }

        if (bulletDestroyed) {
            bulletIt = bullets.erase(bulletIt);
        } else {
            ++bulletIt;
        }
    }
    debugHUD.update(deltaTime);
}

void Engine::renderScore() const {
    scoreFont.display("LEVEL: " + std::to_string(round), 200, 10, 255, 255, 255, 255);
    scoreFont.display(std::to_string(score), SCREEN_WIDTH - 200, 10, 255, 255, 255, 255);
}

void Engine::render() {
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255); // Clear color
    SDL_RenderClear(renderer);
    if (gameState == GameState::LOADING) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_FRect destRect;
        destRect.w = 200; // width you want
        destRect.h = 200; // height you want
        destRect.x = (SCREEN_WIDTH - destRect.w) / 2;  // center horizontally
        destRect.y = SCREEN_HEIGHT / 3;                // some vertical offset
        SDL_RenderTexture(renderer, avatarTexture, nullptr, &destRect);
        loadingFont.display("Press [SPACE] to Start", SCREEN_WIDTH / 2.75f, SCREEN_HEIGHT / 1.7f, 255, 255, 255, 255);
        titleFont.display("HuMaN AiMbOt", SCREEN_WIDTH / 13.0f, SCREEN_HEIGHT / 3.0f, 0, 255, 0, 255);
        instructionsFont.display("Arrow Keys to Move          Space to Fire", SCREEN_WIDTH / 3.3f, SCREEN_HEIGHT / 1.1f, 255, 255, 255, 255);
    } else {
        renderScore();
        if (gameState == GameState::PLAYING) {
            player.render();
        }

        for (auto& bullet : bullets) {
            bullet.render(renderer);
        }

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
            instructionsFont.display("Press [SPACE] to Play Again", SCREEN_WIDTH / 2.7f, SCREEN_HEIGHT / 2.0f, 255, 255, 255, 255);
        }

        if (gameState == GameState::PLAYING && asteroids.empty()) {
            round++;                      // increment round
            spawnAsteroidsForRound();     // spawn next round
        }
    }

    SDL_RenderPresent(renderer);
}

bool Engine::circleRectangleCollision(const Vector2& circleCenter, float circleRadius,
                                      const Vector2& rectTopLeft, float rectWidth, float rectHeight) const {
    float closestX = SDL_clamp(circleCenter.x, rectTopLeft.x, rectTopLeft.x + rectWidth);
    float closestY = SDL_clamp(circleCenter.y, rectTopLeft.y, rectTopLeft.y + rectHeight);

    float dx = circleCenter.x - closestX;
    float dy = circleCenter.y - closestY;

    return (dx * dx + dy * dy) < (circleRadius * circleRadius);
}

bool Engine::circleCircleCollision(const Vector2& aPos, float aRadius,
                                   const Vector2& bPos, float bRadius) const {
    float dx = aPos.x - bPos.x;
    float dy = aPos.y - bPos.y;
    float distanceSq = dx * dx + dy * dy;
    float radiusSum = aRadius + bRadius;
    return distanceSq < (radiusSum * radiusSum);
}

void Engine::collisionCheck() {
    if (!player.isAlive() || player.isInvincible()) return;

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
        player.death();
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
        lives -= 1;
        std::cout << "Remaining lives: " << lives << std::endl;

        if (lives < 0) {
            gameState = GameState::GAMEOVER;
        } else {
            respawnTimer = RESPAWN_DELAY;
        }
    }
}

void Engine::fireBullet() {
    bullets.emplace_back(player.getPosition(), player.getAngle());
    bulletSound.playNow();
    firing = false;
}

void Engine::spawnAsteroidsForRound() {
    int baseCount = 3;
    int asteroidCount = baseCount + round * 2; // more asteroids each round

    asteroids.clear();
    for (int i = 0; i < asteroidCount; ++i) {
        Asteroid a(renderer, player.getPosition());
        a.setVelocity(a.getVelocity() * (1.0f + round * 0.1f));
        asteroids.push_back(a);
    }
}

void Engine::shutdown() {
    player.shutdown();

    for (auto& explosion : explosions) {
        explosion->shutdown(); // you may need to add this method in Explosion
    }
    explosions.clear();

    // Clear asteroids
    asteroids.clear();

    // Unload any static textures
    Explosion::UnloadTexture();
}