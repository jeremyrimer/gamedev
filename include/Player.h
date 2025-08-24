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

    Vector2 getPosition() const;
    Vector2 getSize() const;
    float getAngle() const { return angle; }
    Vector2 getVelocity() const { return velocity; }

    void setAlive(bool liveOrDead);
    bool isAlive() const;
    bool isInvincible() const;
    void respawn(Vector2 pos, bool startInvincible);
    void shutdown(); 

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
    bool alive = true;
    bool invincible = false;
    float invincibilityTimer;
    bool shuttingDown = false;

    Audio thrusterSound;

    void renderThruster();
};
