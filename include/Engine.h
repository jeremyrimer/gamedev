#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include "Vector2.h"
#include "Player.h"
#include "Asteroid.h"
#include "DebugHUD.h"

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

private:
    SDL_Renderer* renderer;
    TTF_Font* gameOverFont;

    Player player;
    std::vector<Asteroid> asteroids;
    DebugHUD debugHUD;

    GameState gameState = GameState::PLAYING;

    void collisionCheck();
    bool circleRectangleCollision(const Vector2& circleCenter, float circleRadius,
                                  const Vector2& rectTopLeft, float rectWidth, float rectHeight);
    void handlePlayerDeath();
};
