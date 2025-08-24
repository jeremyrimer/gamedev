#pragma once
#include <SDL3/SDL.h>
#include "Vector2.h"
#include "Constants.h"

class Bullet {
public:
    Bullet(Vector2 startPos, float direction);
    void update(float dt);
    void render(SDL_Renderer* renderer);
    bool isAlive() const;
    float getRadius() const;
    Vector2 getPosition() const;

private:
    Vector2 pos;
    Vector2 vel;
    float speed;
    float lifetime;
    bool active;
    float radius;
};