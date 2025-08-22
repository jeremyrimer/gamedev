#define _USE_MATH_DEFINES
#include "Player.h"
#include "Constants.h"
#include "Vector2.h"
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <cmath>

#define _USE_MATH_DEFINES
#define DEG2RAD(angleDegrees) (((angleDegrees) - 90.0f) * M_PI / 180.0f)

Player::Player(SDL_Renderer* renderer)
    : renderer(renderer),
      angle(PLAYER_STARTING_ANGLE),
      speed(PLAYER_STARTING_SPEED),
      rotationSpeed(PLAYER_STARTING_ROTATION_SPEED),
      thrust(PLAYER_THRUST),
      friction(PLAYER_FRICTION),
      thrusterSound("assets/sound/thrusters.wav"),
      invincibilityTimer(0.0f),
      invincible(false) {
    
    SDL_Surface* surface = IMG_Load("assets/images/player-ship.png");
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        throw std::runtime_error("Failed to load Player Ship image!");
    }
    SDL_DestroySurface(surface);

    size = Vector2(32, 32);
    position = Vector2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);
    velocity = Vector2(0, 0);
}

void Player::handleInput(const bool* keystates) {
    rotatingLeft = keystates[SDL_SCANCODE_LEFT];
    rotatingRight = keystates[SDL_SCANCODE_RIGHT];
    thrusting = keystates[SDL_SCANCODE_UP];
    braking = keystates[SDL_SCANCODE_DOWN];
}

void Player::update(float deltaTime) {
    if (alive) {
         if (invincible) {
            invincibilityTimer -= deltaTime;
            if (invincibilityTimer <= 0.0f) {
                invincible = false;
            }
        }
        if (rotatingLeft) {
            angle -= rotationSpeed * deltaTime;
            // TODO - set to 0 past 360
             if (angle < 0.0f) angle = 360;
        }
        if (rotatingRight) {
            angle += rotationSpeed * deltaTime;
            // TODO - set to 360 when past 0
            if (angle > 360.0f) angle = 0;
        }

        float radians = static_cast<float>(DEG2RAD(angle));
        Vector2 direction = Vector2(std::cos(radians), std::sin(radians));

        if (thrusting) {
            velocity += direction * thrust * deltaTime;
        }
        if (braking) {
            velocity -= direction * thrust * deltaTime;
        }

        position += velocity * deltaTime;

        float halfW = size.x / 2.0f;
        float halfH = size.y / 2.0f;

        // Screen wrap
        if (position.x < -halfW) position.x = SCREEN_WIDTH + halfW;
        if (position.x > SCREEN_WIDTH + halfW) position.x = -halfW;
        if (position.y < -halfH) position.y = SCREEN_HEIGHT + halfH;
        if (position.y > SCREEN_HEIGHT + halfH) position.y = -halfH;

        // Friction
        velocity -= velocity * friction * deltaTime;

        // Thruster audio
        if (thrusting) {
            thrusterSound.play();
            thrusterSound.updateForLooping();
        } else {
            thrusterSound.stop();
        }
    } else {
        // move player off screen immediately
        position.y = SCREEN_HEIGHT * 2;
        position.x = SCREEN_HEIGHT * 2;
        thrusterSound.stop();
    }
}

void Player::render() {
    if (!alive) return;

    // If invincible, flicker every ~0.2s
    if (invincible) {
        int flicker = static_cast<int>(invincibilityTimer * 10) % 2;
        if (flicker == 0) return; // skip draw this frame
    }

    SDL_FRect dest;
    dest.w = size.x;
    dest.h = size.y;
    dest.x = position.x - size.x / 2;
    dest.y = position.y - size.y / 2;

    SDL_FPoint center = { size.x / 2, size.y / 2 };
    SDL_RenderTextureRotated(renderer, texture, nullptr, &dest, angle, &center, SDL_FLIP_NONE);

    if (thrusting) {
        renderThruster();
    }
}

void Player::renderThruster() {
    if (rand() % 100 < 90) {
        float flameLength = 18.0f + (rand() % 6);
        float flameSpread = 0.5f + (rand() % 3);

        float rad = static_cast<float>(DEG2RAD(angle));
        float rearX = position.x - std::cos(rad) * (size.x / 2);
        float rearY = position.y - std::sin(rad) * (size.y / 2);

        int r = 240 + rand() % 16;
        int g = 80 + rand() % 30;
        int b = 0;

        SDL_Vertex verts[3];
        verts[0].position = { rearX - std::cos(rad) * flameLength,
                              rearY - std::sin(rad) * flameLength };
        verts[1].position = { rearX + std::sin(rad) * flameSpread,
                              rearY - std::cos(rad) * flameSpread };
        verts[2].position = { rearX - std::sin(rad) * flameSpread,
                              rearY + std::cos(rad) * flameSpread };

        for (int i = 0; i < 3; ++i)
            verts[i].color = { static_cast<float>(r), static_cast<float>(g), static_cast<float>(b), 255 };

        SDL_RenderGeometry(renderer, nullptr, verts, 3, nullptr, 0);
    }
}

void Player::respawn(Vector2 pos) {
    position = pos;
    velocity = {0, 0};
    angle = PLAYER_STARTING_ANGLE;
    alive = true;
    invincible = true;
    invincibilityTimer = 3.0f; // 3 seconds of safety
}

Vector2 Player::getPosition() const {
    return position;
}

Vector2 Player::getSize() const {
    return size;
}

void Player::setAlive(bool liveOrDead) { 
    alive = liveOrDead; 
}

bool Player::isAlive() const {
    return alive;
}