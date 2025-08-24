#pragma once
#include <SDL3/SDL.h>
#include "Vector2.h"

class Bullet {
public:
    Bullet(Vector2 position, float angle);

    void update(float deltaTime);
    void render(SDL_Renderer* renderer);
    bool isAlive() const { return life > 0.0f; }

private:
    Vector2 pos;
    Vector2 vel;
    float life;
};
