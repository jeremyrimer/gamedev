#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <memory>
#include "Vector2.h"
#include "Player.h"
#include "Asteroid.h"
#include "DebugHUD.h"
#include "Explosion.h"
#include "Constants.h"
#include "Bullet.h"

enum class GameState {
    PLAYING,
    GAMEOVER
};

class Engine {
public:
    Engine(SDL_Renderer* renderer);
    ~Engine();

    void init();
    void handleGlobalInput(const SDL_Event& event, const bool* keyboardState);
    void update(float deltaTime);
    void render();
    void shutdown();

    static SDL_Texture* texture; // Shared by all explosions

private:
    SDL_Renderer* renderer;
    Text gameOverFont;
    DebugHUD debugHUD;
    
    Player player;
    std::vector<Asteroid> asteroids;
    std::vector<std::unique_ptr<Explosion>> explosions;
    std::vector<Bullet> bullets;
    int lives;
    float respawnTimer;
    bool firing = false;
    bool wasFiringLastTick = false;
    GameState gameState = GameState::PLAYING;
    Audio bulletSound;

    void collisionCheck();
    bool circleRectangleCollision(const Vector2& circleCenter, float circleRadius,
                                  const Vector2& rectTopLeft, float rectWidth, float rectHeight) const;
    bool circleCircleCollision(const Vector2& aPos, float aRadius,
                               const Vector2& bPos, float bRadius) const;                             
    void handlePlayerDeath();
    void initGame();
    void fireBullet();
};
