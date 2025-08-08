#pragma once
#include <SDL3/SDL.h>
#include "Player.h"
#include "Vector2.h"
#include <string>
#include <SDL3_ttf/SDL_ttf.h>
#include "Text.h"

class DebugHUD {
public:
    DebugHUD(SDL_Renderer* renderer, Player* player);
    void handleInput(const bool* keystates);
    void update(float deltaTime);
    void render();

private:
    Player* player;
    SDL_Renderer* renderer;
    void renderText(const std::string &text, int x, int y);
    bool lastF3State = false;
    bool visible = false;
    Vector2 playerBounds;
    float playerAngle;
    Vector2 playerVelocity;
    Text font;
};