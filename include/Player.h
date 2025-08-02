#pragma once

#include <SDL3/SDL.h>
#include "Vector2.h"
#include "Audio.h"

class Player {
public:
    Player(SDL_Renderer* renderer);

    void handleInput(const bool* keystates);
    void update(float deltaTime);
    void render();

    Vector2 getPosition() const { return position; }
    Vector2 getSize() const { return size; }
    float getRadius() const { return size.x * 0.5f; } // ship is square
    float getAngle() const { return angle; }
    Vector2 getVelocity() const { return velocity; }

    void setAlive(bool alive) { isAlive = alive; }
    bool isAliveNow() const { return isAlive; }

private:
    SDL_Renderer* renderer;
    SDL_Texture* texture;

    Vector2 position;  // Center of ship
    Vector2 size;      // Width and height
    Vector2 velocity;

    float angle;
    float speed;
    float rotationSpeed;
    float thrust;
    float friction;

    bool rotatingLeft = false;
    bool rotatingRight = false;
    bool thrusting = false;
    bool braking = false;
    bool isAlive = true;

    Audio thrusterSound;

    void renderThruster();
};
