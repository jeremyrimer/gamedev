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
    GAMEOVER,
    LOADING
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
private:
    SDL_Renderer* renderer;
    Text gameOverFont;
    Text loadingFont;
    Text titleFont;
    Text instructionsFont;
    Text scoreFont;
    DebugHUD debugHUD;
    SDL_Texture* avatarTexture;
    
    Player player;
    std::vector<Asteroid> asteroids;
    std::vector<std::unique_ptr<Explosion>> explosions;
    std::vector<Bullet> bullets;
    GameState gameState = GameState::LOADING;
    Audio bulletSound;
    SDL_Gamepad* gamepad = nullptr;

    int lives;
    int score = 0;
    int round = 1;
    float respawnTimer;
    bool firing = false;
    bool wasFiringLastTick = false;

    void initController();
    void spawnAsteroidsForRound();
    void collisionCheck();
    bool circleRectangleCollision(const Vector2& circleCenter, float circleRadius,
                                  const Vector2& rectTopLeft, float rectWidth, float rectHeight) const;
    bool circleCircleCollision(const Vector2& aPos, float aRadius,
                               const Vector2& bPos, float bRadius) const;                             
    void handlePlayerDeath();
    void initGame();
    void fireBullet();
    void renderScore() const;
};
